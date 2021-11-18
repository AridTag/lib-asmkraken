#include "../../../include/mem/scanning/PatternScanMeta.h"

namespace asmkraken::mem {
    PatternScanMeta::PatternScanMeta() :
            id(),
            type(PatternScanMetaType::Int),
            stringValue(),
            intValue(0) {

    }

    const string &PatternScanMeta::GetId() const {
        return id;
    }

    void PatternScanMeta::SetId(const string &metaId) {
        id = metaId;
    }

    PatternScanMetaType PatternScanMeta::GetType() const {
        return type;
    }

    void PatternScanMeta::SetType(PatternScanMetaType metaType) {
        type = metaType;
    }

    const string &PatternScanMeta::GetStringValue() const {
        return stringValue;
    }

    void PatternScanMeta::SetStringValue(const string &metaValue) {
        stringValue = metaValue;
    }

    int64_t PatternScanMeta::GetIntValue() const {
        return intValue;
    }

    void PatternScanMeta::SetIntValue(int64_t metaValue) {
        intValue = metaValue;
    }
}
