#if !defined(TestModule_H) && !defined(__CINT__)
#define TestModule_H

#include "SFrameTools/include/AnalysisModule.h"

#include <memory>

enum tests{
    TEST_SETTINGS = 0, TEST_OUT_H, TEST_OUT_EVENT, TEST_IN_EVENT, TEST_OUT_TREE
};

/** @short Test for the AnalysisModule
 * 
 * Tests different aspects of the AnalysisModule mechnisms. Which test is
 * run is set via the "test" setting in the xml file.
 *  0. reading settings from the xml config (TEST_SETTINGS)
 *  1. storing output histograms (TEST_OUT_H)
 *  2. creating additional per-event data, both objects and "trivial" data types (TEST_OUT_EVENT)
 *  3. reading in per-event data, both objects and "trivial" data types (TEST_IN_EVENT). Use two input files to test that multiple input works.
 *  4. creating an additional output tree (TEST_OUT_TREE)
 * 
 * All tests are run 3 times: in local mode, in proof-lite mode and in full proof mode. These passes
 * are done subsequently: first, all tests are run in local mode, then all are run in lite mode. Finally, they run in
 * proof mode.
 * 
 * The test script ensures that the output is in the "out0", "out1", etc. sub-directories, relative to the "test" directory.
 * 
 * To check the prroduced root files, the python script test-rootfile.py is used which tests the trees and histograms
 * for their expected contents.
 */
class TestModule: public AnalysisModule {
public:
    virtual void begin_dataset(Context & ctx);
    virtual void process(EventCalc & event, Context & ctx);

private:
    int test; // test number
    
    // for TEST_OUT_H:
    TH1D * h1, *h2;
    int ievent;
    
    // for TEST_OUT_EVENT and TEST_IN_EVENT:
    int event_intdata;
    Jet event_jetdata;
    
    // additional TTrees in the output for TEST_OUT_TREE:
    double tree2_double;
    Jet tree2_jet;
};

#endif
