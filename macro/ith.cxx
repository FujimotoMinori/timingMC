/*
	 ith.C
	 check the correlation of charge and ToT in every modules
 */

#include <iostream>
#include <sstream>
#include <TFile.h>
#include <TH1.h>
#include <TH2D.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TString.h>
#include <cmath>
using namespace std;

void ith(){
	cout << "----- start ith.cxx -----" << endl;

	string finname = "/Users/fujimoto/Desktop/data/outputMC2017final.root";
	//string foutname = "../data/test3.root";

	//file open
	TFile* fin = TFile::Open(finname.c_str(), "READ");
	if (!fin) {
		cout << " input file:" << finname.c_str() << " does not exist"<< endl;
		return;
	}
	cout << " input data file:" << finname.c_str() << " open..." << endl;

	//get tree
	TTree *tin = (TTree*)fin->Get("chargeMC");
	cout << "----- get tree -----" << endl;

	//make histogram
	TH1F *htot = new TH1F("htot","ToT",21,0,20);
	TH1F *hcharge = new TH1F("hcharge","charge",100,0,30000);
	TH2F *hQvsToT = new TH2F("hbunch","chargeVSToT;charge;ToT",40,3500,10000,20,0,20);

	//TFile* fout = TFile::Open(foutname.c_str(), "RECREATE"); 
	
	Int_t bec;
	Int_t layerID;
	Int_t moduleID;
	Int_t bunch;
	float charge;
	Int_t ToT;
	tin->SetBranchAddress("bec", &bec);
	tin->SetBranchAddress("layerID", &layerID);
	tin->SetBranchAddress("moduleID", &moduleID);
	tin->SetBranchAddress("bunch", &bunch);
	tin->SetBranchAddress("charge", &charge);
	tin->SetBranchAddress("ToT", &ToT);
	const Int_t N = tin->GetEntries();
	cout << "entry number=" << N << endl;
	 
	//fill in histogram
	for (Int_t ientry = 0; ientry < N; ientry++) {
		tin->GetEntry(ientry);
		hcharge->Fill(charge);
		htot->Fill(ToT);
		if(bunch<3&&ToT<20&&ToT>5&&bec==0&&layerID==2/*&&abs(moduleID)==0*/){
			hQvsToT->Fill(charge,ToT);
			//hbunch->Fill(bunch,ToT);
			//hbunch2->Fill(bunch,charge);
			htot->Fill(ToT);
		}
	}
	
	//get content of histgram
	Int_t first = hQvsToT->GetXaxis()->FindBin(5000);
	Int_t last  = hQvsToT->GetXaxis()->FindBin(8000);
   
  /*
	double z[hbunch->GetNbinsX()+1][hbunch->GetNbinsY()+1];
	for (int i=1; i<hbunch->GetNbinsX()+1; i++){  
		for (int j=0; j<hbunch->GetNbinsY()+1; j++){
			z[i][j] = hbunch->GetBinContent(hbunch->GetBin(i,j));
		}
	}
	*/

	//draw histogram
	TCanvas *c1 = new TCanvas("c1", "c1");
	htot->Draw();
	TCanvas *c2 = new TCanvas("c2", "c2");
	hcharge->Draw();
	TCanvas *c3 = new TCanvas("c3", "c3");
	hQvsToT->Draw("box");
	//TCanvas *c4 = new TCanvas("c4", "c4");
	//hQvsToT->ProjectionY("",first,last)->Draw();
	hQvsToT->ProjectionX("",6,7)/*->Draw()*/;
    double edge;
    double mean;
    double rms;
    for(int i = 6;i<14;i++){
      mean = hQvsToT->ProjectionX("",i,i+1)->GetMean();
      rms = hQvsToT->ProjectionX("",i,i+1)->GetRMS();
      edge = mean+rms;
      std::cout << "edge of ToT " << i << "= " << edge << std::endl; 
    }
	
	//fout->Write();
	//fout->Close();

	cout << "----- finished  bunchvsToT.cxx -----" << endl;

	return;

}


