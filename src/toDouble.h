#include "toNum.h"

template <Str s, size_t len, int _i>
struct ParseDecimalNumber {
    static constexpr double value = 0.0;
    static const int i = _i;
};
template <Str s, size_t len, int _i>
    requires(IsNum<s[_i]>)
struct ParseDecimalNumber<s, len, _i> {
    typedef ParseDecimalNumber<s, len, _i + 1> Next;
    static constexpr double value = (Next::value + s[_i] - '0') / 10.0;
    static const int i = Next::i;
};
template <Str s, size_t len, int _i>
struct ParseDecimal {
    typedef ParseDecimalNumber<s, len, _i + 1> Parse;
    static const int i = Parse::i;
    static constexpr double value = Parse::value;
};
template <Str s, size_t len, int _i>
    requires(s[_i] != '.')
struct ParseDecimal<s, len, _i> {
    static const int i = _i;
    constexpr static const double value = 0.0;
};

template <Str s, size_t len, int _i>
struct ReadFloat {
    typedef ReadNum<s, len, _i> IntegerComponent;
    typedef ParseDecimal<s, len, IntegerComponent::i> Decimal;
    static const int i = Decimal::i;
    static constexpr double value =
        (double)IntegerComponent::value + Decimal::value;
};

template <Str s, size_t len, int _i>
struct ReadValue<s, len, _i, double> : public ReadFloat<s, len, _i> {};
