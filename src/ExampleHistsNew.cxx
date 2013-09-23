#include "include/ExampleHistsNew.h"
#include "SFrameTools/include/EventCalc.h"
#include <iostream>

using namespace std;

ExampleHistsNew::ExampleHistsNew(Context & ctx, const string & dirname): Hists(ctx, dirname){
  // book all histograms here

  // missing ET and HT
  unique_ptr<double[]> bins = log_binning(40, 50, 1200);
  book<TH1F>("MET_lx", "missing E_{T} [GeV]", 40, bins.get());
  
  bins = log_binning(40, 150, 3000);
  book<TH1F>("HT_lx", "H_{T} [GeV]", 40, bins.get());
  
  bins = log_binning(40, 150, 1200);
  book<TH1F>( "HTlep_lx", "H_{T}^{lep} [GeV]", 40, bins.get());

  // jets
  book<TH1F>( "N_jets_ly", "N^{jets}", 20, 0, 20 );
  
  bins = log_binning(40, 150, 1500);
  book<TH1F>( "pt_jet1_lx", "p_{T}^{jet 1} [GeV/c]", 40, bins.get());
  bins = log_binning(40, 50, 1000);
  book<TH1F>( "pt_jet2_lx", "p_{T}^{jet 2} [GeV/c]", 40, bins.get()); 
  bins = log_binning(40, 50, 500);
  book<TH1F>( "pt_jet3_lx", "p_{T}^{jet 3} [GeV/c]", 40, bins.get());
  bins = log_binning(40, 50, 250);
  book<TH1F>( "pt_jet4_lx", "p_{T}^{jet 4} [GeV/c]", 40, bins.get());
  
  
  book<TH1F>( "eta_jet1", "#eta^{jet 1}", 40, -2.5, 2.5);
  book<TH1F>( "eta_jet2", "#eta^{jet 2}", 40, -2.5, 2.5);
  book<TH1F>( "eta_jet3", "#eta^{jet 3}", 40, -2.5, 2.5);
  book<TH1F>( "eta_jet4", "#eta^{jet 4}", 40, -2.5, 2.5);

  // leptons
  book<TH1F>( "N_mu", "N^{#mu}", 10, 0, 10 );
  
  bins = log_binning(40, 45, 500);
  book<TH1F>( "pt_mu_lx", "p_{T}^{#mu} [GeV/c]", 40, bins.get());
  
  book<TH1F>( "eta_mu", "#eta^{#mu}", 40, -2.1, 2.1);
  book<TH1F>( "reliso_mu", "#mu rel. Iso", 40, 0, 0.5);


  // primary vertices
  book<TH1F>( "N_pv", "N^{PV}", 50, 0, 50 );
  book<TH1F>( "N_events_perLumiBin", "N^{evt}", 24, 0, 24 );
  book<TH1F>( "N_pv_perLumiBin", "N^{PV}", 24, 0, 24 );
}


void ExampleHistsNew::fill(EventCalc & event){
  // fill the histograms. Don't forget to always use the weight:
  double weight = event.GetWeight();
  
  int run = event.GetRunNum();
  int lumiblock = event.GetLumiBlock();
  int Npvs = event.GetPrimaryVertices()->size();
  get_hist("N_pv")->Fill(Npvs, weight);
  
  if(event.IsRealData()){
    LuminosityHandler* lumih = event.GetLumiHandler();
    if(lumih){
        get_hist( "N_pv_perLumiBin")->Fill( lumih->GetLumiBin(run, lumiblock), Npvs*weight);
        get_hist( "N_events_perLumiBin")->Fill( lumih->GetLumiBin(run, lumiblock), weight);
    }
  }

  MET* met = event.GetMET();
  get_hist("MET_lx")->Fill(met->pt(), weight);

  double HT = event.GetHT();
  get_hist("HT_lx")->Fill(HT, weight);

  double HTlep = event.GetHTlep();
  get_hist("HTlep_lx")->Fill(HTlep, weight);

  std::vector<Jet>* jets = event.GetJets();
  int Njets = jets->size();
  get_hist("N_jets_ly")->Fill(Njets, weight);

  if(Njets>=1){
    get_hist("pt_jet1_lx")->Fill(jets->at(0).pt(), weight);
    get_hist("eta_jet1")->Fill(jets->at(0).eta(), weight);
  }
  if(Njets>=2){
    get_hist("pt_jet2_lx")->Fill(jets->at(1).pt(), weight);
    get_hist("eta_jet2")->Fill(jets->at(1).eta(), weight);
  }
  if(Njets>=3){
    get_hist("pt_jet3_lx")->Fill(jets->at(2).pt(), weight);
    get_hist("eta_jet3")->Fill(jets->at(2).eta(), weight);
  }
  if(Njets>=4){
    get_hist("pt_jet4_lx")->Fill(jets->at(3).pt(), weight);
    get_hist("eta_jet4")->Fill(jets->at(3).eta(), weight);
  }

  std::vector<Muon>* muons = event.GetMuons();
  int Nmuons = muons->size();
  get_hist("N_mu")->Fill(Nmuons, weight);
  for (int i=0; i<Nmuons; ++i){
    Muon thismu = muons->at(i);
    get_hist("pt_mu_lx")->Fill(thismu.pt(), weight);
    get_hist("eta_mu")->Fill(thismu.eta(), weight);
    get_hist("reliso_mu")->Fill(thismu.relIso(), weight);
  }
}

