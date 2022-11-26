#include <iostream>
#include <type_traits>

// Now let's use our type traits in SFINAE functions

/*
    C++11 gave us a very simple template struct called enable_if.
*/

template <bool Condition, typename T = void>
struct my_enable_if {};

template <typename T>
struct my_enable_if<true, T> {
    typedef T type;
    // using type = T;
};

// The inner type only exists if the consition is true! 
// We can use type traits as consditions!

// our traits from last time
template <typename T, typename = void>
struct has_c_str : std::false_type {};

template <typename T>
struct has_c_str<T, decltype(std::declval<T>().c_str(), void())>  : std::is_same<decltype(std::declval<T>().c_str()), const char*>::type {};


// as a default parameter int* = 0
template <typename T>
void PrintMe_1(T t, typename my_enable_if<has_c_str<T>::value, int>::type = 0) {
// collapses to:  void PrintMe_2(T t, int = 0), can we get read of int?
    std::cout << t.c_str() << " 1"<< std::endl;
}


//as a return type void (*notice enable_if_t)
template <typename T>
std::enable_if_t<has_c_str<T>::value, void> PrintMe_2(T t) {
// collapses to:  void PrintMe_2(T t)
    std::cout << t.c_str() << " 2" << std::endl;
}

//as a template parameter
template <typename T, typename std::enable_if<has_c_str<T>::value, int>::type = 0>
//collapses to: template <typename T, int = 0>
void PrintMe_3(T t) {
    std::cout << t.c_str() << " 3" << std::endl;
}

// A function for all things without c_str
template <typename T>
void PrintMe_4(T t, typename my_enable_if<!has_c_str<T>::value, int>::type = 0) {
    std::cout << "hello " << t << std::endl;
}


int main() {
    std::string s = "hello";
    PrintMe_1(s);
    PrintMe_2(s);
    PrintMe_3(s);
    PrintMe_4(4);
}