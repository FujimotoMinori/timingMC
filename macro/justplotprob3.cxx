/*
   compare three files and just plot prob
 */
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2D.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TString.h>

int justplotprob3(){
  //set open file
  TString ifn = "../text/data2018blayer.txt";
  TString ifn2 = "../text/tuned2018blayer.txt";
  TString ifn3 = "../text/MC2015blayer.txt";

  std::cout << "#inputFile=" <<ifn2 << std::endl;
  ifstream fin;
  ifstream fin2;
  ifstream fin3;
  std::string str;
  std::string str2;
  std::string str3;

  //open files
  fin.open(ifn);
  if(fin.fail()){
    cerr << "#cannot open file1 : " << ifn << std::endl;
    return 1;
  }
  fin2.open(ifn2);
  if(fin2.fail()){
    cerr << "#cannot open file2 : " << ifn2 << std::endl;
    return 1;
  }
  fin3.open(ifn3);
  if(fin3.fail()){
    cerr << "#cannot open file3 : " << ifn3 << std::endl;
    //return 1;
  }
  std::cout << "#finished opening files" << std::endl;

  //read file
  int a;
  float b,c;
  float val1[20] = {};
  float err1[20] = {};
  while(getline(fin,str))
  {
    if(str[0] == '#') continue;
    a = 0;
    b = 0;
    c = 0;
    sscanf(str.data(), "%d %f %f", &a, &b, &c);
    val1[a] = b;
    err1[a] = c;
  }

  int a2;
  float b2,c2;
  float val2[20] = {};
  float err2[20] = {};
  while(getline(fin2,str2))
  {
    if(str2[0] == '#') continue;
    a2 = 0;
    b2 = 0;
    c2 = 0;
    sscanf(str2.data(), "%d %f %f", &a2, &b2, &c2);
    val2[a2] = b2;
    err2[a2] = c2;
  }

  int a3;
  float b3,c3;
  float val3[20] = {};
  float err3[20] = {};
  while(getline(fin3,str3))
  {
    if(str3[0] == '#') continue;
    a3 = 0;
    b3 = 0;
    c3 = 0;
    sscanf(str3.data(), "%d %f %f", &a3, &b3, &c3);
    val3[a3] = b3;
    err3[a3] = c3;
  }

  const Int_t n = 20;
  Double_t x[n] = {} ,y[n] = {};
  Double_t x2[n] = {} ,y2[n] = {};
  Double_t x3[n] = {} ,y3[n] = {};
  Double_t xe[n] = {} ,ye[n] = {};
  Double_t xe2[n] = {} ,ye2[n] = {};
  Double_t xe3[n] = {} ,ye3[n] = {};
  int i = 0;
  for(i=4;i<20;i++){
    x[i] = i;
    x2[i] = i;
    x3[i] = i;
    xe[i] = 0;
    xe2[i] = 0;
    xe3[i] = 0;
    y[i] = val1[i];
    ye[i] = err1[i];
    y2[i] = val2[i];
    ye2[i] = err2[i];
    y3[i] = val3[i];
    ye3[i] = err3[i];
  }

  Double_t xlo = 3.;    // x の下限
  Double_t xhi = 20.;   // x の上限
  Double_t ylo = 0.;    // y の下限
  Double_t yhi = 0.07;   // y の上限
  TCanvas *c1 = new TCanvas("c1","My Canvas",10,10,800,600);
  TH1F *frame = gPad->DrawFrame(xlo,ylo,xhi,yhi);

  TGraphErrors *gr = new TGraphErrors(n-4,x+4,y+4,xe+4,ye+4);
  TGraphErrors *gr2 = new TGraphErrors(n-4,x2+4,y2+4,xe2+4,ye2+4);
  TGraphErrors *gr3 = new TGraphErrors(n-6,x3+6,y3+6,xe3+6,ye3+6);
  gr->SetMaximum(0.7);
  gr->SetMinimum(0.);
  gr->SetTitle("timewalk probability;ToT;prob");
  gr->SetMarkerColor(2);
  gr->SetMarkerStyle(20);
  gr->SetMarkerSize(1.2);
  gr->Draw("P");
  gr2->SetMarkerColor(4);
  gr2->SetMarkerStyle(20);
  gr2->SetMarkerSize(1.2);
  gr3->SetMarkerColor(1);
  gr3->SetMarkerStyle(20);
  gr3->SetMarkerSize(1.2);
  gr3->Draw("P");
  gr2->Draw("P");
  frame->SetTitle("timewalk probability;ToT;prob");
  frame->GetYaxis()->SetTitleOffset(1.2);

  TLegend *leg = new TLegend(0.7,0.7,0.9,0.9);
  leg->AddEntry(gr,"data (2018) ","lep");
  leg->AddEntry(gr3,"default MC","lep");
  leg->AddEntry(gr2,"new MC","lep");
  leg->SetBorderSize(0); 
  leg->SetFillStyle(0); 
  leg->Draw();

  return 0;

}


