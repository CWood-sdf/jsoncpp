#include "utils.h"
#include <cstddef>

template <Str s, size_t len, int _i, int currentNum = 0>
struct ReadNum {
    static const int value = currentNum;
    static const int i = _i;
};

template <Str s, size_t len, int _i>
    requires(!IsNum<s[_i]>)
struct ReadNum<s, len, _i, 0> {
    static_assert(false, "No number to read");
    static const int value = 0;
    static const int i = _i;
};

template <Str s, size_t len, int _i>
    requires IsNum<s[_i]>
struct ReadNum<s, len, _i, 0> {
    typedef ReadNum<s, len, _i + 1, s[_i] - '0'> next;
    static const int value = next::value;
    static const int i = next::i;
};

template <Str s, size_t len, int _i, int currentNum>
    requires IsNum<s[_i]>
struct ReadNum<s, len, _i, currentNum> {
    typedef ReadNum<s, len, _i + 1, currentNum * 10 + s[_i] - '0'> next;
    static const int value = next::value;
    static const int i = next::i;
};
