/*
	 mktree.cxx
 */

#include <iostream>
#include <sstream>
//#include <ifstream>
#include <TFile.h>
#include <TH1.h>
#include <TH2D.h>
#include <TH2F.h>
#include <TCanvas.h>
using namespace std;

int mktree(){
    cout << "start mktree.cxx" << endl;
    
    //read file
    TString ifn = "processedk.txt";
    ifstream fin;
    fin.open(ifn);
    if(fin.fail()){
          cerr << "can't open file : " << ifn << endl;
          return 1;
    }

    int val1, val2, val3, val4,val6;
    float val5;

    //make tree
    TTree *tree = new TTree("chargeMC", "chargeMC");
    tree->Branch("bec", &val1, "val1/I");
    tree->Branch("layerID", &val2, "val2/I");
    tree->Branch("moduleID", &val3, "val3/I");
    tree->Branch("bunch", &val4, "val4/I");
    tree->Branch("charge", &val5, "val5");
    tree->Branch("ToT", &val6, "val6/I");

    //fill tree
    while (fin >> val1 >> val2 >> val3 >> val4 >> val5 >> val6) {
        tree->Fill();  
    cout << "val5= " << val5 << endl;
    }
    cout << "finished filling" << endl;

    //make output file
    TString ofn = "outputk.root";
    TFile *fout = new TFile(ofn, "recreate");
    tree->Write();  
    fout->Close();  

    cout << "finished mktree.cxx" << endl;
    return 0;

}


