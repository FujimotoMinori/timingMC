/*
	 dq.C
	 to see the data quality for each module:ver1
 */

#include <iostream>
#include <sstream>
#include <TFile.h>
#include <TH1.h>
#include <TH2D.h>
#include <TH2F.h>
#include <TCanvas.h>
using namespace std;

void dq(){
	cout << "----- start dq.cxx -----" << endl;

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

	TFile* fout = TFile::Open(foutname.c_str(), "RECREATE");

  //Draw histogram
  TH2F *h2 = new TH2F("h2","",40,-20,20,60,0,59);
  tin->Draw("phi_index:eta_index >> h2 ","","colz");

	fout->Write();
	fout->Close();
	
	cout << "----- finished  dq.cxx -----" << endl;
	}

