/*
   compare two files
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

int comparediff(){
  std::cout << "#-----start checkflipbitFE.cxx-----" << std::endl;

  //set open file
  TString ifn = "../text/datanewL1.txt";
  TString ifn2 = "../text/tunedL1.txt";

  //TString name;
  //name.Form("canv.pdf");
  //TCanvas* canv = new TCanvas(name.Data(),name.Data(),1000,800);
  //canv->Print(name + "[","pdf");

  std::cout << "#inputFile=" <<ifn2 << std::endl;
  ifstream fin;
  ifstream fin2;
  std::string str;
  std::string str2;

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

  //comparediff 2 values
  float val = 0.;
  float err = 0.;
  float differr = 0.;
  float diff = 0.;
  const Int_t n = 20;
  Double_t x[n] = {} ,y[n] = {};
  Double_t xe[n] = {} ,ye[n] = {};
  int i = 0;
  for(i=6;i<20;i++){
    //std::cout << "ToT" << i << " val from data= " << val1[i] << std::endl;
    //std::cout << "ToT" << i << " val from MC= " << val2[i] << std::endl;
    val = val2[i] - val1[i];
    err = err2[i] - err1[i];
    diff = val2[i]-val1[i];
    differr = sqrt(pow(err1[i],2)+pow(err2[i],2)); 
    x[i] = i;
    //y[i] = val;
    y[i] = diff;
    xe[i] = 0;
    ye[i] = differr;
    std::cout << "val in ToT " << i << " = " << val << std::endl;
    std::cout << "err in ToT " << i << " = " << err << std::endl;
    std::cout << "diff in ToT " << x[i] << " = " << diff << std::endl;
    std::cout << "differror in ToT " << x[i] << " = " << differr << std::endl;
  }

  TGraphErrors *gr = new TGraphErrors(n-6,x+6,y+6,xe+6,ye+6);
  gr->SetMaximum(0.10);
  gr->SetMinimum(-0.10);
  gr->SetTitle("Diff of Data and MC;ToT;prob of MC - prob of data");
  gr->GetYaxis()->SetTitleOffset(1.3);
  gr->SetMarkerColor(4);
  gr->SetMarkerStyle(20);
  gr->SetMarkerSize(1.0);
  gr->Draw("AP");


  return 0;

}

