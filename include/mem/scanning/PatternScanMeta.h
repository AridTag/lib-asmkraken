#ifndef LIB_ASMKRAKEN_PATTERNSCANMETA_H
#define LIB_ASMKRAKEN_PATTERNSCANMETA_H

#include <cstdint>
#include <string>

namespace asmkraken::mem {
    using std::string;

    enum PatternScanMetaType {
        Int,
        String
    };

    class PatternScanMeta {
    public:
        PatternScanMeta();

        [[nodiscard]] const string& GetId() const;
        void SetId(const string &metaId);

        [[nodiscard]] PatternScanMetaType GetType() const;
        void SetType(PatternScanMetaType metaType);

        [[nodiscard]] const string& GetStringValue() const;
        void SetStringValue(const string &metaValue);

        [[nodiscard]] int64_t GetIntValue() const;
        void SetIntValue(int64_t metaValue);

    private:
        string id;
        PatternScanMetaType type;
        string stringValue;
        int64_t intValue;
    };
}

#endif //LIB_ASMKRAKEN_PATTERNSCANMETA_H
