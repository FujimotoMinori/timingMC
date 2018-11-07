/*
	 compare three files
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

int compare3(){
	std::cout << "#-----start compare3.cxx-----" << std::endl;

	//set open file
	TString ifn = "../text/dataL2.txt";
	TString ifn2 = "../text/tunedL2.txt";
	TString ifn3 = "../text/summaryMC2017ithL2.txt";

	//TString name;
	//name.Form("canv.pdf");
	//TCanvas* canv = new TCanvas(name.Data(),name.Data(),1000,800);
	//canv->Print(name + "[","pdf");

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

  //compare 2 values
	//float val = 0.;
	//float vals = 0.;
	float err = 0.;
	float errs = 0.;
	float ratioerr = 0.;
	float ratioerrs = 0.;
	float ratio = 0.;
	float ratios = 0.;
	const Int_t n = 20;
    Double_t x[n] = {} ,y[n] = {};
    Double_t xs[n] = {} ,ys[n] = {};
    Double_t xe[n] = {} ,ye[n] = {};
    Double_t xes[n] = {} ,yes[n] = {};
    int i = 0;
	for(i=6;i<20;i++){
		//val = val2[i] - val1[i];
		//vals = val3[i] - val1[i];
		//err = err2[i] - err1[i];
		//errs = err3[i] - err1[i];
		ratio = val2[i]/val1[i];
		ratios = val3[i]/val1[i];
		ratioerr = sqrt(pow(val2[i]*err1[i],2)+pow(val1[i]*err2[i],2))/pow(val2[i],2); 
		ratioerrs = sqrt(pow(val3[i]*err1[i],2)+pow(val1[i]*err3[i],2))/pow(val3[i],2); 
        x[i] = i;
        xs[i] = i;
        y[i] = ratio;
        ys[i] = ratios;
        xe[i] = 0;
        xes[i] = 0;
        ye[i] = ratioerr;
        yes[i] = ratioerrs;
	}

    TGraphErrors *gr = new TGraphErrors(n-6,x+6,y+6,xe+6,ye+6);
    TGraphErrors *gr2 = new TGraphErrors(n-6,xs+6,ys+6,xes+6,yes+6);
    gr2->SetMaximum(2.0);
    gr2->SetMinimum(0.0);
    gr2->SetTitle("Ratio of Data and MC;ToT;prob of MC / prob of data");
    gr->SetMarkerColor(4);
    gr->SetMarkerStyle(20);
    gr->SetMarkerSize(1.0);
    gr2->SetMarkerColor(1);
    gr2->SetMarkerStyle(20);
    gr2->SetMarkerSize(1.0);
    gr2->Draw("AP");
    gr->Draw("P");


 return 0;

}

