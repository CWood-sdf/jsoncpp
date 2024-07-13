#include "property.h"

template <class C>
concept IsProp = requires(C c) {
    // from stack overflow https://stackoverflow.com/a/71921982/22302689
    []<Str n, size_t len, typename T>(Prop<n, len, T>&) {}(c);
};

template <typename T, Str name, size_t len, int i = 0>
struct PropEq {
    typedef PropEq<T, name, len, i> Next;
    static const bool value = Next::value;
};

template <Str n, size_t len, typename T, Str name, size_t lenOther, int i>
    requires(len != lenOther)
struct PropEq<Prop<n, len, T>, name, lenOther, i> {
    static const bool value = false;
};

template <Str n, size_t len, typename T, Str name, size_t otherLen, int i>
    requires(i >= len - 1 && len == otherLen)
struct PropEq<Prop<n, len, T>, name, otherLen, i> {
    static const bool value = true;
};

template <Str n, size_t len, typename T, Str name, int i>
    requires(i <= len - 1 && n[i] == name[i])
struct PropEq<Prop<n, len, T>, name, len, i> {
    typedef PropEq<Prop<n, len, T>, name, len, i + 1> Next;
    static const bool value = Next::value;
};

template <Str n, size_t len, typename T, Str name, int i>
    requires(i <= len - 1 && n[i] != name[i])
struct PropEq<Prop<n, len, T>, name, len, i> {
    static const bool value = false;
};

template <IsProp... T>
struct JsonPropertyList {};

template <Str s, size_t len, int _i, class JsonPropList>
struct ReadPropList {};

template <Str s, size_t len, int _i, class JsonPropList>
struct JsonBlock {
    typedef SkipWhitespace<s, len, _i> Skip;

    static_assert(s[Skip::i] == '{', "Expected a { to start a json block");
    typedef SkipWhitespace<s, len, Skip::i + 1> Skip2;

    typedef ReadPropList<s, len, Skip2::i, JsonPropList> Read;

    typedef SkipWhitespace<s, len, Read::i> Skip3;

    static_assert(s[Skip3::i] == '}', "Expected a } to close a json block");

    static constexpr auto value = Read::value;
    static const int i = Skip3::i + 1;
};
template <Str s, size_t len, int _i, IsProp... props>
struct ReadValue<s, len, _i, JsonPropertyList<props...>>
  : public JsonBlock<s, len, _i, JsonPropertyList<props...>> {};

template <Str s, size_t len, int _i, IsProp T1, IsProp... T>
struct ReadPropList<s, len, _i, JsonPropertyList<T1, T...>> {
    typedef SkipWhitespace<s, len, _i> Skip;

    typedef ReadProp<s, len, Skip::i, T1> Prop;

    typedef SkipWhitespace<s, len, Prop::i> Skip2;

    static_assert(s[Skip2::i] == ',', "Expected a comma after a property");

    typedef ReadPropList<s, len, Skip2::i + 1, JsonPropertyList<T...>> Next;

    static constexpr auto value = Prop::value;
    static const int i = Next::i;
};

template <Str s, size_t len, int _i, IsProp T1>
struct ReadPropList<s, len, _i, JsonPropertyList<T1>> {
    typedef SkipWhitespace<s, len, _i> Skip;

    typedef ReadProp<s, len, Skip::i, T1> Prop;

    typedef SkipWhitespace<s, len, Prop::i> Skip2;

    static constexpr auto value = Prop::value;
    static const int i = Skip2::i;
};

template <class T, IsProp P>
struct FindProp {
    static_assert(false, "Could not find prop");
};

template <Str s, size_t len, int _i, class JsonPropList, IsProp P>
struct FindProp<JsonBlock<s, len, _i, JsonPropList>, P> {

    typedef FindProp<typename JsonBlock<s, len, _i, JsonPropList>::Read, P>
        Read;

    typedef Read::Value Value;

    static constexpr auto value = Read::value;
};

template <Str s, size_t len, int _i, Str name, size_t nameLen, typename PropTp>
struct FindProp<ReadPropList<s, len, _i, JsonPropertyList<>>,
    Prop<name, nameLen, PropTp>> {
    static_assert(false, "Could not find prop");
};

template <Str s, size_t len, int _i, IsProp T1, Str name, size_t nameLen,
    typename PropTp, IsProp... T>
    requires(PropEq<T1, name, nameLen>::value)

struct FindProp<ReadPropList<s, len, _i, JsonPropertyList<T1, T...>>,
    Prop<name, nameLen, PropTp>> {

    typedef ReadPropList<s, len, _i, JsonPropertyList<T1, T...>> Read;
    typedef Read::Prop Value;

    static constexpr auto value = Read::value;
};

// stuff

template <Str s, size_t len, int _i, IsProp T1, Str name, size_t nameLen,
    typename PropTp, IsProp... T>
    requires(!PropEq<T1, name, nameLen>::value)
struct FindProp<ReadPropList<s, len, _i, JsonPropertyList<T1, T...>>,
    Prop<name, nameLen, PropTp>> {
    typedef ReadPropList<s, len, _i, JsonPropertyList<T1, T...>> ReadNext;
    typedef FindProp<typename ReadNext::Next, Prop<name, nameLen, PropTp>> Next;

    typedef Next::Value Value;
    static constexpr auto value = Next::value;
};

template <Str s, size_t len, int _i, IsProp T1, Str name, size_t nameLen,
    typename PropTp>
    requires(PropEq<T1, name, nameLen>::value)
struct FindProp<ReadPropList<s, len, _i, JsonPropertyList<T1>>,
    Prop<name, nameLen, PropTp>> {
    typedef ReadPropList<s, len, _i, JsonPropertyList<T1>> Read;
    typedef Read::Prop::Value Value;
    static constexpr auto value = Read::value;
};

//

template <Str s, size_t len, int _i, IsProp T1, Str name, size_t nameLen,
    typename PropTp>
    requires(!PropEq<T1, name, nameLen>::value)
struct FindProp<ReadPropList<s, len, _i, JsonPropertyList<T1>>,
    Prop<name, nameLen, PropTp>> {
    static_assert(false, "Could not find prop");
};
