#include "TApplication.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TFitResult.h"
#include "TH1F.h"
#include "TLegend.h"
#include "TMath.h"
#include "TMatrixD.h"
#include "TROOT.h"
#include "TRandom.h"
#include "TStyle.h"

void setStyle() {
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(1122);
  gStyle->SetOptFit(1111);
  gStyle->SetPalette(57);
  gStyle->SetOptTitle(0);
}

void analysis() {
  // getting histograms from files
  TFile* file = new TFile("./histograms/histos.root");
  TH1F* hTypes = (TH1F*)file->Get("types");
  TH1F* hTheta = (TH1F*)file->Get("theta");
  TH1F* hPhi = (TH1F*)file->Get("phi");
  TH1F* hP = (TH1F*)file->Get("p");
  TH1F* hPTrans = (TH1F*)file->Get("pTrans");
  TH1F* hEnergy = (TH1F*)file->Get("energy");
  TH1F* hInvMass = (TH1F*)file->Get("invMass");
  TH1F* hInvMassDiscordant = (TH1F*)file->Get("invMassDiscordant");
  TH1F* hInvMassConcordant = (TH1F*)file->Get("invMassConcordant");
  TH1F* hInvMassDiscordantKP = (TH1F*)file->Get("invMassDiscordantKP");
  TH1F* hInvMassConcordantKP = (TH1F*)file->Get("invMassConcordantKP");
  TH1F* hInvMassDecay = (TH1F*)file->Get("invMassDecay");

  // subtracting invariant mass distributions:
  TH1F* hDiffInvMass = new TH1F(*hInvMassDiscordant);
  hDiffInvMass->SetTitle(
      "Difference between particles with concordant and discordant charges");
  hDiffInvMass->Add(hInvMassDiscordant, hInvMassConcordant, 1., -1.);

  TH1F* hDiffInvMassKP = new TH1F(*hInvMassDiscordantKP);
  hDiffInvMassKP->SetTitle(
      "Difference between kaons and pions with concordant and discordant "
      "charges");
  hDiffInvMassKP->Add(hInvMassDiscordantKP, hInvMassConcordantKP, 1., -1.);

  // changing type histogram axis name
  hTypes->GetXaxis()->SetBinLabel(1, "pione+");
  hTypes->GetXaxis()->SetBinLabel(2, "pione-");
  hTypes->GetXaxis()->SetBinLabel(3, "kaone+");
  hTypes->GetXaxis()->SetBinLabel(4, "kaone-");
  hTypes->GetXaxis()->SetBinLabel(5, "protone+");
  hTypes->GetXaxis()->SetBinLabel(6, "protone-");
  hTypes->GetXaxis()->SetBinLabel(7, "K+");

  // getting type histogram entries:
  for (int i{}; i < 7; ++i) {
    std::cout << "numero di particelle del tipo "
              << hTypes->GetXaxis()->GetBinLabel(i + 1) << ": "
              << hTypes->GetBinContent(i + 1) << " +/- "
              << hTypes->GetBinError(i + 1) << '\n';
  }

  // defining functions for histogram fit:
  TF1* phiFit = new TF1("phiFit", "pol0", 0., 2 * TMath::Pi());
  TF1* thetaFit = new TF1("thetaFit", "pol0", 0., TMath::Pi());
  TF1* pFit = new TF1("pFit", "expo", 0., 10.);
  TF1* DiffInvMassFit = new TF1("DiffInvMassFit", "gaus", 0., 10.);
  TF1* DiffInvMassKPFit = new TF1("DiffInvMassKPFit", "gaus", 0., 10.);

  // initializing fit parameters:
  phiFit->SetParameter(0, 10e5);
  thetaFit->SetParameter(0, 10e5);
  pFit->SetParameter(0, 1.);
  pFit->SetParameter(1, 1.);
  DiffInvMassFit->SetParameter(0, 10.);
  DiffInvMassFit->SetParameter(1, 1.);
  DiffInvMassFit->SetParameter(2, 0.1);
  DiffInvMassKPFit->SetParameter(0, 10.);
  DiffInvMassKPFit->SetParameter(1, 1.);
  DiffInvMassKPFit->SetParameter(2, 0.1);

  // histogram fitting:
  hPhi->Fit("phiFit", "Q");
  hTheta->Fit("thetaFit", "Q");
  hP->Fit("pFit", "Q");
  hDiffInvMass->Fit("DiffInvMassFit", "Q");
  hDiffInvMassKP->Fit("DiffInvMassKPFit", "Q");

  // printing fit results:
  std::cout << "fit angoli phi:\naltezza fit uniforme: "
            << phiFit->GetParameter(0) << " +/- " << phiFit->GetParError(0)
            << "\nchi^2/NDF: " << (phiFit->GetChisquare()) / (phiFit->GetNDF())
            << "\nchi^2 probability: " << phiFit->GetProb() << '\n';

  std::cout << "fit angoli theta:\naltezza fit uniforme: "
            << thetaFit->GetParameter(0) << " +/- " << thetaFit->GetParError(0)
            << "\nchi^2/NDF: "
            << (thetaFit->GetChisquare()) / (thetaFit->GetNDF())
            << "\nchi^2 probability: " << thetaFit->GetProb() << '\n';

  std::cout << "fit esponenziale quantità di moto:\nprimo parametro: "
            << pFit->GetParameter(0) << " +/- " << pFit->GetParError(0)
            << "\nsecondo parametro: " << pFit->GetParameter(1) << " +/- "
            << pFit->GetParError(1)
            << "\nchi^2/NDF: " << (pFit->GetChisquare()) / (pFit->GetNDF())
            << "\nchi^2 probability: " << pFit->GetProb() << '\n';

  std::cout << "fit gaussiana differenza degli istogrammi di massa invariante "
               "cariche discordi e concordi:\nprimo parametro: "
            << DiffInvMassFit->GetParameter(0) << " +/- "
            << DiffInvMassFit->GetParError(0)
            << "\nmedia: " << DiffInvMassFit->GetParameter(1) << " +/- "
            << DiffInvMassFit->GetParError(1)
            << "\nstd: " << DiffInvMassFit->GetParameter(2) << " +/- "
            << DiffInvMassFit->GetParError(2) << "\nchi^2/NDF: "
            << (DiffInvMassFit->GetChisquare()) / (DiffInvMassFit->GetNDF())
            << "\nchi^2 probability: " << DiffInvMassFit->GetProb() << '\n';

  std::cout
      << "fit gaussiana differenza degli istogrammi di massa invariante "
         "cariche discordi e concordi tra kaoni e pioni:\nprimo parametro: "
      << DiffInvMassKPFit->GetParameter(0) << " +/- "
      << DiffInvMassKPFit->GetParError(0)
      << "\nmedia: " << DiffInvMassKPFit->GetParameter(1) << " +/- "
      << DiffInvMassKPFit->GetParError(1)
      << "\nstd: " << DiffInvMassKPFit->GetParameter(2) << " +/- "
      << DiffInvMassKPFit->GetParError(2) << "\nchi^2/NDF: "
      << (DiffInvMassKPFit->GetChisquare()) / (DiffInvMassKPFit->GetNDF())
      << "\nchi^2 probability: " << DiffInvMassKPFit->GetProb() << '\n';

  //------------------- draw histograms -------------------
  //--- Types ---
  TCanvas* cTypes = new TCanvas("cTypes", "types canvas", 800, 800);

  hTypes->SetLineWidth(1);
  hTypes->SetLineColor(kBlack);
  hTypes->SetTitle("number of particles generated");
  hTypes->SetMarkerStyle(kFullCircle);
  hTypes->SetMarkerSize(0.8);
  hTypes->SetMarkerColor(kBlack);
  hTypes->SetFillColor(kBlue);
  hTypes->Draw("H,E");

  cTypes->Print("./histograms/canvasTypes.pdf");

  //--- Angles ---
  TCanvas* cAngles = new TCanvas("cAngles", "angle canvas", 800, 800);
  cAngles->Divide(1, 2);
  cAngles->cd(1);

  hTheta->SetLineWidth(1);
  hTheta->SetLineColor(kBlack);
  hTheta->SetTitle("theta distribution");
  hTheta->SetMarkerStyle(kFullCircle);
  hTheta->SetMarkerSize(0.8);
  hTheta->SetMarkerColor(kBlack);
  hTheta->SetFillColor(kViolet);
  hTheta->SetMinimum(0);
  hTheta->Draw("H,E");

  cAngles->cd(2);

  hPhi->SetLineWidth(1);
  hPhi->SetLineColor(kBlack);
  hPhi->SetTitle("phi distribution");
  hPhi->SetMarkerStyle(kFullCircle);
  hPhi->SetMarkerSize(0.8);
  hPhi->SetMarkerColor(kBlack);
  hPhi->SetFillColor(kViolet);
  hPhi->SetMinimum(0);
  hPhi->Draw("H,E");

  cAngles->Print("./histograms/canvasAngles.pdf");

  //--- Momentum ---
  TCanvas* cP = new TCanvas("cP", "momentum canvas", 800, 800);
  cP->Divide(1, 2);
  cP->cd(1);

  hP->SetLineWidth(1);
  hP->SetLineColor(kBlack);
  hP->SetTitle("momentum distribution");
  hP->SetMarkerStyle(kFullCircle);
  hP->SetMarkerSize(0.8);
  hP->SetMarkerColor(kBlack);
  hP->SetFillColor(kRed);
  hP->SetMinimum(0);
  hP->Draw("H,E");

  cP->cd(2);

  hPTrans->SetLineWidth(1);
  hPTrans->SetLineColor(kBlack);
  hPTrans->SetTitle("transverse momentum distribution");
  hPTrans->SetMarkerStyle(kFullCircle);
  hPTrans->SetMarkerSize(0.8);
  hPTrans->SetMarkerColor(kBlack);
  hPTrans->SetFillColor(kRed);
  hPTrans->SetMinimum(0);
  hPTrans->Draw("H,E");

  cP->Print("./histograms/canvasMomentum.pdf");

  //--- Energy ---
  TCanvas* cEnergy = new TCanvas("cEnergy", "energy canvas", 800, 800);

  hEnergy->SetLineWidth(1);
  hEnergy->SetLineColor(kBlack);
  hEnergy->SetTitle("energy distribution");
  hEnergy->SetMarkerStyle(kFullCircle);
  hEnergy->SetMarkerSize(0.8);
  hEnergy->SetMarkerColor(kBlack);
  hEnergy->SetFillColor(kOrange);
  hEnergy->SetMinimum(0);
  hEnergy->Draw("H,E");

  cEnergy->Print("./histograms/canvasEnergy.pdf");

  //--- Inv Mass ---
  TCanvas* cInvMass =
      new TCanvas("cInvMass", "invariant mass canvas", 800, 800);

  hInvMass->SetLineWidth(1);
  hInvMass->SetLineColor(kBlack);
  hInvMass->SetTitle("invariant mass distribution");
  hInvMass->SetMarkerStyle(kFullCircle);
  hInvMass->SetMarkerSize(0.8);
  hInvMass->SetMarkerColor(kBlack);
  hInvMass->SetFillColor(kTeal);
  hInvMass->SetMinimum(0);
  hInvMass->Draw("H,E");

  cInvMass->Print("./histograms/canvasInvMass.pdf");

  //--- Inv Mass for concordant discordant ---
  TCanvas* cInvMassCharges = new TCanvas(
      "cInvMassCharges",
      "invariant mass canvas for concordant/discordant charges", 800, 800);
  cInvMassCharges->Divide(2, 2);
  cInvMassCharges->cd(1);

  hInvMassDiscordant->SetLineWidth(1);
  hInvMassDiscordant->SetLineColor(kBlack);
  hInvMassDiscordant->SetTitle(
      "invariant mass distribution for discordant charges");
  hInvMassDiscordant->SetMarkerStyle(kFullCircle);
  hInvMassDiscordant->SetMarkerSize(0.8);
  hInvMassDiscordant->SetMarkerColor(kBlack);
  hInvMassDiscordant->SetFillColor(kTeal);
  hInvMassDiscordant->SetMinimum(0);
  hInvMassDiscordant->Draw("H,E");

  cInvMassCharges->cd(2);

  hInvMassConcordant->SetLineWidth(1);
  hInvMassConcordant->SetLineColor(kBlack);
  hInvMassConcordant->SetTitle(
      "invariant mass distribution for concordant charges");
  hInvMassConcordant->SetMarkerStyle(kFullCircle);
  hInvMassConcordant->SetMarkerSize(0.8);
  hInvMassConcordant->SetMarkerColor(kBlack);
  hInvMassConcordant->SetFillColor(kTeal);
  hInvMassConcordant->SetMinimum(0);
  hInvMassConcordant->Draw("H,E");

  cInvMassCharges->cd(3);
  hInvMassDiscordantKP->SetLineWidth(1);
  hInvMassDiscordantKP->SetLineColor(kBlack);
  hInvMassDiscordantKP->SetTitle(
      "invariant mass distribution for discordant charges (only kaons or "
      "pions)");
  hInvMassDiscordantKP->SetMarkerStyle(kFullCircle);
  hInvMassDiscordantKP->SetMarkerSize(0.8);
  hInvMassDiscordantKP->SetMarkerColor(kBlack);
  hInvMassDiscordantKP->SetFillColor(kTeal);
  hInvMassDiscordantKP->SetMinimum(0);
  hInvMassDiscordantKP->Draw("H,E");

  cInvMassCharges->cd(4);
  hInvMassDiscordantKP->SetLineWidth(1);
  hInvMassDiscordantKP->SetLineColor(kBlack);
  hInvMassDiscordantKP->SetTitle(
      "invariant mass distribution for discordant charges(only kaons or "
      "pions)");
  hInvMassDiscordantKP->SetMarkerStyle(kFullCircle);
  hInvMassDiscordantKP->SetMarkerSize(0.8);
  hInvMassDiscordantKP->SetMarkerColor(kBlack);
  hInvMassDiscordantKP->SetFillColor(kTeal);
  hInvMassDiscordantKP->SetMinimum(0);
  hInvMassDiscordantKP->Draw("H,E");

  cInvMassCharges->Print("./histograms/canvasChargesInvMass.pdf");

  //--- Inv Mass for decayed ---
  TCanvas* cInvMassDecayed = new TCanvas(
      "cInvMassDecayed",
      "invariant mass canvas for particles generated by decay", 800, 800);

  hInvMassDecay->SetLineWidth(1);
  hInvMassDecay->SetLineColor(kBlack);
  hInvMassDecay->SetTitle(
      "invariant mass distribution for particles generated by decay");
  hInvMassDecay->SetMarkerStyle(kFullCircle);
  hInvMassDecay->SetMarkerSize(0.8);
  hInvMassDecay->SetMarkerColor(kBlack);
  hInvMassDecay->SetFillColor(kTeal);
  hInvMassDecay->SetMinimum(0);
  hInvMassDecay->Draw("H,E");

  cInvMassDecayed->Print("./histograms/canvasDecayInvMass.pdf");

  //--- Differences of Inv Mass ---
  TCanvas* cDiff =
      new TCanvas("cDiff", "differences of invariant mass canvas", 800, 800);
  cDiff->Divide(1, 2);
  cDiff->cd(1);

  hDiffInvMass->SetLineWidth(1);
  hDiffInvMass->SetLineColor(kBlack);
  hDiffInvMass->SetTitle("theta distribution");
  hDiffInvMass->SetMarkerStyle(kFullCircle);
  hDiffInvMass->SetMarkerSize(0.8);
  hDiffInvMass->SetMarkerColor(kBlack);
  hDiffInvMass->SetFillColor(kGreen);
  hDiffInvMass->SetMinimum(0);
  hDiffInvMass->Draw("H,E");

  cDiff->cd(2);

  hDiffInvMassKP->SetLineWidth(1);
  hDiffInvMassKP->SetLineColor(kBlack);
  hDiffInvMassKP->SetTitle("phi distribution");
  hDiffInvMassKP->SetMarkerStyle(kFullCircle);
  hDiffInvMassKP->SetMarkerSize(0.8);
  hDiffInvMassKP->SetMarkerColor(kBlack);
  hDiffInvMassKP->SetFillColor(kGreen);
  hDiffInvMassKP->SetMinimum(0);
  hDiffInvMassKP->Draw("H,E");

  cDiff->Print("./histograms/canvasAngles.pdf");
}