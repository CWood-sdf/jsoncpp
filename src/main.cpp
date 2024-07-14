#include "array.h"
using namespace std;
// the _ pattern in rust, it has to be declared first because any
// constraints on the primary class template must apply to all overloads
template <int foo>
struct Sign {
    static const int sign = 1;
};
template <int foo>
    requires(foo < 0)
struct Sign<foo> {
    static const int sign = -1;
};
template <>
struct Sign<0> {
    static const int sign = 0;
};
int main() {
    static constexpr char str[] =
        R"({ "urmom": [69, 420], "aasdf": 0, "aslj": "asdf", "ooga": { "aasdf": 69, "aslj": "ooogabooga" } })";
    static constexpr char arr[] = R"([ { "aasdf": 420.5 } ])";
    static constexpr char propName[] = "aasdf";
    static constexpr char propName2[] = "aslj";
    static constexpr char propName3[] = "ooga";
    static constexpr char propName4[] = "urmom";

    typedef Prop<propName, sizeof(propName), double> P_aasdf;
    typedef Prop<propName2, sizeof(propName2), string> P_aslj;
    typedef Prop<propName3, sizeof(propName3),
        JsonPropertyList<P_aasdf, P_aslj>>
        P_ooga;
    typedef Prop<propName4, sizeof(propName4), JsonArrayDef<int>> P_urmom;
    typedef ReadValue<arr, sizeof(arr), 0,
        JsonArrayDef<JsonPropertyList<P_aasdf>>>
        Arr;
    // typedef decltype(Arr::value) ArrTp;

    typedef decltype(GetArrayElement<Arr::Value, 0>::value) ArrSubObj;

    printf("%f\n", FindProp<ArrSubObj, P_aasdf>::value);

    // printf("%d\n", FindProp<ArrSubObj, P_aasdf>::value);

    // printf("%d\n", ReadProp<str, sizeof(str), 0, P>::value);
    typedef JsonBlock<str, sizeof(str), 0,
        JsonPropertyList<P_urmom, P_aasdf, P_aslj, P_ooga>>
        Read;
    // printf("%d\n", Read::Next::value);
    // printf("%d\n", PropEq<P2, propName, sizeof(propName)>::value);
    typedef FindProp<Read, P_ooga> SubObj;

    typedef FindProp<Read, P_urmom> Arr2;

    // printf("%d\n", GetArrayElement<Arr2::Value, 0>::value);
    //
    // printf("%d\n", FindProp<SubObj::Value, P_aslj>::value);
    //
    //
    // printf("%d\n", FindProp<Read, P_aslj>::value);
}
