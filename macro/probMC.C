/*
   probMC.C
   make tree of data and make graph of ToT and probability
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

int probMC(){
  cout << "----- start prob.cxx -----" << endl;

  string finname = "/Users/fujimoto/Desktop/data/outputMC0424_2.root"; //MC2017
  //string finname = "../data/outputMC2017newcharge.root"; //MC2015
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

  //make histogram
  TH2F *h2 = new TH2F("h2","bunchVSToT;bunch;ToT",3,-0.5,2.5,20,1,20);
  const int nParam=7;
  TH2F *eta[nParam];
  for ( int i=0;i<nParam;i++ ){
    eta[i]=new TH2F(Form("eta%d",i+1),"",3,-0.5,2.5,20,1,20);
  }

  //TFile* fout = TFile::Open(foutname.c_str(), "RECREATE");

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
    if(bunch<3&&ToT<20&&ToT>=4&&bec==0&&layerID==1/*&&abs(moduleID)==0*/){
      h2->Fill(bunch,ToT);
    }
    for(int e=0;e<nParam;e++){
      if(bunch<3&&ToT<20&&ToT>=4&&bec==0&&layerID==1&&abs(moduleID)==e){
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
  double prob=0.;
  double prob2[nParam]= {};
  double e1[nParam]= {};
  double e2[nParam]= {};
  double ep[nParam]= {};
  double n1[nParam]= {};
  double n2[nParam]= {};
  int n = h2->GetNbinsY();
  Double_t x[nParam][n] ;
  Double_t y[nParam][n] ;
  Double_t ye[nParam][n] ;
  Double_t xe[nParam][n] ;

  for (int j=0; j<h2->GetNbinsY()+1; j++){
    if((z[1][j]+z[2][j]) == 0) continue;
    prob = z[3][j]/(z[2][j]+z[3][j]);

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
      cout << "ToT=" << j << " moduleID=" << e << " prob=" << prob2[e] << " error=" << ye[e][j] << endl;
    }
  }
  
  //TCanvas *c1 = new TCanvas("c", "c",1000,1000);
  gStyle->SetMarkerStyle(20);
  gStyle->SetMarkerSize(1.0);

  //TGraph *g[nParam];
  TGraphErrors *g[nParam];
  for(int i=0;i<nParam;i++){
    cout << "i=" << i << endl;
    //g[i] = new TGraph(n, x[i], y[i]);
    g[i] = new TGraphErrors(n-4, x[i]+4, y[i]+4,xe[i]+4,ye[i]+4);
    g[i]->SetMarkerColor(i+1);
  }

  TMultiGraph *mg = new TMultiGraph();
  //mg->GetYaxis()->SetRangeUser(0,0.7);
  TLegend *leg = new TLegend(0.1,0.7,0.48,0.9);

  for(int i=0;i<nParam;++i){
    mg->Add(g[i]);
    leg->AddEntry(g[i],Form("eta_module%d",i),"p");
  }

  mg->SetTitle(";ToT;prob");
  mg->Draw("AP");

  leg->Draw();

  //fout->Write();
  //fout->Close();

  cout << "----- finished  probMC.cxx -----" << endl;

  return 0;

}


