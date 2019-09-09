void test() {

  //read time diff and distance from antinuMC
  TFile *f1 = new TFile("../candidate_antinu.root", "READ");
  TTree *candidate = (TTree *)f1->Get("candidate");

  //read fitted functions from data
  TFile *f2 = new TFile("../../test_data.root", "READ");
  TF1 *funcPosdiff = (TF1 *)f2->Get("funcPosdiff");
  TF1 *funcTickdiff = (TF1 *)f2->Get("funcTickdiff");

  funcPosdiff->SetParameter(1, 1.);
  double lambda = funcTickdiff->GetParameter(1);

  TFile *fsave = new TFile("product.root", "RECREATE");
  TTree product("product", "product");

  TH1D * hProduct_antinu = new TH1D("hProduct_antinu", "Ptime*Pdist", 120, -12, 0.);

  double posdiff;
  ULong64_t tickdiff;
  double Pdist, Ptime, Pproduct;

  product.Branch("Pdist", &Pdist, "Pdist/D");
  product.Branch("Ptime", &Ptime, "Ptime/D");
  product.Branch("Pproduct", &Pproduct, "Pproduct/D");
  product.Branch("posdiff", &posdiff, "posdiff/D");
  product.Branch("tickdiff", &tickdiff, "tickdiff/l");

  candidate->SetBranchAddress("posdiff_4", &posdiff);
  candidate->SetBranchAddress("tickdiff50_4", &tickdiff);

  for(int iEv=0; iEv<candidate->GetEntries(); iEv++) {
    candidate->GetEvent(iEv);

    if (tickdiff<50000) {
      Pdist = funcPosdiff->Integral(0., posdiff);
      Ptime = 1 - TMath::Exp(lambda*(double)tickdiff);
      Pproduct = Pdist * Ptime;
      hProduct_antinu->Fill(TMath::Log10(Pproduct));
      product.Fill();}
  }
  
  hProduct_antinu->Write();
  hProduct_antinu->Draw();
  product.Write();
}
