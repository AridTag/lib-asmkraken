#include "PatternScanMatch.h"

namespace asmkraken::mem {

    PatternScanMatch::PatternScanMatch() :
            offset(0) {
    }

    ptrdiff_t PatternScanMatch::GetOffset() const {
        return offset;
    }

    void PatternScanMatch::SetOffset(ptrdiff_t newOffset) {
        offset = newOffset;
    }
}