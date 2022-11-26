#include <iostream>
#include <type_traits>

// TYPE TRAITS 

/*
    Somewhere along the way, we got type traits. 
    They are special template stucts that can create a compile type BOOL according to a template parameter

    some examples are:
*/

template <typename T>
constexpr bool is_int = std::is_same<T, int>::value; // true if T is int, false otherwise

template <typename T>
constexpr bool is_pointer = std::is_pointer<T>::value; // true if T is a pointer, false otherwise

template <typename T>
constexpr bool is_reference = std::is_base_of<std::string, T>::value; // true if T is inherited from string, false otherwise

/*
    To understand these traits we must first meet two base structs:
        - std::true_type
        - std::false_type

    They are actually structs of type std::integral_constant<bool, true> and std::integral_constant<bool, false>
    a struct that used to provide compile time constants as TYPES!

    So std::true_type::value is true and std::false_type::value is false

    When you implement a type trait, you must inherit from std::true_type or std::false_type
    This will let you call type_trait::value to get the boolean result
    You can also call ::type to get the type of the boolean result (std::true_type or std::false_type)

    Let's see how std::is_same implements it's logic:
*/

template <typename T, typename U>
struct my_is_same : std::false_type {
    //constexpr static bool value = false;
};
 
template <typename T>
struct my_is_same<T, T> : std::true_type {
    //constexpr static bool value = true;
};

// I can now call my_is_same<int, int>::value to get true

//from c++17 every type trait has a _v suffix that returns the value of the type trait
template <typename T, typename U>
constexpr bool my_is_same_v = my_is_same<T, U>::value;


/* 
    Let's do something more complicated.. 
    Our own type traits
*/

//1. a type trait that tells us if it is either a pointer or a reference
template <typename T>
struct is_pointer_or_reference : std::__or_<std::is_reference<T>, std::is_pointer<T>>::type {};

//2. a type trait that tells if T has a member function c_str() that returns const char*...
template <typename T, typename = void>
struct has_c_str : std::false_type {};

template <typename T>
struct has_c_str<T, decltype(std::declval<T>().c_str(), void())>  : std::is_same<decltype(std::declval<T>().c_str()), const char*>::type {};


int main() {

    int x = 5;
    int& x_ref = x;

    std::cout << std::boolalpha << "is x a pointer or reference?   " << is_pointer_or_reference<decltype(x)>::value << std::endl;
    std::cout << std::boolalpha << "is x_ref a pointer or reference?   " << is_pointer_or_reference<decltype(x_ref)>::value << std::endl;

    std::cout << std::boolalpha << "does int have c_str?   " << has_c_str<int>::value << std::endl; 
    std::cout << std::boolalpha << "does std::string have c_str?   " << has_c_str<std::string>::value << std::endl; 
}