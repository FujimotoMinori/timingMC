/*
   L1AvsToT2018.C
   make tree of data and make histogram of bunch and ToT
   for latest 2018 data
 */

#include <iostream>
#include <sstream>
#include <string>
#include <TFile.h>
#include <TH1.h>
#include <TH2D.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TString.h>
using namespace std;

int L1AvsToT2018(){

    string finname = "/Users/fujimoto/Desktop/data/timingCharge_361635.root";
    TString ifndata = "/Users/fujimoto/Desktop/data/modulecutnew.dat";

    //first read data file
    //file open
    fstream findata;
    std::string strdata;
    findata.open(ifndata);
    if(findata.fail()){
        cerr << "cannot open data file : " << ifndata << std::endl;
        return 1;
    }

    //read file
    const int n_bec = 1000;
    const int n_layer = 1000;
    const int n_phi = 1000;
    const int n_eta = 1000;
    int a,b,c,d;
    int becF[n_bec] = {};
    int layerF[n_layer] = {};
    int phi[n_phi] = {};
    int eta[n_eta] = {};
    int n = 0;

    while(getline(findata,strdata))
    {
        if(strdata[0] == '#') continue;
        n++;
        a = 0;
        b = 0;
        c = 0;
        d = 0;
        sscanf(strdata.data(), "%d %d %d %d", &a, &b, &c, &d);
        becF[n] = a;
        layerF[n] = b;
        phi[n] = c;
        eta[n] = d;
    }
    //cout << " " << becF[1] << " " << layerF[1]<< " "  << phi[1]<< " "  << eta[1] << endl;

    //file open
    TFile* fin = TFile::Open(finname.c_str(), "READ");
    if (!fin) {
        cout << " input file:" << finname.c_str() << " does not exist"<< endl;
        return 0;
    }
    cout << " input data file:" << finname.c_str() << " open..." << endl;
    
    std::ofstream fout("data2018blayer.txt");

    //get tree
    TTree *tin = (TTree*)fin->Get("timingCharge");
    cout << "----- get tree -----" << endl;

    //make histogram
    TH1F *h1 = new TH1F("h1","ToT",40,1,40);
    //TH2F *h2 = new TH2F("h2","bunchVSToT;L1A;ToT",3,-0.5,2.5,20,1,20);
    TH1F *h3 = new TH1F("h3","bunch",10,-5.,5.);

    //get branch
    Int_t bec;
    Int_t layer;
    Int_t L1A;
    Int_t ToT;
    Int_t eta_index;
    Int_t phi_index;
    tin->SetBranchAddress("bec", &bec);
    tin->SetBranchAddress("layer", &layer);
    tin->SetBranchAddress("L1A", &L1A);
    tin->SetBranchAddress("ToT", &ToT);
    tin->SetBranchAddress("eta_index", &eta_index);
    tin->SetBranchAddress("phi_index", &phi_index);
    const Int_t N = tin->GetEntries();
    cout << "entry number=" << N << endl;
    cout << "filling in histogram........." << endl;

    TH2F* hL1A[3];
    for(int LID=1;LID<2;LID++){ //Layer ID loop
        std::cout << "layer ID=" << LID << std::endl;
        hL1A[LID] = new TH2F(Form("L1A%d",LID),Form("layer#%d",LID),3,-0.5,2.5,20,1,20);

        //fill in histogram
        int nbingo = 0;
        for (Int_t ientry = 0; ientry < N; ientry++) {
            tin->GetEntry(ientry);    
            bool bingo = false;

            for(int i =1;i<n;i++){
                if(bec == becF[i] && layer == layerF[i] && phi_index == phi[i] && eta_index == eta[i]) {
                    bingo = true;
                    nbingo++;
                }
            }

            if(bingo == false){
                h1->Fill(ToT);
                //h3->Fill(L1A);
                if(L1A<3&&ToT<20&&bec==0&&layer==LID){
                    hL1A[LID]->Fill(L1A,ToT);
                }
            }

        }

        cout << "nbingo =  " << nbingo << endl;
        cout << "finished filling in histogram" << endl;

        //draw histogram
        TCanvas *c1 = new TCanvas("c1", "c1");
        h1->Draw();
        //TCanvas *c2 = new TCanvas("c2", "c2");
        //hL1A[LID]->Draw("text box");
        //hL1A[LID]->SetStats(0);
        //TCanvas *c3 = new TCanvas("c3", "c3");
        //h3->Draw();

        //get content of histgram
        double z[hL1A[LID]->GetNbinsX()+1][hL1A[LID]->GetNbinsY()+1];
        for (int i=1; i<hL1A[LID]->GetNbinsX()+1; i++){  
            for (int j=0; j<hL1A[LID]->GetNbinsY()+1; j++){
                z[i][j] = hL1A[LID]->GetBinContent(hL1A[LID]->GetBin(i,j));
            }
        }

        //get the prob and error
        double prob=0.;
        double e1=0.;
        double e2=0.;
        double ep=0.;
        double n1=0.;
        double n2=0.;
        for (int j=0; j<hL1A[LID]->GetNbinsY()+1; j++){
            if((z[2][j]+z[3][j]) == 0) continue;
            n1 = z[2][j];
            n2 = z[3][j];
            prob = n2/(n1+n2);
            e1 = pow(n1,0.5);
            e2 = pow(n2,0.5);
            ep = sqrt(pow(n2*e1,2)+pow(n1*e2,2))/pow((n1+n2),2);

            cout << "prob when ToT" << j << "= " << prob <<  " error= " << ep << endl; 
            fout << j << " " << prob << " " << ep << " " << endl;
        }
    }
    fout.close();

    cout << "----- finished  L1AvsToT.cxx -----" << endl;
    return 0;
}

