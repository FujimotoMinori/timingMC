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

int getrightprob(){

	string finname = "/Users/fujimoto/Desktop/data/outputMC0507.root";
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

	//make histogram
	TH2F *hbunch = new TH2F("hbunch","bunchVSToT;bunch;ToT",3,-0.5,2.5,20,1,20);

    std::ofstream fout("tuned2018blayer0507.txt");
	
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
	 
    //Event Loop
	for (Int_t ientry = 0; ientry < N; ientry++) {
		  tin->GetEntry(ientry);

          //move hits to next bunches
          double prob = 0.;
          if (bec == 2 && layerID ==0){
              if      (charge < 5550) prob = 0.124; //ToT = 6
              else if (charge < 6000) prob = 0.080;  //ToT = 7
              else if (charge < 6400) prob = 0.0005; //ToT = 8
              else if (charge < 6500) prob = 0.002; //ToT = 9
              else if (charge < 6800) prob = 0.040;  //ToT = 10
              else if (charge < 7300) prob = 0.031;  //ToT = 11
              else if (charge < 7400) prob = 0.04;  //ToT = 12
              else if (charge < 7500) prob = 0.001;  //ToT = 13
          }
          if (bec == 2 && layerID ==1){
              if      (charge < 5550) prob = 0.110; //ToT = 6
              else if (charge < 6000) prob = 0.061;  //ToT = 7
              else if (charge < 6400) prob = 0.0001; //ToT = 8
              else if (charge < 6500) prob = 0.0005; //ToT = 9
              else if (charge < 6800) prob = 0.040;  //ToT = 10
              else if (charge < 7300) prob = 0.031;  //ToT = 11
              else if (charge < 7400) prob = 0.04;  //ToT = 12
              else if (charge < 7500) prob = 0.001;  //ToT = 13
          }
          if (bec == 2 && layerID ==2){
              if      (charge < 5400) prob = 0.1800; //ToT = 6
              else if (charge < 5700) prob = 0.067;  //ToT = 7
              else if (charge < 5760) prob = 0.048; //ToT = 8
              else if (charge < 5800) prob = 0.040; //ToT = 9
              else if (charge < 5880) prob = 0.040;  //ToT = 10
              else if (charge < 6000) prob = 0.040;  //ToT = 11
              else if (charge < 6500) prob = 0.034;  //ToT = 12
              else if (charge < 7000) prob = 0.001;  //ToT = 13
          }
          double rnd = gRandom->Uniform(0.0,1.0);
          if (rnd<prob) bunch = bunch + 1; 

          //fill in the histogram
          if(bunch<3&&ToT<14&&ToT>5&&bec==2&&layerID==2/*&&abs(moduleID)==0*/){
              hbunch->Fill(bunch+1,ToT);
          }
	}

	//get content of histgram
	double z[hbunch->GetNbinsX()+1][hbunch->GetNbinsY()+1];
	double prob[hbunch->GetNbinsY()+1];
	double ep[hbunch->GetNbinsY()+1];
	for (int i=1; i<hbunch->GetNbinsX()+1; i++){  
		for (int j=0; j<hbunch->GetNbinsY()+1; j++){
			z[i][j] = hbunch->GetBinContent(hbunch->GetBin(i,j));
		}
	}

	//get the prob
	double e1=0.;
	double e2=0.;
	double n1=0.;
	double n2=0.;
	for (int j=0; j<hbunch->GetNbinsY()+1; j++){
		if(z[2][j]+z[3][j] == 0) continue;
		n1 = z[2][j];
		n2 = z[3][j];
		prob[j] = n2/(n1+n2);
		//calculate error of prob
		e1 = pow(n1,0.5);
		e2 = pow(n2,0.5);
		ep[j] = sqrt(pow(n2*e1,2)+pow(n1*e2,2))/pow((n1+n2),2);

		cout <<  j << " " << prob[j] <<  " " << ep[j] << endl; 
		fout <<  j << " " << prob[j] <<  " " << ep[j] << endl; 
	}

    //compare the prob with the answer
    /*
	for (int j=0; j<hbunch->GetNbinsY()+1; j++){
        double ratio = 0.;
		if(prob[j] == 0.0) continue;
        ratio = prob[j]/answer[j];
		cout << "for ToT" << j << " ratio is =  " << ratio  << endl; 
    }
    */

    fout.close();

	cout << "----- finished  bunchvsToT.cxx -----" << endl;
	return 0;
}

