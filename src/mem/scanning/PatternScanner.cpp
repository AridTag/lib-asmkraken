#include "PatternScanner.h"

namespace asmkraken::mem {

    PatternScanner::PatternScanner() {

    }

    bool PatternScanner::LoadRules(const path& rulesDir, bool recursive) {
        string rule;
        if (!LoadRule(rule))
        {
            return false;
        }

        return true;
    }

    bool PatternScanner::LoadRule(const string& rule) {
        return false;
    }

    PatternScanContext PatternScanner::Scan() const {
        PatternScanContext scanContext;

        return scanContext;
    }
}