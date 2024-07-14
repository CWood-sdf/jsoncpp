#include "utils.h"

template <Str s, size_t len, int _i, int currentNum = 0, bool foundNum = false>
struct ReadNum {
    static const int value = currentNum;
    static const int i = _i;
};

template <Str s, size_t len, int _i>
    requires(!IsNum<s[_i]>)
struct ReadNum<s, len, _i, 0, false> {
    static_assert(false, "No number to read");
    static const int value = 0;
    static const int i = _i;
};

template <Str s, size_t len, int _i, bool found>
    requires IsNum<s[_i]>
struct ReadNum<s, len, _i, 0, found> {
    typedef ReadNum<s, len, _i + 1, s[_i] - '0', true> next;
    static const int value = next::value;
    static const int i = next::i;
};

template <Str s, size_t len, int _i, int currentNum, bool found>
    requires IsNum<s[_i]>
struct ReadNum<s, len, _i, currentNum, found> {
    typedef ReadNum<s, len, _i + 1, currentNum * 10 + s[_i] - '0', found> next;
    static const int value = next::value;
    static const int i = next::i;
};

template <Str s, size_t len, int _i>
struct ReadValue<s, len, _i, int> : public ReadNum<s, len, _i> {};
