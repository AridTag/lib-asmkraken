#include "../../../include/mem/scanning/PatternScanRule.h"

namespace asmkraken::mem {
    PatternScanRule::PatternScanRule() = default;

    const std::string &PatternScanRule::GetName() const {
        return name;
    }

    void PatternScanRule::SetName(const string &ruleName) {
        name = ruleName;
    }

    void PatternScanRule::AddMeta(const PatternScanMeta &meta) {
        metadata.push_back(meta);
    }

    void PatternScanRule::AddMatch(const PatternScanMatch &match) {
        matches.push_back(match);
    }

    const vector<PatternScanMatch> &PatternScanRule::GetMatches() const {
        return matches;
    }

    const vector<PatternScanMeta>& PatternScanRule::GetMetadata() const {
        return metadata;
    }

}
