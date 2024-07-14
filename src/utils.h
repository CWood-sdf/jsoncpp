#pragma once

#include <cstddef>
typedef const char* const Str;

template <char c>
concept IsNum = c >= '0' && c <= '9';

template <char c>
concept IsWhitespace = c == ' ' || c == '\n' || c == '\t';

template <char c>
concept IsQuote = c == '"';

template <Str s, size_t len, int _i>
struct SkipWhitespace {
    static const int i = _i;
};

template <Str s, size_t len, int _i>
    requires IsWhitespace<s[_i]>
struct SkipWhitespace<s, len, _i> {
    typedef SkipWhitespace<s, len, _i + 1> next;
    static const int i = next::i;
};

template <size_t len, int _i>
concept InBounds = _i < len;

template <Str s, size_t len, int _i, typename T>
struct ReadValue {
    static_assert(false, "Could not read type");
};
