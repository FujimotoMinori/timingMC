/*
   prob.C
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

int prob(){
  cout << "----- start prob.cxx -----" << endl;

  //string finname = "../data/TimingChargePix.root"; //data2017
  string finname = "../data/TimingChargePix.root"; //data2017
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

  //make histogram
  TH2F *h2 = new TH2F("h2","bunchVSToT;L1A;ToT",3,-0.5,2.5,20,1,20);
  const int nParam=7;
  //const int nParam=50;
  TH2F *eta[nParam];
  //TH2F *phi[nParam];
  for ( int i=0;i<nParam;i++ ){
    eta[i]=new TH2F(Form("eta%d",i+1),"",3,-0.5,2.5,20,1,20);
    //phi[i]=new TH2F(Form("phi%d",i+1),"",3,-0.5,2.5,20,1,20);
  }

  TFile* fout = TFile::Open(foutname.c_str(), "RECREATE");
	std::ofstream summary("summarydataL2.txt");

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
    if(L1A<3&&ToT<20&&bec==0&&layer==3/*&&abs(eta_index)==0*/){
      h2->Fill(L1A,ToT);
    }
    for(int e=0;e<nParam;e++){
      if(L1A<3&&ToT<20&&bec==0&&layer==3&&abs(eta_index)==e){
      //if(L1A<3&&ToT<20&&bec==0&&layer==1&&abs(phi_index)==e){
        eta[e]->Fill(L1A,ToT);
        //phi[e]->Fill(L1A,ToT);
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
        //z2[e][i][j] = phi[e]->GetBinContent(phi[e]->GetBin(i,j));
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

  std::cout << "----------------------------------- # of ToT= " << h2->GetNbinsY() << std::endl;
  for (int j=0; j<h2->GetNbinsY()+1; j++){
    if((z[2][j]+z[3][j]) == 0) continue;
		no1=z[2][j];
		no2=z[3][j];
		prob=no2/(no1+no2);
		//error
		eo1 = pow(no1,0.5);
		eo2 = pow(no2,0.5);
		eop = sqrt(pow(no2*eo1,2)+pow(no1*eo2,2))/pow((no1+no2),2);

    for(int e=0;e<nParam;e++){
      if((z2[e][2][j]+z[3][j]) == 0) continue;
      n1[e]=z2[e][2][j];
      n2[e]=z2[e][3][j];
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
    } //loop for eta module
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

  //TGraph *g[nParam];
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
    //leg->AddEntry(g[i],Form("phi%d",i),"p");
  }

  mg->SetTitle(";ToT;prob");
  mg->Draw("AP");

  leg->Draw();

  fout->Write();
  fout->Close();
	summary.close();

  cout << "----- finished  prob.cxx -----" << endl;

  return 0;

}


