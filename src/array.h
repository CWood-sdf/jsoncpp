
#include "jsonBlock.h"
#include "utils.h"
#include <cstddef>
template <typename T>
struct JsonArrayDef {};

// template <auto... els>
// struct JsonArray {};

// template <auto L, class R>
// struct MergeArray {
//     static_assert(false, "asdf");
// };

template <class Arr, int i>
struct GetArrayElement {
    static_assert(false, "Not an array type");
};
template <class Arr, int i>
struct GetArrayElement<const Arr, i> : public GetArrayElement<Arr, i> {};

// template <typename T, T el, T... Els>
// struct MergeArray<el, JsonArray<Els...>> {
//     typedef JsonArray<el, Els...> Ret;
// };
// template <typename T, T el>
// struct MergeArray<el, JsonArray<>> {
//     typedef JsonArray<el> Ret;
// };

template <Str s, size_t len, int _i>
struct SkipCommaMaybe {
    static const int i = _i;
    static const bool hasComma = false;
};
template <Str s, size_t len, int _i>
    requires(s[_i] == ',')
struct SkipCommaMaybe<s, len, _i> {
    static const int i = _i + 1;
    static const bool hasComma = true;
};

template <Str s, size_t len, int _i, typename T, bool commaBefore = true>
struct ReadJsonArrayElement {
    typedef SkipWhitespace<s, len, _i> Skip;
    typedef ReadValue<s, len, Skip::i, T> Read;
    typedef SkipCommaMaybe<s, len, Read::i> Comma;
    typedef SkipWhitespace<s, len, Comma::i> Skip2;
    typedef ReadJsonArrayElement<s, len, Skip2::i, T, Comma::hasComma> Next;
    // typedef MergeArray<Read::value, typename Next::Ret>::Ret Ret;
    static const int i = Next::i;
    static constexpr auto value = Read::value;
};
template <typename T>
concept HasStaticValue = requires() { []() { return T::value; }(); };
template <Str s, size_t len, int _i, typename T, bool commaBefore, int i>
    requires(i >= 0 &&
             !HasStaticValue<ReadJsonArrayElement<s, len, _i, T, commaBefore>>)
struct GetArrayElement<ReadJsonArrayElement<s, len, _i, T, commaBefore>, i> {
    static_assert(false, "I is too big for array");
};

template <Str s, size_t len, int _i, typename T, bool commaBefore>
    requires(HasStaticValue<ReadJsonArrayElement<s, len, _i, T, commaBefore>>)
struct GetArrayElement<ReadJsonArrayElement<s, len, _i, T, commaBefore>, 0> {
    static constexpr auto value =
        ReadJsonArrayElement<s, len, _i, T, commaBefore>::value;
};
template <Str s, size_t len, int _i, typename T, bool commaBefore, int i>
    requires(i > 0)
struct GetArrayElement<ReadJsonArrayElement<s, len, _i, T, commaBefore>, i> {
    static constexpr auto value = GetArrayElement<
        typename ReadJsonArrayElement<s, len, _i, T, commaBefore>::Next,
        i - 1>::value;
};
template <Str s, size_t len, int _i, typename T>
    requires(s[_i] == ']')
struct ReadJsonArrayElement<s, len, _i, T, true> {
    // typedef JsonArray<> Ret;
    static const int i = _i + 1;
};
template <Str s, size_t len, int _i, typename T>
struct ReadJsonArrayElement<s, len, _i, T, false> {
    typedef SkipWhitespace<s, len, _i> Skip;
    static_assert(s[Skip::i] == ']');
    // typedef JsonArray<> Ret;
    static const int i = Skip::i + 1;
};

template <Str s, size_t len, int _i, typename T>
struct ReadJsonArrayElements {
    typedef SkipWhitespace<s, len, _i> Skip;
    typedef ReadJsonArrayElement<s, len, Skip::i, T> Read;
    static const int i = Read::i;
    static constexpr Read value;
};

template <Str s, size_t len, int _i, typename T>
struct ReadValue<s, len, _i, JsonArrayDef<T>> {
    typedef SkipWhitespace<s, len, _i> Skip;
    static_assert(s[Skip::i] == '[', "Expected a [ to start a json array");
    typedef ReadJsonArrayElements<s, len, Skip::i + 1, T> Read;
    static constexpr auto value = Read::value;
    static const int i = Read::i;
    typedef decltype(value) Value;
};
