#include <stdio.h>
#include <iostream>
#include <chrono>
#include <type_traits>


// A REAL WORLD EXAMPLE FROM RedSea

/*
    Benchmark gets any callable and parameters.
    It runs the function, tells us how much time it took and returns the result of the function.

    //***NOTE rvalue references and std::forward are not used for simplicity.
*/

template <typename T, typename... Args>
auto Benchmark(const char* name, T func, Args... args) -> decltype(func(args...)) {
    auto start = std::chrono::system_clock::now();
    auto result = func(args...);
    auto end = std::chrono::system_clock::now();

    printf("%s took %ld\n", name, (end - start).count());
    
    return result;
}

int main() {

    // benchmark work as expected
    int x = Benchmark("test 1", []() { return 5; });
    int y = Benchmark("test 2", [](int a, int b) { return  a + b; }, 10, 30);

    // UNLESS WE TRY TO RETURN VOIIIDDD!!!
     Benchmark("test 1", []() { printf("haha\n"); });

     // How can we use SFINAE to make this work?
}

/*
 ANSWERS
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
We need two functions one for void and one for non void.
The difference should be mainly in the function declaration, we use return type for the sfinae

1. For the first function, enable if the return type is void, return void.

template <typename T, typename... Args>
auto Benchmark(const char* name, T func, Args... args) -> typename std::enable_if<std::is_void<decltype(func(args...))>::value, void>::type {

2. For the second function, enable if the return type is not void, return the return type of the invocation

template <typename T, typename... Args>
auto Benchmark(const char* name, T func, Args... args) -> typename std::enable_if<!std::is_void<decltype(func(args...))>::value, decltype(func(args...))>::type {



*/




