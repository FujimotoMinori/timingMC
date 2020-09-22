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

int comparelast(){
	std::cout << "#-----start checkflipbitFE.cxx-----" << std::endl;

	//set open file
	//TString ifn = "../text/data2018L2.txt";
	//TString ifn2 = "../text/tunedL2.txt";
	//TString ifn3 = "../text/MC2015L2.txt";
	TString ifn = "../text/data2018ECCL1.txt";
	TString ifn2 = "../text/MC2018ECCL1.txt";
	TString ifn3 = "../text/MCdefaultECCL1.txt";

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
	float val = 0.;
	float err = 0.;
	float ratioerr = 0.;
	float ratio = 0.;
	const Int_t n = 20;
    Double_t x[n] = {} ,y[n] = {};
    Double_t xe[n] = {} ,ye[n] = {};
    int i = 0;
	for(i=6;i<20;i++){
		//std::cout << "ToT" << i << " val from data= " << val1[i] << std::endl;
		//std::cout << "ToT" << i << " val from MC= " << val2[i] << std::endl;
		val = val2[i] - val1[i];
		err = err2[i] - err1[i];
		ratio = val2[i]/val1[i];
		ratioerr = sqrt(pow(val2[i]*err1[i],2)+pow(val1[i]*err2[i],2))/pow(val2[i],2); 
        x[i] = i;
        //y[i] = val;
        y[i] = ratio;
        xe[i] = 0;
        ye[i] = ratioerr;
		//std::cout << "val in ToT " << i << " = " << val << std::endl;
		//std::cout << "err in ToT " << i << " = " << err << std::endl;
		//std::cout << "ratio in ToT " << x[i] << " = " << ratio << std::endl;
		//std::cout << "ratioerror in ToT " << x[i] << " = " << ratioerr << std::endl;
	}


	float vald = 0.;
	float errd = 0.;
	float ratioerrd = 0.;
	float ratiod = 0.;
	const Int_t nd = 20;
    Double_t xd[nd] = {} ,yd[nd] = {};
    Double_t xed[nd] = {} ,yed[nd] = {};
    int id = 0;
	for(id=6;id<20;id++){
		std::cout << "ToT" << id << " val from data= " << val1[id] << std::endl;
		std::cout << "ToT" << id << " val from MC= " << val3[id] << std::endl;
		vald = val3[id] - val1[id];
		errd = err3[id] - err1[id];
		ratiod = val3[id]/val1[id];
		ratioerrd = sqrt(pow(val3[id]*err1[id],2)+pow(val1[id]*err3[id],2))/pow(val3[id],2); 
        xd[id] = id;
        //y[i] = val;
        yd[id] = ratiod;
        xed[id] = 0;
        yed[id] = ratioerrd;
		std::cout << "val in ToT " << id << " = " << vald << std::endl;
		std::cout << "err in ToT " << id << " = " << errd << std::endl;
		std::cout << "ratio in ToT " << xd[id] << " = " << ratiod << std::endl;
		std::cout << "ratioerror in ToT " << xd[id] << " = " << ratioerrd << std::endl;
	}

    Double_t xlo = 5.;    // x の下限
    Double_t xhi = 20.;   // x の上限
    Double_t ylo = 0.;    // y の下限
    Double_t yhi = 2.;   // y の上限
    TCanvas *c1 = new TCanvas("c1","My Canvas",10,10,800,600);
    TH1F *frame = gPad->DrawFrame(xlo,ylo,xhi,yhi);
    TGraphErrors *gr = new TGraphErrors(n-6,x+6,y+6,xe+6,ye+6);
    gr->SetMarkerColor(4);
    gr->SetMarkerStyle(20);
    gr->SetMarkerSize(1.0);
    gr->Draw("p");
    TGraphErrors *grd = new TGraphErrors(nd-6,xd+6,yd+6,xed+6,yed+6);
    grd->SetMarkerColor(1);
    grd->SetMarkerStyle(20);
    grd->SetMarkerSize(1.0);
    grd->Draw("p");
    frame->SetTitle(";ToT;prob of MC / prob of data;ToT;timewalk probability");

    TLegend *leg = new TLegend(0.7,0.7,0.9,0.9);
    leg->AddEntry(grd,"default MC","lep");
    leg->AddEntry(gr,"new MC","lep");
    leg->SetBorderSize(0); 
    leg->SetFillStyle(0); 
    leg->Draw();

    TLine *line = new TLine(5.0,1.0,20,1.0);
    line->SetLineStyle(2);
    line->Draw("p");
    //TLine *line2 = new TLine(5.0,0.70,20,0.70);
    //line2->SetLineStyle(3);
    //line2->Draw("p");

 return 0;

}

