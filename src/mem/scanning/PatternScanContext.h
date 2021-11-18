#ifndef LIB_ASMKRAKEN_PATTERNSCANCONTEXT_H
#define LIB_ASMKRAKEN_PATTERNSCANCONTEXT_H

#include <vector>
#include "PatternScanRule.h"

namespace asmkraken::mem {
    using std::vector;

    class PatternScanContext {
    public:
        PatternScanContext();

        void AddDetected(const PatternScanRule &scanRule);

        void AddUndetected(const PatternScanRule &scanRule);

        [[nodiscard]] const vector<PatternScanRule> &GetDetectedRules() const;

        [[nodiscard]] const vector<PatternScanRule> &GetUndetectedRules() const;

    private:
        vector<PatternScanRule> detectedRules;
        vector<PatternScanRule> undetectedRules;
    };
}

#endif //LIB_ASMKRAKEN_PATTERNSCANCONTEXT_H
