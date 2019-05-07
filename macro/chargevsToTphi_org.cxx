/*
	 chargevsToTphi_org.cxx
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

int chargevsToTphi_org(){
	string finname = "../data/outputMC0424_2.root";
	string foutname = "totphi_org.root";

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
	Int_t phiID;
	Int_t bunch;
	float charge;
	Int_t ToT;
	tin->SetBranchAddress("bec", &bec);
	tin->SetBranchAddress("layerID", &layerID);
	tin->SetBranchAddress("moduleID", &moduleID);
	tin->SetBranchAddress("phiID", &phiID);
	tin->SetBranchAddress("bunch", &bunch);
	tin->SetBranchAddress("charge", &charge);
	tin->SetBranchAddress("ToT", &ToT);
	const Int_t N = tin->GetEntries();
	cout << "entry number=" << N << endl;


	TFile* fout = TFile::Open(foutname.c_str(), "RECREATE"); 
	TH1F *hphi = new TH1F("hphi","",61,0,60); 

	//layerID loop
	//TH2F* hedge[3];
	////for(int LID=2;LID<3;LID++){
	//int LID = 2;
	//std::cout << "layer ID=" << LID << std::endl;
	//hedge[LID] = new TH2F(Form("edge%d",LID),Form("layer#%d",LID),51,0,49,7,4,10);
	TH2F *hedge = new TH2F("edge","",51,0,50,7,4,10);
	
  //make histogram
  const int nToT=6;
  const int nmodule=50;
  //TH1F* hToT[nToT][nmodule];
  TH1F* hToT[6][50];
	for (int i=4;i<nToT+4;i++){
		for (int j=0;j<nmodule;j++){
			hToT[i][j] = new TH1F(Form("tot%dmodule%d",i,j),"",20,2000,15000);
		}
	}

	//fill in histogram
	for (Int_t ientry = 0; ientry < N; ientry++) {
		tin->GetEntry(ientry);
		//std::cout << "phiID= " << phiID << std::endl;
		hphi->Fill(phiID);
		if(bec == 0 && layerID == 2){
			for(int i=4;i<nToT+4;i++){
				for (int j=0;j<nmodule;j++){
					if(ToT == i) {
						if(abs(phiID) == j){
							hToT[i][j]->Fill(charge);
						}
					}
				}
			}
		}
	}
	cout << "filled in histograms" << endl;

	//draw histogram
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
			edge[i][j] = mean+sigma;
			cout << "edge in ToT " << i << "in module " << j << "= " << edge[i][j] << endl;
			hedge->Fill(j,i,edge[i][j]);
		}
	}

  hedge->SetStats(0);
	//hedge[LID]->Draw("text45 colz");
	//std::cout << "LID= " << LID << std::endl;
  hedge->Write();

	//}//layerID loop

	hphi->Draw();
	hphi->Write();

	fout->Close();
	return 0;
}


