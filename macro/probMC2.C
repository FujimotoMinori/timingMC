/*
   probMC2.C
   make tree of data and make graph of ToT and probability
	 try fitting 
 */

#include <iostream>
#include <sstream>
#include <TFile.h>
#include <TH1.h>
#include <TH2D.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TString.h>
using namespace std;

int probMC2(){
  cout << "----- start probMC2.cxx -----" << endl;

  //string finname = "../data/outputMC2017tuned10.root"; //MC2017
  string finname = "/Users/fujimoto/Desktop/data/outputMC2017ith.root"; 
  string foutname = "../data/probtest.root";

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
  TH2F *h2 = new TH2F("h2","bunchVSToT;bunch;ToT",3,-0.5,2.5,20,1,20);
  const int nParam=7;
  TH2F *eta[nParam];
  for ( int i=0;i<nParam;i++ ){
    eta[i]=new TH2F(Form("eta%d",i+1),"",3,-0.5,2.5,20,1,20);
  }

  TFile* fout = TFile::Open(foutname.c_str(), "RECREATE");
  std::ofstream summary("summaryMC2017ithblayer.txt");

  Int_t bec;
  Int_t layerID;
  Int_t moduleID;
  Int_t bunch;
  Int_t charge;
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
    int tot;
    tin->GetEntry(ientry);    
    if(bunch<3&&ToT<20&&ToT>=6&&bec==0&&layerID==1/*&&abs(moduleID)==0*/){
      h2->Fill(bunch,ToT);
    }
    for(int e=0;e<nParam;e++){
      if(bunch<3&&ToT<20&&ToT>=6&&bec==0&&layerID==1&&abs(moduleID)==e){
        eta[e]->Fill(bunch,ToT);
      }
    }
  }

  //get content of histgram
  double z[h2->GetNbinsX()+1][h2->GetNbinsY()+1];
  double z2[nParam][h2->GetNbinsX()+1][h2->GetNbinsY()+1];
  for (int i=1; i<h2->GetNbinsX()+1; i++){  
    for (int j=0; j<h2->GetNbinsY()+1; j++){
      z[i][j] = h2->GetBinContent(h2->GetBin(i,j));
      for(int e=0;e<nParam;e++){
        z2[e][i][j] = eta[e]->GetBinContent(eta[e]->GetBin(i,j));
      }
    }
  }

  //get the prob
  int n = h2->GetNbinsY();
  double prob=0.;
  double eo1= {};
  double eo2= {};
  double eop= {};
  double no1= {};
  double no2= {};
  Double_t xo[n] ;
  Double_t yo[n] ;
  Double_t yeo[n] ;
  Double_t xeo[n] ;
  double prob2[nParam]= {};
  double e1[nParam]= {};
  double e2[nParam]= {};
  double ep[nParam]= {};
  double n1[nParam]= {};
  double n2[nParam]= {};
  Double_t x[nParam][n] ;
  Double_t y[nParam][n] ;
  Double_t ye[nParam][n] ;
  Double_t xe[nParam][n] ;

  for (int j=0; j<h2->GetNbinsY()+1; j++){
    if((z[1][j]+z[2][j]) == 0) continue;
		no1=z[1][j];
		no2=z[2][j];
		prob=no2/(no1+no2);
		//error
		eo1 = pow(no1,0.5);
		eo2 = pow(no2,0.5);
		eop = sqrt(pow(no2*eo1,2)+pow(no1*eo2,2))/pow((no1+no2),2);

    for(int e=0;e<nParam;e++){
      if((z2[e][1][j]+z[2][j]) == 0) continue;
      //prob2[e] = z2[e][3][j]/(z2[e][2][j]+z2[e][3][j]);
      n1[e]=z2[e][1][j];
      n2[e]=z2[e][2][j];
      if((n1[e]+n2[e]) == 0) continue;
      prob2[e]=n2[e]/(n1[e]+n2[e]);
      //calculate error of prob
      e1[e] = pow(n1[e],0.5);
      e2[e] = pow(n2[e],0.5);
      ep[e] = sqrt(pow(n2[e]*e1[e],2)+pow(n1[e]*e2[e],2))/pow((n1[e]+n2[e]),2);
      
      x[e][j]=j;
      y[e][j]=prob2[e];
      ye[e][j]=ep[e];
      xe[e][j]=0;
      //cout << "ToT=" << j << " moduleID=" << e << " prob=" << prob2[e] << " error=" << ye[e][j] << endl;
    } //loop e for eta module
      xo[j]=j;
      yo[j]=prob;
      yeo[j]=eop;
      xeo[j]=0;
      cout << "ToT=" << j <<  " prob=" << prob << " error=" << yeo[j] << endl;
      summary <<  j  << " " << prob << " " << yeo[j] << endl;
  } //loop j for ToT
  
  //TCanvas *c1 = new TCanvas("c", "c",1000,1000);
  gStyle->SetMarkerStyle(20);
  gStyle->SetMarkerSize(1.0);

  
  TGraphErrors *go = new TGraphErrors(n-6,xo+6,yo+6,xeo+6,yeo+6);
  TGraphErrors *g[nParam];
  for(int i=0;i<nParam;i++){
    cout << "i=" << i << endl;
    //g[i] = new TGraph(n, x[i], y[i]);
    g[i] = new TGraphErrors(n-6, x[i]+6, y[i]+6,xe[i]+6,ye[i]+6);
    g[i]->SetMarkerColor(i+1);
  }

  TMultiGraph *mg = new TMultiGraph();
  TLegend *leg = new TLegend(0.1,0.7,0.48,0.9);

  for(int i=0;i<nParam;++i){
    mg->Add(g[i]);
    leg->AddEntry(g[i],Form("eta_module%d",i),"p");
  }

  mg->SetTitle(";ToT;prob");
  mg->Draw("AP");

	
  go->SetTitle(";ToT;prob");
  go->Draw("AP");
  go->Write();

  leg->Draw();

  //fout->Write();
  //fout->Close();
  summary.close();

  cout << "----- finished  probMC2.cxx -----" << endl;

  return 0;

}



