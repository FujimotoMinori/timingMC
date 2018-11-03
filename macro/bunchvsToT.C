/*
	 bunchvsToT.C
	 make tree for MC and make histograms of ToT
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

int bunchvsToT(){
	cout << "----- start bunchvsToT.cxx -----" << endl;

	string finname = "/Users/fujimoto/Desktop/data/outputMC2017ith.root";
	//string foutname = "../data/test3.root";

	//file open
	TFile* fin = TFile::Open(finname.c_str(), "READ");
	if (!fin) {
		cout << " input file:" << finname.c_str() << " does not exist"<< endl;
		return 0;
	}
	cout << " input data file:" << finname.c_str() << " open..." << endl;

	//get tree
	TTree *tin = (TTree*)fin->Get("chargeMC");
	cout << "----- get tree -----" << endl;
	//tin->Print();

	//make histogram
	TH1F *htot = new TH1F("htot","ToT",21,0,20);
	TH1F *hcharge = new TH1F("hcharge","charge",100,0,30000);
	//TH1F *h2 = new TH1F("h2","bunch",5,0.,2.5);
	TH2F *hbunch = new TH2F("hbunch","bunchVSToT;bunch;ToT",3,-0.5,2.5,20,1,20);
	TH2F *hbunch2 = new TH2F("hbunch2","bunchVScharge;bunch;charge",3,-0.5,2.5,60,4000,15000);
	TH2F *hQvsToT = new TH2F("hbunch","chargeVSToT;charge;ToT",20,3500,10000,20,1,20);
	TH1F *h2 = new TH1F("h2","bunch",11,-5,5);

	//TFile* fout = TFile::Open(foutname.c_str(), "RECREATE"); 
	
	//get branch 
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
	//cout << "-------------------------------------charge =" << charge << endl;
          hcharge->Fill(charge);
          h2->Fill(bunch);
          if(bunch<3&&ToT<20&&ToT>5&&bec==0&&layerID==2/*&&abs(moduleID)==0*/){
          hQvsToT->Fill(charge,ToT);
          hbunch->Fill(bunch+1,ToT);
          hbunch2->Fill(bunch+1,charge);
          htot->Fill(ToT);
		}
	}

	//draw histogram
	TCanvas *c1 = new TCanvas("c1", "c1");
	htot->Draw();
	TCanvas *c2 = new TCanvas("c2", "c2");
	hbunch->SetStats(0);
	hbunch->Draw("text box");
	TCanvas *c3 = new TCanvas("c3", "c3");
	hcharge->Draw();
	TCanvas *c4 = new TCanvas("c4", "c4");
	//h2->Draw();
	//hcharge->Draw();
	hQvsToT->Draw("lego");
	TCanvas *c5 = new TCanvas("c5", "c5");
	hbunch2->Draw("text box");
	
	//get content of histgram
	double z[hbunch->GetNbinsX()+1][hbunch->GetNbinsY()+1];
	for (int i=1; i<hbunch->GetNbinsX()+1; i++){  
		for (int j=0; j<hbunch->GetNbinsY()+1; j++){
			z[i][j] = hbunch->GetBinContent(hbunch->GetBin(i,j));
		}
	}

	//get the prob
	double prob=0.;
	double e1=0.;
	double e2=0.;
	double ep=0.;
	double n1=0.;
	double n2=0.;
	for (int j=0; j<hbunch->GetNbinsY()+1; j++){
		if(z[2][j]+z[3][j] == 0) continue;
		n1 = z[2][j];
		n2 = z[3][j];
		prob = n2/(n1+n2);
		//calculate error of prob
		e1 = pow(n1,0.5);
		e2 = pow(n2,0.5);
		ep = sqrt(pow(n2*e1,2)+pow(n1*e2,2))/pow((n1+n2),2);

		//cout << "prob when ToT" << j << "= " << prob <<  " error= " << ep << endl; 
		cout <<  j << " " << prob <<  " " << ep << endl; 
	}

	//TCanvas *c3 = new TCanvas("c3", "c3");
	//h3->Draw();

	//fout->Write();
	//fout->Close();

	cout << "----- finished  bunchvsToT.cxx -----" << endl;

	return 0;

}

