#!/usr/bin/env python

execfile("common.py")

f = ROOT.TFile("out4/AnalysisModuleRunner.MC.TTbar.root")

tree = f.Get("tree2")
assert tree is not None
assert tree.GetEntries()==200

# read in some infos and recover the 17 and 42 ...:

i = n.zeros(1, dtype=float)
tree.SetBranchAddress("d", i)

ROOT.gROOT.ProcessLine("Jet jetdata;")
tree.SetBranchAddress("j", ROOT.jetdata)

for ievent in range(200):
    tree.GetEntry(ievent)
    #print "%d: %.2f %.2f" % (ievent, i[0], ROOT.jetdata.pt())
    assert (i[0]==42 and ROOT.jetdata.pt() == 43) or (i[0]==23 and ROOT.jetdata.pt() == 24)
