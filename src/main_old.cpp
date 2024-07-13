// #include <iostream>
// #include <stdexcept>
// #include <string>
// #include <tuple>
// #include <type_traits>
// using namespace std;
//
// class StringView {
//
// public:
// 	const char* data;
// 	int len;
// 	constexpr StringView(const char* data, int len) : data(data), len(len) {}
// 	std::string toStr() const {
// 		string ret = "";
// 		for (int i = 0; i < len; i++) {
// 			ret += data[i];
// 		}
// 		return ret;
// 	}
// };
// template <typename T> class BasicJsonValue {
// public:
// 	T value;
// 	constexpr BasicJsonValue<T>(T v) : value(v) {}
// };
//
// template <typename T> class BasicLexRet {
// public:
// 	const T ret;
// 	const int i;
// 	constexpr BasicLexRet(T ret, int i) : ret(ret), i(i) {}
// };
// template <typename T> class LexRet;
// template <typename T> class JsonValue {
// public:
// 	static constexpr LexRet<T> parseValue(const char* str, int i);
// };
// template <typename T> class LexRet : public BasicLexRet<JsonValue<T>> {
// public:
// 	constexpr LexRet(JsonValue<T> ret, int i)
// 	  : BasicLexRet<JsonValue<T>>(ret, i) {}
// };
//
// template <typename... T> class JsonList {
// public:
// 	static constexpr BasicLexRet<tuple<T...>> parseValue(
// 		const char* str, int i) {
// 		if (!std::is_constant_evaluated()) {
// 			cout << "ruh roh" << endl;
// 		}
// 		tuple<T...> ret;
// 		for (size_t j = 0; j < sizeof...(T); j++) {
// 		}
// 	}
// };
//
// template <const char* const _name, typename T, int l> class JsonKv {
// public:
// 	static constexpr StringView name = StringView(_name, l);
// 	JsonValue<T> value;
// 	constexpr JsonKv(JsonValue<T> value) : value(value) {}
// 	static constexpr BasicLexRet<JsonKv<_name, T, l>> parseValue(
// 		const char* str, int i) {
// 		if (!std::is_constant_evaluated()) {
// 			cout << "ruh roh" << endl;
// 		}
// 		// constexpr char qt = '"';
// 		// static_assert(str[i] == qt, "error parsing string");
// 		while (str[i] == ' ' || str[i] == '\n' || str[i] == '\t') {
// 			i++;
// 		}
// 		if (str[i] != '"') {
// 			throw std::logic_error("error parsing string");
// 		}
// 		i++;
// 		int len = 0;
// 		int viewI = 0;
// 		while (str[i] != '"') {
// 			if (len >= name.len) {
// 				throw std::logic_error("error parsing string");
// 			}
// 			if (str[i] != name.data[viewI]) {
// 				throw std::logic_error("error parsing string");
// 			}
// 			i++;
// 			viewI++;
// 			len++;
// 		}
// 		// if (len < name.len) {
// 		// 	throw std::logic_error("error parsing string");
// 		// }
// 		if (str[i] != '"') {
// 			throw std::logic_error("error parsing string");
// 		}
// 		i++;
// 		if (str[i] != ':') {
// 			throw std::logic_error("error parsing string");
// 		}
// 		i++;
// 		LexRet<T> next = JsonValue<T>::parseValue(str, i);
// 		i = next.i;
// 		return BasicLexRet(JsonKv<_name, T, l>(next.ret), i);
// 	}
// 	string getName() const {
// 		return name.toStr();
// 	}
// };
//
// template <> class JsonValue<int> : public BasicJsonValue<int> {
// public:
// 	using BasicJsonValue<int>::BasicJsonValue;
// 	static constexpr LexRet<int> parseValue(const char* str, int i) {
// 		if (!std::is_constant_evaluated()) {
// 			cout << "ruh roh" << endl;
// 		}
// 		int ret = 0;
// 		while (str[i] == ' ' || str[i] == '\n' || str[i] == '\t') {
// 			i++;
// 		}
// 		while (str[i] >= '0' && str[i] <= '9') {
// 			ret *= 10;
// 			ret += str[i] - '0';
// 			i++;
// 		}
// 		if (str[i] != ',' && str[i] != ' ' && str[i] != '\0') {
// 			throw std::invalid_argument("error parsing int");
// 		}
// 		return LexRet(JsonValue<int>(ret), i);
// 	}
// };
// template <> class JsonValue<StringView> : public BasicJsonValue<StringView> {
// public:
// 	using BasicJsonValue<StringView>::BasicJsonValue;
// 	static constexpr LexRet<StringView> parseValue(const char* str, int i) {
// 		if (!std::is_constant_evaluated()) {
// 			cout << "ruh roh" << endl;
// 		}
// 		// constexpr char qt = '"';
// 		// static_assert(str[i] == qt, "error parsing string");
// 		while (str[i] == ' ' || str[i] == '\n' || str[i] == '\t') {
// 			i++;
// 		}
// 		if (str[i] != '"') {
// 			throw std::logic_error("error parsing string");
// 		}
// 		i++;
// 		int len = 0;
// 		auto start = str + i;
// 		while (str[i] != '"') {
// 			i++;
// 			len++;
// 		}
// 		i++;
// 		return LexRet(JsonValue<StringView>(StringView(start, len)), i);
// 	}
// };
//
// template <const char* const str> class PrintWord {
// public:
// 	constexpr std::string get() {
// 		return str;
// 	}
// };
// constexpr int countChars(const char* str) {
// 	int ret = 0;
// 	for (int i = 0; str[i] != '\0'; i++) {
// 		ret++;
// 	}
// 	return ret;
// }
// class Json {};
//
// template <long long v> class Fib {
// public:
// 	static const long long value = Fib<v - 1>::value + Fib<v - 2>::value;
// };
//
// template <> class Fib<0> {
// public:
// 	static const long long value = 1;
// };
// template <> class Fib<1> {
// public:
// 	static const long long value = 1;
// };
//
// int main() {
// 	// Json value;
// 	static constexpr char name[] = "asdf";
//
// 	constexpr auto parse = JsonValue<int>::parseValue("   89", 0);
// 	constexpr auto parse2 = JsonValue<StringView>::parseValue("  \"asdf\" ", 0);
// 	constexpr auto parse3 =
// 		JsonKv<name, int, sizeof(name) - 1>::parseValue("  \"asdf\": 69 ", 0);
// 	cout << Fib<80>::value << endl;
// 	cout << parse.ret.value << endl;
// 	cout << parse2.ret.value.toStr() << endl;
// 	cout << parse3.ret.getName() << endl;
// 	cout << parse3.ret.value.value << endl;
// 	cout << JsonKv<name, int, sizeof(name) - 1>::name.len << endl;
// 	cout << sizeof(name) << endl;
// }
