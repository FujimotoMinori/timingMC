/*
	 dq2.C
	 to see the data quality of each modules
 */

#include <iostream>
#include <sstream>
#include <TFile.h>
#include <TH1.h>
#include <TH2D.h>
#include <TH2F.h>
#include <TCanvas.h>
using namespace std;

void dq2(){
	cout << "----- start dq2.cxx -----" << endl;

	string finname = "../data/TimingChargeIBL.root";
	string foutname = "../data/test.root";

	//file open
	TFile* fin = TFile::Open(finname.c_str(), "READ");
	if (!fin) {
		cout << " input file:" << finname.c_str() << " dose not exist"<< endl;
		return 0;
	}
	cout << " input data file:" << finname.c_str() << " open..." << endl;

    //get tree
    TTree *tin = (TTree*)fin->Get("timingCharge");
		cout << "----- get tree -----" << endl;
		tin->Print();

    //make histogram
    TH2F *h2 = new TH2F("h2","eta_phi_layer0;eta_index;phi_index",40,-20,20,60,0,59);
    //TH2F *h2l1 = new TH2F("h2l1","eta_phi;eta_index;phi_index",20,-10,10,60,0,59);
    //TH2F *h2l2 = new TH2F("h2l2","eta_phi;eta_index;phi_index",20,-10,10,60,0,59);
    //TH2F *h2l3 = new TH2F("h2l3","eta_phi;eta_index;phi_index",20,-10,10,60,0,59);
    
    TFile* fout = TFile::Open(foutname.c_str(), "RECREATE");

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

    //fill in histogram
    for (Int_t ientry = 0; ientry < N; ientry++) {
        tin->GetEntry(ientry);    
				if(layer==0){
        	h2->Fill(eta_index,phi_index);
				}
    }

    //draw histogram
    //h2->Draw("colz");
    h2->SetStats(0);

    fout->Write();
    fout->Close();

    //make canvas
    TString name;
    name.Form("canvi0.pdf");
    TCanvas *c1 = new TCanvas(name.Data(), name.Data(), 1000, 800);
    c1->Print(name + "[", "pdf");    
    //for (Int_t ihist = 0; ihist < Nhists; ihist++) {
        h2->Draw("colz");
        c1->Print(name, "pdf");       
    //}

    c1->Print(name + "]", "pdf");    

	cout << "----- finished  dq2.cxx -----" << endl;
	}


