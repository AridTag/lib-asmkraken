#ifndef LIB_ASMKRAKEN_PATTERNSCANRULE_H
#define LIB_ASMKRAKEN_PATTERNSCANRULE_H

#include <string>
#include <vector>
#include "PatternScanMeta.h"
#include "PatternScanMatch.h"

namespace asmkraken::mem {
    using std::string;
    using std::vector;

    class PatternScanRule {
    public:
        PatternScanRule();

        [[nodiscard]] const string &GetName() const;
        void SetName(const string &ruleName);
        void AddMeta(const PatternScanMeta &meta);
        void AddMatch(const PatternScanMatch &match);
        [[nodiscard]] const vector<PatternScanMatch> &GetMatches() const;

    private:
        string name;
        vector<PatternScanMeta> metadata;
        vector<PatternScanMatch> matches;
    };
}

#endif //LIB_ASMKRAKEN_PATTERNSCANRULE_H
