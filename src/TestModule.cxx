#include <iostream>
#include "include/TestModule.h"

#include <boost/lexical_cast.hpp>

using namespace std;

namespace{
    void test_settings(const Context & cfg){
        // getting some existing setting:
        string value1 = cfg.get_setting("setting1");
        assert(value1=="value1");
        assert(cfg.has_setting("setting1"));
        
        // exceptions for non-existing settings:
        assert(!cfg.has_setting("nonexisting_setting"));
        bool exception = false;
        try{
            cfg.get_setting("nonexisting_setting");
        }
        catch(...){
            exception = true;
        }
        assert(exception);
        
        // default value mechanism:
        string default_value = cfg.get_setting("nonexisting_setting", "default-value");
        assert(default_value == "default-value");
        
        // get all settings:
        map<string, string> settings = cfg.get_all_settings();
        assert(settings.find("setting1")->second == "value1");
        assert(settings.find("nonexisting_setting") == settings.end());
        
        // sframe dataset info: type and version:
        string sframe_dataset_type = cfg.get_setting("dataset.type");
        string sframe_dataset_version = cfg.get_setting("dataset.version");
        assert(sframe_dataset_type == "MC");
        assert(sframe_dataset_version == "TTbar");        
    }
}

void TestModule::begin_dataset(Context & ctx){
    test = boost::lexical_cast<int>(ctx.get_setting("test"));
    
    if(test==TEST_SETTINGS){
        test_settings(ctx);
    }
    else if(test==TEST_OUT_H){
        h1 = new TH1D("h1", "h1", 100, 0, 100);
        h2 = new TH1D("h2", "h2", 100, 0, 100);
        ctx.put("h1", h1);
        ctx.put("dir1/subdir1/h2", h2);
        ievent = 0;
    }
    else if(test==TEST_OUT_EVENT){
        ctx.declare_event_output("intdata", event_intdata);
        ctx.declare_event_output("jetdata", event_jetdata);
    }
    else if(test==TEST_IN_EVENT){
        ctx.declare_event_input("intdata", event_intdata);
        ctx.declare_event_input("jetdata", event_jetdata);
        h1 = new TH1D("intdata", "intdata", 2000, 0, 2000);
        h2 = new TH1D("jetdata", "jetdata", 2000, 0, 2000);
        ctx.put("intdata", h1);
        ctx.put("jetdata", h2);
    }
    else if(test==TEST_OUT_TREE){
        ctx.declare_output("tree2", "d",  tree2_double);
        ctx.declare_output("tree2", "j", tree2_jet);
    }
    else{
        throw runtime_error("unknown test");
    }
        
}


void TestModule::process(EventCalc & event, Context & ctx) {
    // nothing to do for TEST_SETTINGS ...
    if(test==TEST_OUT_H){
        h1->Fill(ievent);
        h2->Fill(ievent);
        ++ievent;
    }
    else if(test==TEST_OUT_EVENT){
        // check that we could read in some info:
        assert(event.GetJets()!=0);
        assert(event.GetMuons()!=0);
        assert(event.GetElectrons()!=0);
        assert(event.GetTaus()!=0);
        assert(event.GetMET()!=0);
        assert(event.GetGenParticles()!=0);
        assert(event.GetPrimaryVertices()!=0);
        assert(event.GetGenJets()!=0);
        
        event_intdata = 17;
        event_jetdata.set_pt(42);
    }
    else if(test==TEST_IN_EVENT){
        h1->Fill(event_intdata);
        h2->Fill(event_jetdata.pt());
    }
    else if(test==TEST_OUT_TREE){
        // always fill in two doubles / jets per event:
        tree2_double = 42.0;
        tree2_jet.set_pt(43.0);
        ctx.write_output("tree2");
        tree2_double = 23.0;
        tree2_jet.set_pt(24.0);
        ctx.write_output("tree2");
    }
}

REGISTER_ANALYSIS_MODULE(TestModule)
