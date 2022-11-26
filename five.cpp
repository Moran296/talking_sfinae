#include <iostream>
#include <type_traits>

// ENTER C++17

template <typename T, typename = void>
struct has_c_str : std::false_type {};

template <typename T>
struct has_c_str<T, decltype(std::declval<T>().c_str(), void())>  : std::is_same<decltype(std::declval<T>().c_str()), const char*>::type {};


class WithCStr {
public:
    const char* c_str() const { return "WithCStr"; }
};

class Grut {
public:
    const char* talk() const { return "I am grut"; }
};

// This is already much easier and much clearer

template <typename T>
void PrintMe(T t) {
    //static_assert(std::is_same_v<Grut> || has_c_str<T>::value);

    if constexpr (std::is_same_v<T, Grut>) {
        std::cout << t.talk() << std::endl;
    } else if constexpr (has_c_str<T>::value) {
        std::cout << t.c_str() << std::endl;
    } else {
        std::cout << t << std::endl;
    }
}



int main() {
    PrintMe(std::string{"hello"});
    PrintMe(WithCStr{});
    PrintMe(Grut{});
    PrintMe(5);
}