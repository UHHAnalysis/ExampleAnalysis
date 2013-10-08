#!/usr/bin/env python

execfile("common.py")

f = ROOT.TFile("out2/AnalysisModuleRunner.MC.TTbar.root")
tree = f.Get("AnalysisTree")
assert tree is not None
assert tree.GetEntries()==100

# read in some infos and recover the 17 and 42 ...:

i = n.zeros(1, dtype=int)
tree.SetBranchAddress("intdata", i)

ROOT.gROOT.ProcessLine("Jet jetdata;")
tree.SetBranchAddress("jetdata", ROOT.jetdata)

for ievent in range(100):
    tree.GetEntry(ievent)
    assert i[0]==17
    assert ROOT.jetdata.pt() == 42
