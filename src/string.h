#pragma once
// #include <concepts>
#include "utils.h"
#include <iostream>
#include <string>
#include <vector>

template <char... T>
struct StringClass {
    static constexpr std::string toString() {
        if (!std::is_constant_evaluated()) {
            std::cout << "ruh roh" << std::endl;
        }
        std::vector<char> vec = {T...};
        std::string ret = "";
        for (auto& c : vec) {
            ret += c;
        }
        return ret;
    }
};

template <typename T, char c>
struct Concat {
    static_assert(false, "Dont use this");
};
template <char c, char... els>
struct Concat<StringClass<els...>, c> {
    typedef StringClass<els..., c> ret;
};

template <Str s, size_t len, int _i, typename CurrentString = StringClass<>,
    bool done = false>
    requires InBounds<len, _i>
struct ReadString {
    typedef Concat<CurrentString, s[_i]>::ret Current;
    typedef ReadString<s, len, _i + 1, Current, true> Next;
    static const int i = Next::i;
    static constexpr std::string value = Next::value;
};

template <Str s, size_t len, int _i>
    requires IsQuote<s[_i]> && InBounds<len, _i>
struct ReadString<s, len, _i, StringClass<>, false> {
    typedef ReadString<s, len, _i + 1, StringClass<>, true> Next;
    static const int i = Next::i;
    static constexpr std::string value = Next::value;
};

template <Str s, size_t len, int _i>
    requires(!IsQuote<s[_i]> && InBounds<len, _i>)
struct ReadString<s, len, _i, StringClass<>, false> {
    static_assert(false, "Expected a \" to start a string");
};

template <Str s, size_t len, int _i, char... chars>
    requires IsQuote<s[_i]> && InBounds<len, _i>
struct ReadString<s, len, _i, StringClass<chars...>, true> {
    static const int i = _i + 1;
    static constexpr std::string value = StringClass<chars...>::toString();
};

template <Str s, size_t len, int _i>
struct ReadValue<s, len, _i, std::string> : public ReadString<s, len, _i> {};
