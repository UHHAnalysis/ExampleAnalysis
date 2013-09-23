#include <iostream>

using namespace std;

#include "include/ExampleCycleNew.h"
#include "include/ExampleHistsNew.h"

#include "SFrameAnalysis/include/SelectionModules.h"
#include "SFrameAnalysis/include/HypothesisHists.h"

ID(BSelection);
ID(NoBSelection);
ID(chi2_selection);
ID(TopSelection);


void ExampleCycleNew::begin_dataset(Context & ctx){
    // 1. set up selections:
    
    // b selection:
    unique_ptr<AndSelection> sel(new AndSelection(BSelection));
    sel->add(make_unique<NBTagSelection>(1)); //at least one b tag
    selection_modules.emplace_back(move(sel));

    // no b selection:
    sel.reset(new AndSelection(NoBSelection));
    sel->add(make_unique<NBTagSelection>(0,0)); //no b tags
    selection_modules.emplace_back(move(sel));

    // top selection:
    sel.reset(new AndSelection(TopSelection));
    sel->add(make_unique<NTopJetSelection>(1,int_infinity(),350,2.5));
    sel->add(make_unique<NTopTagSelection>(1,int_infinity()));
    selection_modules.emplace_back(move(sel));

    // 2. Set up Histograms:
    h_nocuts.reset(new ExampleHistsNew(ctx, "NoCuts"));
    h_btag.reset(new ExampleHistsNew(ctx, "BTag"));
    h_nobtag.reset(new ExampleHistsNew(ctx, "NoBTag"));
    h_topsel.reset(new ExampleHistsNew(ctx, "TopSel"));

    //3. initialise all AnalysisModules, i.e. all Selections:
    for(auto & m : selection_modules){
        m->begin_dataset(ctx);
    }
    
    // 4. output
    // 4.a. define some more output in the event tree:
    ctx.declare_event_output("event_intdata", event_intdata);
    ctx.declare_event_output("event_jetdata", event_jetdata);
    
    // 4.b. define output in an additional, non-event TTree.
    // (Note that you have to declare Tree2 as MetadataOutputTree in the xml config to make this work)
    ctx.declare_output("Tree2", "jet", tree2_jet);
    ctx.declare_output("Tree2", "double", tree2_double);
}


void ExampleCycleNew::process(EventCalc & event, Context & ctx) {
    // this is the most important part: here the full analysis happens
    // user should implement selections, filling of histograms and results
    
    // 1. determine the outcome of the selections by running the selection modules.
    // The AndSelection modules write the outcome of the selection cut to the event, so
    // run this before testing which selection was passed.
    for(auto & m : selection_modules){
        m->process(event, ctx);
    }

    // 2. read the selection results and fill the corresponding histograms:
    h_nocuts->fill(event);
    if(event.selection_passed(BSelection)){
        h_btag->fill(event);
    }
    if(event.selection_passed(NoBSelection)){
        h_nobtag->fill(event);  
    }
    
    if(event.selection_passed(TopSelection)){
        h_topsel->fill(event);
    }
    
    // Output:
    // a. event tree output:
    // Which events are written to the event output tree is controlled by the OutputSelection setting in the xml file.
    event_intdata = 7;
    event_jetdata.set_pt(16);
    
    // the non-event output tree has to be filled by hand; we can fill in more than one entry per event:
    ID(Tree2);
    tree2_double = 7;
    ctx.write_output(Tree2); // note that using the identifier Tree2 instead of the string "Tree2" is just to save some time here
    tree2_double = 8;
    ctx.write_output(Tree2);
    
}

// as we want to run the ExampleCycleNew directly with AnalysisModuleRunner, we have to make sure we are found by the class name.
// This is achieved by calling this macro:
REGISTER_ANALYSIS_MODULE(ExampleCycleNew)
