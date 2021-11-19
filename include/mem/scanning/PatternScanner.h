#ifndef LIB_ASMKRAKEN_PATTERNSCANNER_H
#define LIB_ASMKRAKEN_PATTERNSCANNER_H

#include <string>
#include <filesystem>
#include <exception>
#include <optional>

#include <yara.h>

#include "PatternScanContext.h"

namespace asmkraken::mem {
    using std::string;
    using std::filesystem::path;
    using std::optional;

    class PatternScanner {
    public:
        PatternScanner();

        bool LoadRules(const path& rulesDir, bool recursive = true);
        bool LoadRule(const string& rule);

        [[nodiscard]] optional<PatternScanContext> Scan(const uint8_t* pStart, size_t size) const;
    private:
        bool isInitialized;
        int loadedRuleCount;
        YR_COMPILER* yaraCompiler;

        void Initialize();
    };
}


#endif //LIB_ASMKRAKEN_PATTERNSCANNER_H
