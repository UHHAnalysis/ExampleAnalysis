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
    auto_ptr<AndSelection> sel(new AndSelection(BSelection));
    auto_ptr<SelectionModule> gtr1b(new NBTagSelection(1));  //at least one b tag
    sel->add(gtr1b);
    // note that the object has been moved to sel
    assert(gtr1b.get()==0);
    selection_modules.push_back(sel.release());
    // also, sel has been moved to selection_modules:
    assert(sel.get()==0);
    
    // as 'sel' is empty now, re-cycle it for the next AndSelection:
    
    // no b selection:
    sel.reset(new AndSelection(NoBSelection));
    auto_ptr<SelectionModule> no_b(new NBTagSelection(0,0)); //no b tags
    sel->add(no_b);
    selection_modules.push_back(sel.release());

    // top selection:
    sel.reset(new AndSelection(TopSelection));
    auto_ptr<SelectionModule> ntopjets_gtr_1(new NTopJetSelection(1,int_infinity(),350,2.5));
    auto_ptr<SelectionModule> ntoptags_gtr_1(new NTopTagSelection(1,int_infinity()));
    sel->add(ntopjets_gtr_1);
    sel->add(ntoptags_gtr_1);
    selection_modules.push_back(sel.release());

    // 2. Set up Histograms:
    h_nocuts.reset(new ExampleHistsNew(ctx, "NoCuts"));
    h_btag.reset(new ExampleHistsNew(ctx, "BTag"));
    h_nobtag.reset(new ExampleHistsNew(ctx, "NoBTag"));
    h_topsel.reset(new ExampleHistsNew(ctx, "TopSel"));

    //3. initialise all AnalysisModules, i.e. all Selections:
    for(size_t i=0; i<selection_modules.size(); ++i){
        selection_modules[i].begin_dataset(ctx);
    }
    
    // 4. output
    // 4.a. define some more output in the event tree:
    event_intdata = 7;
    event_jetdata.set_pt(16);
    ctx.declare_event_output("event_intdata", event_intdata);
    ctx.declare_event_output("event_jetdata", event_jetdata);
    
    // 4.b. define output in an additional, non-event TTree.
    // (Note that you have to declare Tree2 as MetadataOutputTree in the xml config to make this work)
    ctx.declare_output("Tree2", "jet", tree2_jet);
    ctx.declare_output("Tree2", "double", tree2_double);
    
    // 5. additional input: read in event data written in 4. Note that this will only succeed if running
    // this cycle on an output file containing these branches, e.g. one previously produced with ExampleCycleNew ...
    read_example_data = string2bool(ctx.get_setting("readExampleData"));
    if(read_example_data){
        ctx.declare_event_input("event_intdata", event_intdata_in);
        ctx.declare_event_input("event_jetdata", event_jetdata_in);
    }
}


void ExampleCycleNew::process(EventCalc & event, Context & ctx) {
    // this is the most important part: here the full analysis happens
    // user should implement selections, filling of histograms and results
    
    // 1. determine the outcome of the selections by running the selection modules.
    // The AndSelection modules write the outcome of the selection cut to the event, so
    // run this before testing which selection was passed.
    for(size_t i=0; i<selection_modules.size(); ++i){
        selection_modules[i].process(event, ctx);
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
    
    // 3. firther procerssing, e.g. write something to stdout for debugging:
    if(read_example_data){
        cout << "Example data read: int = " << event_intdata_in << "; jet pt = " << event_jetdata_in.pt() << endl;
    }
    
    // Output:
    // a. event tree output:
    // Which events are written to the event output tree is controlled by the OutputSelection setting in the xml file.
    ++event_intdata;
    event_jetdata.set_pt(event_jetdata.pt() + 1);
    
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
