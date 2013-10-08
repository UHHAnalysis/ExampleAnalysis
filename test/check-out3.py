#!/usr/bin/env python

import ROOT

f = ROOT.TFile("out3/AnalysisModuleRunner.MC.TTbar.root")
intdata = f.Get("intdata")
assert intdata.GetNbinsX()==2000
assert intdata.GetEntries()==200

jetdata = f.Get("jetdata")
assert jetdata.GetNbinsX()==2000
assert jetdata.GetEntries()==200

# intdata should be 0..99 and 1000..1099:
# jetdata should be 42..141 and 1042..1141:
for i in range(2000):
    expect_filled = i<=99 or (i>=1000 and i<=1099)
    if expect_filled:
        assert intdata.GetBinContent(i+1)==1
    else:
        assert intdata.GetBinContent(i+1)==0
    expect_filled = (i>=42 and i<=141) or (i>=1042 and i<=1141)
    if expect_filled:
        assert jetdata.GetBinContent(i+1)==1
    else:
        assert jetdata.GetBinContent(i+1)==0
