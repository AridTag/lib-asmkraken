#ifndef LIB_ASMKRAKEN_PATTERNSCANNER_H
#define LIB_ASMKRAKEN_PATTERNSCANNER_H

#include <string>
#include <filesystem>
#include <exception>

#include "PatternScanContext.h"

namespace asmkraken::mem {
    using std::string;
    using std::filesystem::path;

    class PatternScanner {
    public:
        PatternScanner();

        bool LoadRules(const path& rulesDir, bool recursive = true);
        bool LoadRule(const string& rule);

        [[nodiscard]] PatternScanContext Scan() const;
    private:

    };
}


#endif //LIB_ASMKRAKEN_PATTERNSCANNER_H
