#include "PatternScanContext.h"

namespace asmkraken::mem {
    PatternScanContext::PatternScanContext() :
            detectedRules(),
            undetectedRules() {

    }

    void PatternScanContext::AddDetected(const asmkraken::mem::PatternScanRule &scanRule) {
        detectedRules.push_back(scanRule);
    }

    void PatternScanContext::AddUndetected(const PatternScanRule &scanRule) {
        undetectedRules.push_back(scanRule);
    }

    const vector<PatternScanRule> &PatternScanContext::GetDetectedRules() const {
        return detectedRules;
    }

    const vector<PatternScanRule> &PatternScanContext::GetUndetectedRules() const {
        return undetectedRules;
    }
}
