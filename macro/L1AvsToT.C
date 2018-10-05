/*
	 L1AvsToT.C
	 make tree of data and make histogram of bunch and ToT
 */

#include <iostream>
#include <sstream>
#include <TFile.h>
#include <TH1.h>
#include <TH2D.h>
#include <TH2F.h>
#include <TCanvas.h>
using namespace std;

int L1AvsToT(){
	cout << "----- start L1AvsToT.cxx -----" << endl;

	string finname = "../data/TimingChargePix.root";
	string foutname = "../data/test3.root";

	//file open
	TFile* fin = TFile::Open(finname.c_str(), "READ");
	if (!fin) {
		cout << " input file:" << finname.c_str() << " does not exist"<< endl;
		return 0;
	}
	cout << " input data file:" << finname.c_str() << " open..." << endl;

	//get tree
	TTree *tin = (TTree*)fin->Get("timingCharge");
	cout << "----- get tree -----" << endl;
	//tin->Print();

	//make histogram
	TH1F *h1 = new TH1F("h1","ToT",40,1,40);
	//TH1F *h1 = new TH1F("h1","ToT",21,-10,10);
	TH2F *h2 = new TH2F("h2","bunchVSToT;L1A;ToT",3,-0.5,2.5,20,1,20);
	TH1F *h3 = new TH1F("h3","bunch",11,-5,5);

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
		int tot;
		tin->GetEntry(ientry);    
		h1->Fill(ToT);
		h3->Fill(L1A);
		if(L1A<3&&ToT<20&&bec==0&&layer==1/*&&abs(eta_index)==6*/){
			h2->Fill(L1A,ToT);
		}
	}

	//draw histogram
	TCanvas *c1 = new TCanvas("c1", "c1");
	h1->Draw();
	TCanvas *c2 = new TCanvas("c2", "c2");
	h2->Draw("text box");
	h2->SetStats(0);
	TCanvas *c3 = new TCanvas("c3", "c3");
	h3->Draw();

	//get content of histgram
	double z[h2->GetNbinsX()+1][h2->GetNbinsY()+1];
	for (int i=1; i<h2->GetNbinsX()+1; i++){  
		for (int j=0; j<h2->GetNbinsY()+1; j++){
			z[i][j] = h2->GetBinContent(h2->GetBin(i,j));
		}
	}
		
	//get the prob
	double prob=0.;
	double e1=0.;
	double e2=0.;
	double ep=0.;
	double n1=0.;
	double n2=0.;
	for (int j=0; j<h2->GetNbinsY()+1; j++){
	  if((z[2][j]+z[3][j]) == 0) continue;
		//prob = z[3][j]/(z[2][j]+z[3][j]);
		n1 = z[2][j];
		n2 = z[3][j];
		prob = n2/(n1+n2);
		//calculate error of prob
		e1 = pow(n1,0.5);
		e2 = pow(n2,0.5);
		ep = sqrt(pow(n2*e1,2)+pow(n1*e2,2))/pow((n1+n2),2);

		cout << "prob when ToT" << j << "= " << prob <<  " error= " << ep << endl; 
	}
	 

	fout->Write();
	fout->Close();

	cout << "----- finished  L1AvsToT.cxx -----" << endl;

	return 0;

}



