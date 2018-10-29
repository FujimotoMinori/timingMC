/*
	 chargevsToT_org.cxx
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

int chargevsToT_org(){
	string finname = "/Users/fujimoto/Desktop/data/outputMC1028.root";
	string foutname = "tot_org.root";

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

	TFile* fout = TFile::Open(foutname.c_str(), "RECREATE"); 
	
  //make histogram
  const int nToT=6;
  const int nmodule=7;
  TH1F *hToT[nToT][nmodule];
  TH1F *hToT2[nToT];
	for (int i=4;i<nToT+4;i++){
		for (int j=0;j<nmodule;j++){
			hToT[i][j] = new TH1F(Form("tot%dmodule%d",i,j),";charge;",80,2000,15000);
			hToT[i] = new TH1F(Form("tot%dmodule%d",i,j),";charge;",80,2000,15000);
		}
	}
	//TH1F* hToT4 = new TH1F("tot4","",30,2000,10000);
	TH1F* hmean = new TH1F("hmean","",100,2000,15000);
	TH2F *hedge = new TH2F("edge","",7,0,7,7,4,10);
	TH2F *hedge2 = new TH2F("edge","",7,0,7,7,4,10);
	cout << "made histograms" << endl;

	//fill in histogram
	for (Int_t ientry = 0; ientry < N; ientry++) {
		tin->GetEntry(ientry);
		if(bec == 0 && layerID == 1){
			for(int i=4;i<nToT+4;i++){
				for (int j=0;j<nmodule;j++){
					if(ToT == i) {
						if(abs(moduleID) == j){
							hToT[i][j]->Fill(charge);
						}
					}
				}
			}
		}
	}
	cout << "filled in histograms" << endl;

  //THStack *hs = new THStack("hs","test stacked histograms");
	//draw histogram
	hToT[6][1]->Write();
	hToT[6][4]->Write();
	//fit histogram
	//TF1 * f1 = new TF1("func","gaus",2000,8000);
	double edge[nToT][nmodule] = {};
	for(int i=4;i<nToT+4;i++){
		for (int j=0;j<nmodule;j++){
			double mean,sigma = 0;
			//hToT[i][j]->Fit("func","Q");
			//mean = f1->GetParameter(1);
			//sigma = f1->GetParameter(2);
			mean = hToT[i][j]->GetMean();
			sigma = hToT[i][j]->GetRMS();
			//hmean->Fill(mean);
			edge[i][j] = mean+sigma;
			cout << "edge in ToT " << i << "in module " << j << "= " << edge[i][j] << endl;
			hedge->Fill(j,i,edge[i][j]);
		}
	}

  TCanvas* c1 = new TCanvas("c1");
	double meantot[nmodule] = {};
	double meanofmean,sigmaofmean;
	for (int j=0;j<2;j++){
		//hs->Add(hToT[4][j]);
		hToT[4][j]->Draw("same");
		meantot[j] = hToT[4][j]->GetMean();
		hmean->Fill(meantot[j]);
	}
	meanofmean = hmean->GetMean();
	sigmaofmean = hmean->GetRMS();
	cout << "meanofmean= " << meanofmean << " sigmaofmean= " << sigmaofmean << endl;
	c1->Write();
	//hs->Draw("nostack");
	//hs->Write();

  TCanvas* c2 = new TCanvas("c2");
	hedge->Draw("text colz");
  hedge->Write();
	hmean->Write();

	fout->Close();
	
	return 0;
}



