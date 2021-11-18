#ifndef LIB_ASMKRAKEN_PATTERNSCANMATCH_H
#define LIB_ASMKRAKEN_PATTERNSCANMATCH_H

#include <cstdint>

namespace asmkraken::mem {
    class PatternScanMatch {
    public:
        PatternScanMatch();

        [[nodiscard]] ptrdiff_t GetOffset() const;

        void SetOffset(ptrdiff_t newOffset);

    private:
        ptrdiff_t offset;
    };
}

#endif //LIB_ASMKRAKEN_PATTERNSCANMATCH_H
