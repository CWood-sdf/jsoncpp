#include "string.h"
#include "toNum.h"
#include "utils.h"
#include <string>
template <Str name, size_t len, typename Tp>
struct Prop {
    static constexpr std::string nameStr = name;
    typedef Tp Type;
};

template <Str s, size_t len, int _i, typename Prop, int propI = -1>
struct ValidateProp {
    static_assert(false, "could not match parameters");
};

// starting quote
template <Str s, size_t len, int _i, Str propName, size_t propLen, typename Tp>
    requires IsQuote<s[_i]>
struct ValidateProp<s, len, _i, Prop<propName, propLen, Tp>, -1> {
    static const int i =
        ValidateProp<s, len, _i + 1, Prop<propName, propLen, Tp>, 0>::i;
};

// assert that starts with quote
template <Str s, size_t len, int _i, Str propName, size_t propLen, int propI,
    typename Tp>
    requires(!IsQuote<s[_i]> && propI == -1)
struct ValidateProp<s, len, _i, Prop<propName, propLen, Tp>, propI> {
    static_assert(false, "Property names must begin with a double quote");
};

// asserts that chars match
template <Str s, size_t len, int _i, Str propName, size_t propLen, int propI,
    typename Tp>
    requires(!IsQuote<s[_i]> && s[_i] == propName[propI])
struct ValidateProp<s, len, _i, Prop<propName, propLen, Tp>, propI> {
    static const int i =
        ValidateProp<s, len, _i + 1, Prop<propName, propLen, Tp>, propI + 1>::i;
};
template <Str s, size_t len, int _i, Str propName, size_t propLen, int propI,
    typename Tp>
    requires(!IsQuote<s[_i]> && s[_i] != propName[propI])
struct ValidateProp<s, len, _i, Prop<propName, propLen, Tp>, propI> {
    static_assert(false, "ooga booga");
};
// reached end on expected prop, but not on actual: error
template <Str s, size_t len, int _i, Str propName, size_t propLen, int propI,
    typename Tp>
    requires(!IsQuote<s[_i]> && propI >= propLen - 1)
struct ValidateProp<s, len, _i, Prop<propName, propLen, Tp>, propI> {
    static_assert(false, "expected prop is smaller than actual prop");
};

// reached end on actual prop, but not on expected: error
template <Str s, size_t len, int _i, Str propName, size_t propLen, int propI,
    typename Tp>
    requires(IsQuote<s[_i]> && propI < propLen - 1)
struct ValidateProp<s, len, _i, Prop<propName, propLen, Tp>, propI> {
    static_assert(false, "expected prop is bigger than actual prop");
};

// reached end ok, return i+1
template <Str s, size_t len, int _i, Str propName, size_t propLen, int propI,
    typename Tp>
    requires(IsQuote<s[_i]> && propI == propLen - 1)
struct ValidateProp<s, len, _i, Prop<propName, propLen, Tp>, propI> {
    static const int i = _i + 1;
};

template <Str s, size_t len, int _i, typename T>
struct ReadValue {};

template <Str s, size_t len, int _i>
struct ReadValue<s, len, _i, int> : public ReadNum<s, len, _i> {};

template <Str s, size_t len, int _i>
struct ReadValue<s, len, _i, std::string> : public ReadString<s, len, _i> {};

template <Str s, size_t len, int _i, typename PropTp>
struct ReadProp {
    typedef ValidateProp<s, len, _i, PropTp> Validation;
    static_assert(s[Validation::i] == ':', "Expected a colon after property");
    typedef SkipWhitespace<s, len, Validation::i + 1> Skip;
    typedef ReadValue<s, len, Skip::i, typename PropTp::Type> Value;
    typedef SkipWhitespace<s, len, Value::i> Skip2;
    static constexpr auto value = Value::value;
    static const int i = Skip2::i;
};
