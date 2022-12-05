#include <iostream>

/*
    Introduction!

    - As time goes on, c++ and other compiled languages are trying to achieve more in compile time.
    - Bugs in compile time are better than bugs in runtime. Why?
    - Every new edition of C++ has more constexper tools and uses. Templates are more powerful than ever.
    - Templates let us write generic code and create APIs that are more flexible and powerful.
    - First we will meet some new feautres that are the core of template meta programming.

    We are going to go over:
        - Compile time template meta programming in the past, present, future and beyond!
        - Template deduction
        - Decltype SFINAE
        - Type traits
        - Enable if SFINAE
        - Concepts
        - SURPRISE...
*/

// 1. =================== AUTO ===================

// What is auto deduced to?
auto a = 5; 

// What is auto deduced to?
auto f_1() { return 5; } // f_1 is a function that returns an int

// What is l1? what is param?
auto l1 = [](auto param) { return param; }; // NOTE: param is a template parameter

// what is this strange syntax? what is auto?
auto f_2() -> double
{
    return 5;
}

auto l2 = [](auto param) -> auto { return param; }; // too many autos makes c programmers cry

// 2. =================== DECLTYPE ===================

// I have a function
int foo() { return 5; }

// I have a struct
struct Goo {
    static char staticFunc() { return 'a'; }
    long memberFunc() { return 5; }
};


//what type is x?
decltype(foo()) x = 10; 
//what type is z?
decltype(Goo::staticFunc()) z = 'b';

// Why is this a compile error???
decltype(Goo::memberFunc()) wooops = 5; // error! 

// 3. ================= DECLVAL =================

// We solved the previous problem with std::declval
decltype(std::declval<Goo>().memberFunc()) better_now = 5; 



//4. ================= TEMPLATE SPECIALIZATION =================


// SPECIALIZING a function
template <typename T>
void foo(T t) {
    std::cout << "generic foo is called" << std::endl;
}

template <>
void foo(int t) {
    std::cout << "specialized foo is called" << std::endl;
}

// SPECIALIZING a class
template <typename T>
struct GenericStruct {
    void DoSomethingGeneric() {
        std::cout << "generic struct is called" << std::endl;
    }
};

template <>
struct GenericStruct<int> {
    // look!!! a complete different implementation of the class when it is specialized
    using bad = int;
    using drugs = bad;

    void DoSomethingWithAnInt() {
        std::cout << "specialized struct is called" << std::endl;
    }

    int x;
    long y;
};


// PARTIAL SPECIALIZATION
template <class One, class Two>
void boo() {}

template <class One>
void boo<One, char>() {}


