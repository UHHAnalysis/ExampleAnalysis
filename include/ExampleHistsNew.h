#ifndef ExampleHistsNew_H
#define ExampleHistsNew_H

#include "SFrameTools/include/AnalysisModule.h"

/**
 *   Example class for booking and filling histograms, the new version using AnalysisModule mechanisms.
 */

class ExampleHistsNew: public Hists {
public:
    // use the same constructor arguments as Hists for forwarding:
    ExampleHistsNew(Context & ctx, const string & dirname);

    virtual void fill(EventCalc & ev);
};


#endif
