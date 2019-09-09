#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif

#include "string"
#include "iostream"
#include "fstream"
#include "TH1F.h"
#include "TH1D.h"
#include "TFile.h"
#include "TMinuit.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TF1.h"

#include "TPaveStats.h"
#include "TGraph.h"
#include <algorithm>
#include "TFile.h"
#include "TLatex.h"
#include "TVirtualFitter.h"
#include "iostream"
#include "sstream"
#include "TSystem.h"
#include "TRandom.h"


#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooGaussian.h"
#include "RooGaussModel.h"
#include "RooPoisson.h"
#include "RooPlot.h"
#include "TH1.h"
#include "TTree.h"
#include "RooFFTConvPdf.h"
#include "RooNumConvPdf.h"
#include "RooGlobalFunc.h"
#include "RooFitResult.h"
#include "RooArgSet.h"
#include "RooFormulaVar.h"
#include "RooAddPdf.h"

#include "RooPolynomial.h"
#include "RooAddPdf.h"
#include "RooExtendPdf.h"
#include "RooConstVar.h"
#include "RooUniform.h"
#include "RooProduct.h"


using namespace RooFit ;

void fit() {

  TFile *f1 = new TFile("./random.root", "READ");
  TH1D *hBg = (TH1D *)f1->Get("hProduct");

  TFile *f2 = new TFile("./product.root", "READ");
  TH1D *hSig = (TH1D *)f2->Get("hProduct_antinu");

  TFile * f3 = new TFile("../../test_data.root", "READ");
  TH1D *hData = (TH1D *)f3->Get("hProduct");

  RooRealVar product("product","product",-12,0);
  product.setBins(120);
  
  RooDataHist RoohBg("RoohBg", "RoobBg", product, hBg);
  RooDataHist RoohSig("RoohSig", "RoobSig", product, hSig);
  RooDataHist RoohData("RoohData", "RoobData", product, hData);

  RooHistPdf RoopdfBg("Roopdfbg", "pdf of Bg", product, RoohBg);
  //RooGenericPdf RoopdfBg("", "", product, PoohBg);
  RooHistPdf RoopdfSig("Roopdfsig", "pdf of Signal", product, RoohSig);

  RooRealVar signum("NumSig", "Ratio of signal/total number of events", 1-1./hData->Integral(), 0, hData->Integral());
//  RooRealVar signum("NumSig", "Number of Signals", 1-1./hData->Integral(), 0, hData->Integral());
//  RooRealVar bgnum("NumBg", "Number of Backgrounds", hData->Integral()-1, 0, hData->Integral());

  RooAddPdf model("model","Sig+BG Fit",RooArgList(RoopdfSig,RoopdfBg), RooArgList(signum));

  RooFitResult* fitresult = model.fitTo(RoohData, Save(kTRUE), Extended(kTRUE), SumW2Error(kTRUE),Minimizer("Minuit"),Minos(kTRUE),PrintLevel(0),Verbose(kFALSE));
//  fitresult->Print("v"); 
//  signum.Print();

//  cout << " minNll: " << fitresult->minNll() << endl;

  RooAbsReal *nll = model.createNLL(RoohData, NumCPU(2));
  RooMinimizer(*nll).migrad();
  
  RooPlot *ratioplot = signum.frame(Bins(100), Range(0,5e-5));
  nll->plotOn(ratioplot, ShiftToZero());
  //nll->plotOn(ratioplot);
  ratioplot->Draw();

/*
  RooPlot *figure = product.frame();
  RoohData.plotOn(figure);
  model.plotOn(figure);
  model.plotOn(figure, Components(RoopdfSig), LineStyle(kDashed));
  figure->Draw();
  figure->GetYaxis()->SetRangeUser(1e-2, 1e4);
  gPad->SetLogy();
  figure->Draw();
*/
}


