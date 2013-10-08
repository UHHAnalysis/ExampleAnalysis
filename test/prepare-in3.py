#!/usr/bin/env python

execfile("common.py")

ROOT.gROOT.ProcessLine("Jet jetdata;")

for ifile, fname in enumerate(("in3a.root", "in3b.root")):
    f = ROOT.TFile(fname, "recreate")
    f.cd()
    tree = ROOT.TTree("TestEventTree", "test tree")
    tree.SetDirectory(f)
    i = n.zeros(1, dtype=int)
    tree.Branch("jetdata", "Jet", ROOT.jetdata)
    tree.Branch("intdata", i, "intdata/i")
    for ievent in range(100):
        i[0] = ievent + ifile * 1000
        ROOT.jetdata.set_pt(42 + ievent + ifile * 1000)
        tree.Fill()
    tree.Write()
    f.Close()
