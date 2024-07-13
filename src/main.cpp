#include "jsonBlock.h"
using namespace std;
int main() {
    static constexpr char str[] =
        "{ \"aasdf\": 89, \"aslj\": \"asdf\", \"ooga\": { \"aasdf\": 69, "
        "\"aslj\": \"ooogabooga\" } }";
    static constexpr char propName[] = "aasdf";
    static constexpr char propName2[] = "aslj";
    static constexpr char propName3[] = "ooga";
    typedef Prop<propName, sizeof(propName), int> P;
    typedef Prop<propName2, sizeof(propName2), string> P2;
    typedef Prop<propName3, sizeof(propName3), JsonPropertyList<P, P2>> P3;
    // cout << ReadProp<str, sizeof(str), 0, P>::value << endl;
    typedef JsonBlock<str, sizeof(str), 0, JsonPropertyList<P, P2, P3>> Read;
    // cout << Read::Next::value << endl;
    // cout << PropEq<P2, propName, sizeof(propName)>::value << endl;
    typedef FindProp<Read, P3>::Value SubObj;
    SubObj asdf;

    cout << FindProp<SubObj, P>::value << endl;

    // cout << FindProp<FindProp<Read, P3>, P>::value << endl;
    // cout << Read::i << endl;
}
