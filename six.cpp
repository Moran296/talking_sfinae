#include <iostream>
#include <type_traits>


/*
    Enter C++20

    in C++20 we have a new concept called "concepts" which is a way to express requirements on template parameters.
    It means that we can forget about 85% of everything we learned about SFINAE and type traits and just use concepts.

    This is good :)

    concepts are in the end some kind of compile time bool according to some compile time condition,
    very much like type traits, but has better syntax and is more powerful.
*/


// Let's define a concept that says that a type has a c_str() method that returns a const char*
// we can use predefined concepts inside our own concepts like std::same_as, std::convertible_to, std::derived_from, etc.
template <typename T>
concept HasCStr = requires(T t) {
    { t.c_str() } -> std::same_as<const char*>;
    //{ t.c_str() } -> std::convertible_to<const char*>;
};

//Now let's see how we can use this concept in a function

// This is the basic sybtax for a function that uses a concept
template <class T>
requires HasCStr<T>
void PrintMe(T t) {
    std::cout << t.c_str() << std::endl;
}

//We can also use concepts as template parameters
template <HasCStr T>
void PrintMe_2(T t) {
    std::cout << t.c_str() << std::endl;
}

// Actually we don't even need the template line!!
void PrintMe_3(HasCStr auto t) {
    std::cout << t.c_str() << std::endl;
}

//Above three functions are exactly the same!!!

/*
    Concepts are a very powerful tool.
    Togather with constexper/consteval they are a huge step for compile time programming.
    With concepts you don't need to pass a type to a function, 
    you can pass any thing that is convertible to that type and create more generic code in compile time.

    This is real static polymorphism.
*/

template <typename T>
concept JsonSerializable = requires(T t) {
    { t.c_str() } -> std::same_as<const char*>;} ||
    std::is_same_v<T, const char*> ||
    std::is_arithmetic_v<T> ||
    std::is_same_v<T, std::nullptr_t>;

void AddToJson(std::convertible_to<const char*> auto key, JsonSerializable auto value) {
    //do something with the key and something with the value
}

int main() {
    std::string s = "hello world";
    PrintMe(s);
    PrintMe_2(s);
    PrintMe_3(s);

    // enable to see compilation error on concepts
    // PrintMe("hello world");
}

/*
    Concept are duck typed traits.
    A class can satisfy a concept if it has some traits/methods
    You cannot implement a concept, you can only satisfy it.

    This is the time foooooooooooor....
*/
