#include <iostream>
#include <string>


/* 
SFINAE - substitution failure is not an error!

SFINAE is a technique that allows us to write code that will compile only 
if certain conditions are met.

This allows us to 
    - write generic code that will work for many types, but not all.
    - catch errors at compile time.
    - Create powerful APIs that are flexible and generic.
*/ 


struct ClassWithType {using type = int;};

template <typename T>
void foo(T t, typename T::type x) {
     std::cout << "foo is called" << std::endl;
}


void callingFoo() {
    ClassWithType c;

    foo(c, 5); 

    //Compilation error!!! WHY?
    foo(4, 4); 
}

/* 
    1. template deduction:

    - When we call a template function, the compiler tries to deduce the template parameters from the arguments.
        - name lookup: the compiler looks for the function name in the current scope.
        - for template functions, the compiler tries to replace all template instances with the parameter passed.
        - If the susbsitution is successful, it is added to the overload set.
        - If the substitution is not successful, it is not added to the overload set.
        - If the overload set is empty, the compiler will throw an error.
        - If the overload set is not empty, the compiler will choose the best match.
        - If all the overloads are equally good, the compiler will throw an error over ambigous call.

        let's go back to previous example and try to understand what happens with each overload.
*/

// This will be a compile error if we use the wrong type.. WHY?
template <typename T>
void boo(T t) {
    typename T::type x; 

     std::cout << "foo is called" << std::endl;
}



// ========== finally some meat! SFINAE WITH DECLTYPES ===========

/*
    I have a function called PrintMe that takes a parameter of type T and prints it.

    Some types can be turned into a string.
    Some types can't, but have a c_str()/talk() function that returns a const char*

    Let's solve it
*/

class WithCStr {
public:
    const char* c_str() const { return "WithCStr"; }
};

class Grut {
public:
    const char* talk() const { return "I am grut"; }
};

//overload for types that have c_str()
template <typename T>
decltype(std::declval<T>().c_str(), void()) PrintMe(T t) {
    std::cout << t.c_str() << std::endl;
}

//overload for types that have talk() (auto here says first evalute parameters and then return type)
template <typename T>
auto PrintMe(T t) -> decltype(t.talk(), void()) {
    std::cout << t.talk() << std::endl;
}

//overload for types that have neither c_str() nor talk() but can be converted to string
template <typename T>
auto PrintMe(T t) -> decltype(std::to_string(t), void()) {
    std::cout << std::to_string(t).c_str() << std::endl;
}


int main() {
    PrintMe(std::string{"hello"});
    PrintMe(WithCStr{});
    PrintMe(Grut{});
    PrintMe(5);
}




