#ifndef LIB_ASMKRAKEN_TOGGLEABLE_H
#define LIB_ASMKRAKEN_TOGGLEABLE_H

namespace asmkraken {
    class Toggleable {
    public:
        virtual bool IsEnabled() = 0;
        virtual void Toggle() = 0;
        virtual void Enable() = 0;
        virtual void Disable() = 0;
    };
}

#endif //LIB_ASMKRAKEN_TOGGLEABLE_H
