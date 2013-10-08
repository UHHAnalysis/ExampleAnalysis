#!/usr/bin/env python

import ROOT

f = ROOT.TFile("out1/AnalysisModuleRunner.MC.TTbar.root")
h1 = f.Get("h1")
assert h1 is not None
assert h1.GetNbinsX()==100
assert h1.GetEntries()==100


h2 = f.Get("dir1/subdir1/h2")
assert h2 is not None
assert h2.GetNbinsX()==100
assert h2.GetEntries()==100

