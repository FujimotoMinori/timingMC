/*
	 chargevsToT.cxx
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

int chargevsToT(){
	string finname = "/Users/fujimoto/Desktop/data/outputMC0507.root";
	//string foutname = "../rootfile/tot.root";

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

  //set output file
	//TFile* fout = TFile::Open(foutname.c_str(), "RECREATE"); 
	//std::ofstream summary("../text/chargedistribution.txt");

  //make canvas
  //TString name;
	//name.Form("canv.pdf");
	//TCanvas *c1 = new TCanvas(name.Data(), name.Data(), 1000, 500);
	//c1->Print(name + "[", "pdf"); 

	//layerID loop
	TH2F* hedge[3];
	for(int LID=1;LID<4;LID++){
	std::cout << "layer ID=" << LID << std::endl;
	//summary << "layer ID=" << LID << endl;
	hedge[LID] = new TH2F(Form("edge%d",LID),Form("layer#%d",LID),7,0,7,7,4,10);
	
  //make histogram
  const int nToT=6;
  const int nmodule=7;
  TH1F *hToT[3][nToT][nmodule];
	for (int i=4;i<nToT+4;i++){
		for (int j=0;j<nmodule;j++){
			hToT[LID][i][j] = new TH1F(Form("LID%dtot%dmodule%d",LID,i,j),"",40,2000,15000);
		}
	}
	cout << "made histograms" << endl;

	//fill in histogram
	for (Int_t ientry = 0; ientry < N; ientry++) {
		tin->GetEntry(ientry);
		if(bec == 0 && layerID == LID){
			for(int i=4;i<nToT+4;i++){
				for (int j=0;j<nmodule;j++){
					if(ToT == i) {
						if(abs(moduleID) == j){
							hToT[LID][i][j]->Fill(charge);
						}
					}
				}
			}
		}
	}
	cout << "filled in histograms" << endl;

	//draw histogram
	hToT[1][6][1]->Draw();
    hToT[1][6][1]->Write();
    //fit histogram
	//TF1 * f1 = new TF1("func","gaus",2000,8000);
	double edge[nToT][nmodule] = {};
	double edgetotal[nToT] = {};
	for(int i=4;i<nToT+4;i++){
		for (int j=0;j<nmodule;j++){
			double mean,sigma = 0;
			//hToT[i][j]->Fit("func","Q");
			//mean = f1->GetParameter(1);
			//sigma = f1->GetParameter(2);
			mean = hToT[LID][i][j]->GetMean();
			sigma = hToT[LID][i][j]->GetRMS();
			edge[i][j] = mean+sigma;
			cout << "edge in ToT " << i << "in module " << j << "= " << edge[i][j] << endl;
			hedge[LID]->Fill(j,i,edge[i][j]);
			edgetotal[i] += edge[i][j];

            //hToT[LID][i][j]->Draw("same");
        }
			cout << "--------------------------mean edge in ToT " << i << " of all module= " << edgetotal[i]/nmodule << endl;
			//summary << "--------------------------mean edge in ToT " << i << " of all module= " << edgetotal[i]/nmodule << endl;
	}

    //hedge[LID]->SetStats(0);
    //hedge[LID]->Draw("text45 colz");
    //hedge[LID]->Write();

	}//layerID loop

	//fout->Close();
	//summary.close();

	return 0;
}

