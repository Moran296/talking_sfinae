---
title: Talking SFINAE
sub_title: the dark magic of C++ 11-20 and beyond
author: Moran
---

<!-- end_slide -->
<!-- jump_to_middle -->

Chapter 1: THE BASICS
===
<!-- end_slide -->
# Intro

<!-- incremental_lists: true -->
* As time goes on, c++ and other compiled languages are trying to achieve more in compile time.
* Bugs in compile time are better than bugs in runtime. Why?
* Every new edition of C++ has more constexper tools and uses. Templates are more powerful than ever.
* Templates let us write generic code and create APIs that are more flexible and powerful.
* First we will meet some new feautres that are the core of template meta programming.

<!-- pause -->

We are going to go over:
<!-- incremental_lists: true -->
* Compile time template meta programming in the past, present, future and beyond!
* Template deduction
* Decltype SFINAE
* Type traits
* Enable if SFINAE
* Concepts
* SURPRISE...

<!-- end_slide -->
<!-- jump_to_middle -->

Chapter 1: THE BASICS
===
<!-- end_slide -->
# autos
Let's review some of the new things we got when C++11 came out..

<!-- pause -->
What is this deduced to?
```c++
auto a = 5; 
```
<!-- pause -->
To:
```c++
int a = 5; 
```

<!-- pause -->
And this?
```c++
auto f_1()
{
     return 5;
}
```
<!-- pause -->
To:
```c++
int a = 5; 
```
<!-- end_slide -->
# lambdas

What about this?
```cpp
auto l = [](auto c) {return c;}
```
<!-- pause -->
To:
```cpp
class __lambda_1_10
{
  public: 
  template<class type_parameter_0_0>
  inline /*constexpr */ auto operator()(type_parameter_0_0 c) const
  {
    return c;
  }
  private: 
  template<class type_parameter_0_0>
  static inline /*constexpr */ auto __invoke(type_parameter_0_0 c)
  {
    return __lambda_1_10{}.operator()<type_parameter_0_0>(c);
  }
  
};

__lambda_1_10 l = __lambda_1_10{};
```

<!-- end_slide -->
# trailing return syntax


<!-- new_lines: 10 -->


Do you know the arrow return value syntax?

<!-- pause -->
```cpp
auto f_2() -> double
{
    return 5;
}
```

<!-- pause -->
too many autos makes c programmers cry
```cpp
auto l2 = [](auto param) -> auto { return param; }; 
```

<!-- pause -->
Riddle: Why is this syntax important? What does it give us?






<!-- end_slide -->
# decltypes

<!-- pause -->
I have a function!
```cpp
int foo() { return 5; }
```
<!-- pause -->
I have a struct!
```cpp
struct Goo {
    static char staticFunc() { return 'a'; }
    long memberFunc() { return 5; }
};
```
<!-- pause -->
what type is x?
```cpp
decltype(foo()) x = 10; 
```
<!-- pause -->
what type is z?
```cpp
decltype(Goo::staticFunc()) z = 'b';
```
<!-- end_slide -->
# declvals

```cpp
struct Goo {
    static char staticFunc() { return 'a'; }
    long memberFunc() { return 5; }
};
```

Why is this a compile error???
```cpp
decltype(Goo::memberFunc()) wooops = 5; // error! 
```
<!-- pause -->
Solving it with declval:
```cpp
decltype(std::declval<Goo>().memberFunc()) better_now = 5; 
```
<!-- end_slide -->


# funtion template specialization

Template specialization let's us define a whole different function/class according to the template paramters:

Here is a template function
```cpp
template <typename T>
void foo(T t) {
    std::cout << "generic foo is called" << std::endl;
}
```

<!-- pause -->
Here it is specialized on int
```cpp
template <>
void foo(int t) {
    std::cout << "int foo is called" << std::endl;
}
```

<!-- end_slide -->
# classes template specialization
We can do it for classes too:

```cpp
template<typename T>
struct GenericStruct
{
  inline void DoSomethingGeneric()
  {
    std::cout << "generic struct is called" << std::endl;
  }
  
};
```


<!-- pause -->
And.... specialized on int (notice the struct is completely different)
```cpp
template<>
struct GenericStruct<int>
{
  using bad = int;
  using drugs = bad;
  inline void DoSomethingWithAnInt()
  {
    std::cout << "specialized struct is called" << std::endl;
  }
  
  int x;
  long y;
};
```
<!-- end_slide -->
# partial specialization
And we can also do PARTIAL specialization

```cpp
template <class One, class Two>
struct boo {};
```

<!-- pause -->
specialization on the first/second parameter

```cpp
template <class One>
struct boo<One, char> {};
```
Or 
```cpp
template <class Two>
struct boo<char, Two> {};
```

<!-- end_slide -->
<!-- jump_to_middle -->

Chapter 2: SFINAE - substitution failure is not an error!
===
<!-- end_slide -->

# introducing SFINAE

SFINAE is a technique that allows us to write code that will compile only 
if certain conditions are met.

This allows us to 
<!-- incremental_lists: true -->
* Erite generic code that will work for many types, but not all.
* Catch errors at compile time.
* Create powerful APIs that are flexible and generic.

Let's look at this:
```cpp {1|3-6|8-9|3-6, 11|3-6, 13-14}
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
```

<!-- end_slide -->
# template deduction!

What happens when we call a template function?
When we call a template function, the compiler tries to deduce the template parameters from the arguments
<!-- incremental_lists: true -->
* name lookup: the compiler looks for the function name in the current scope.
* for template functions, the compiler tries to replace all template instances with the parameter passed.
* If the susbsitution is successful, it is added to the overload set.
* If the substitution is not successful, it is not added to the overload set.
* If the overload set is empty, the compiler will throw an error.
* If the overload set is not empty, the compiler will choose the best match.
* If all the overloads are equally good, the compiler will throw an error over ambigous call.

<!-- pause -->
Now that we know let's take another look now:
```cpp
template <typename T>
void foo(T t, typename T::type x) {
     std::cout << "foo is called" << std::endl;
}

ClassWithType c;
foo(c, 5); 
foo(4, 4); 
```

<!-- pause -->
SFINAE means that if the deduction failed for one type, it will continue searching for other possible resolutions until it finds one, but this **only applicable for the function/class declaration**

<!-- pause -->
This is not the same
```cpp
template <typename T>
void foo(T t) {
    T::type x = 5;
    std::cout << x << std::endl;
}
```
<!-- end_slide -->
# using SFINAE

Confused? Me too! Let's look at real SFINAE.. (Now is the hard part finally)

```cpp {1-4 | 6-9 | 12-15 | 18-21 | 24-27 | 31 | 31, 12-15 | 32, 1-4 | 32, 1-4, 12-15 | 33, 6-9 | 6-9, 18-21, 33 | 34 | 34, 24-27}
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
```
<!-- end_slide -->
<!-- jump_to_middle -->

Chapter 3: TYPE TRAITS
===
<!-- end_slide -->

# \#include <type_traits>
Somewhere along the way, we got type traits. 
They are special template stucts that can create a compile type BOOL according to a template parameter
<!-- pause -->

some examples are:
```cpp {1-2|4-5|7-8}
template <typename T>
constexpr bool is_int = std::is_same<T, int>::value; // true if T is int, false otherwise

template <typename T>
constexpr bool is_pointer = std::is_pointer<T>::value; // true if T is a pointer, false otherwise

template <typename T>
constexpr bool is_reference = std::is_base_of<std::string, T>::value; // true if T is inherited from string, false otherwise
```
<!-- pause -->

To understand these traits we must first meet two base structs:
<!-- pause -->
* true_type:
```cpp
typedef std::integral_constant<bool,true> true_type;
```
<!-- pause -->
* false_type:
```cpp
typedef std::integral_constant<bool,false> false_type;
```

<!-- pause -->

They are actually structs of type std::integral_constant<bool, true> and std::integral_constant<bool, false>
a struct that used to provide compile time constants as TYPES!

So std::true_type::value is true and std::false_type::value is false

<!-- pause -->

When you implement a type trait, you must inherit from std::true_type or std::false_type
This will let you call type_trait::value to get the boolean result
You can also call ::type to get the type of the boolean result (std::true_type or std::false_type)

Inside the true_type / false_type class it has a `static constexpr bool value = true/false;`

<!-- end_slide -->
# std::is_same
Let's implement our own type_trait: is_same

```cpp {1-4| 1-9 | 1-15}
template <typename T, typename U>
struct my_is_same : std::false_type {
    //constexpr static bool value = false;
};
 
template <typename T>
struct my_is_same<T, T> : std::true_type {
    //constexpr static bool value = true;
};

//from c++17 every type trait has a _v suffix that returns the value of the type trait
template <typename T, typename U>
constexpr bool my_is_same_v = my_is_same<T, U>::value;
```

<!-- pause -->
Now I can do the following
```cpp
auto func() {return 5;}
int d;
static_assert(my_is_same_v<decltype(func()), int>, "func must return int!");
```

<!-- end_slide -->
# more examples! 
Let's do something more complicated.. 
Our own type traits

<!-- pause -->
## is_pointer_or_reference?
A type trait that tells us if it is either a pointer or a reference
```cpp
template <typename T>
struct is_pointer_or_reference : std::__or_<std::is_reference<T>, std::is_pointer<T>>::type {};
```

<!-- pause -->
## has_c_str?
A type trait that tells if T has a member function c_str() that returns const char*...
```cpp
template <typename T, typename = void>
struct has_c_str : std::false_type {};

template <typename T>
struct has_c_str<T, decltype(std::declval<T>().c_str(), void())>  : std::is_same<decltype(std::declval<T>().c_str()), const char*>::type {};
```

<!-- pause -->
```cpp +exec
int main() {

    int x = 5;
    int& x_ref = x;

    std::cout << std::boolalpha << "is x a pointer or reference?   " << is_pointer_or_reference<decltype(x)>::value << std::endl;
    std::cout << std::boolalpha << "is x_ref a pointer or reference?   " << is_pointer_or_reference<decltype(x_ref)>::value << std::endl;

    std::cout << std::boolalpha << "does int have c_str?   " << has_c_str<int>::value << std::endl; 
    std::cout << std::boolalpha << "does std::string have c_str?   " << has_c_str<std::string>::value << std::endl; 
}
```

<!-- end_slide -->
<!-- jump_to_middle -->

Chapter 4: ENABLE IF
===
<!-- end_slide -->

# std::enable_if
Now let's use our type traits in SFINAE functions!
<!-- pause -->

C++11 gave us a very simple template struct called enable_if.

```cpp {1-2 | 1-7}
template <bool Condition, typename T = void>
struct my_enable_if {};

template <typename T>
struct my_enable_if<true, T> {
    using type = T;
};

```
<!-- pause -->
Look! Specialization! The inner type only exists if the consition is true!.
We can use type traits as conditions!

<!-- end_slide -->
# enable_if has_c_str
Here is a type trait we created before...
```
template <typename T>
constexpr bool has_c_str_v = has_c_str<T>::value;
```
<!-- pause -->

following are 4 different ways we can use the trait in a function that is enabled only if the trait is implemented for the type!

```cpp {0 | 1-6 | 10-14 | 16-21 | 23-27}
// as a default parameter int = 0
template <typename T>
void PrintMe_1(T t, typename my_enable_if<has_c_str<T>::value, int>::type = 0) {
    // collapses to:  void PrintMe_1(T t, int = 0), can we get read of int?
    std::cout << t.c_str() << " 1"<< std::endl;
}


//as a return type void (notice enable_if_t)
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
```
<!-- end_slide -->
<!-- jump_to_middle -->

Chapter 4.5: PRACTICE!!
===
<!-- end_slide -->

# benchmark
Here is a cool benchmarking function.

```cpp
template <typename T, typename... Args>
auto Benchmark(const char* name, T func, Args... args) -> decltype(func(args...)) {
    auto start = std::chrono::system_clock::now();
    auto result = func(args...);
    auto end = std::chrono::system_clock::now();

    std::cout << name << " took " << (end - start).count() << " ms" << std::endl;
    
    return result;
}
```
<!-- pause -->
What does it do??
<!-- incremental_lists: true -->
* Receives a name, function and arguments as paramters.
* Runs the function
* Prints to the screen how much time it took to run the functions
* Returns whatever the benchmarked function returned

<!-- pause -->
```cpp
    int x = Benchmark("test 1", []() { return 5; });
    int y = Benchmark("test 2", [](int a, int b) { return  a + b; }, 10, 30);
    Benchmark("test 1", []() { printf("haha\n"); });
```

Cool.. Does it compile? What is the catch?....
<!-- pause -->

What happens when the argument function returns void?
<!-- pause -->
**error: return-statement with a value, in function returning ‘void’ [-fpermissive]**
<!-- pause -->
We need a way to run this function differently for functions returning value and for functions returning void!!
<!-- end_slide -->
# enable_if returns void/non-void
First let's write the specialization for function returning void:
<!-- pause -->
## enable_if return type is void
```cpp {1-10 | 3 | 4 - 10 | 1-10}
template <typename T, typename... Args>
auto Benchmark(const char* name, T func, Args... args) -> 
  typename std::enable_if<std::is_void<decltype(func(args...))>::value, void>::type 
{
    auto start = std::chrono::system_clock::now();
    func(args...);
    auto end = std::chrono::system_clock::now();

    std::cout << name << " took " << (end - start).count() << " ms" << std::endl;
}
```
<!-- pause -->
This actually turns to 
```cpp
auto Benchmark(const char* name, T func, Args... args) -> void
```
For functions returning void or fails in substitution otherwise..
<!-- pause -->
## enable_if return type is not void
The returning non void looks almost the same:
```cpp {1-12 | 3 | 4-12 | 1-12}
template <typename T, typename... Args>
auto Benchmark(const char* name, T func, Args... args) -> 
  typename std::enable_if<!std::is_void<decltype(func(args...))>::value, decltype(func(args...))>::type 
{
    auto start = std::chrono::system_clock::now();
    auto result = func(args...);
    auto end = std::chrono::system_clock::now();

    std::cout << name << " took " << (end - start).count() << " ms" << std::endl;

    return result;
}
```
<!-- end_slide -->
<!-- jump_to_middle -->

Chapter 5: C++17 -> CONSTEXPR IF and VOID_T
===
<!-- end_slide -->
# constexpr if

When C++17 entered we could finally get rid of a lot of the ugly stuff.
Now we can run some compile time checks on our types with `constexpr if`

<!-- pause -->
Let's return to our has_c_str and grut example:
<!-- pause -->
```cpp {1-6 | 7-14 | 7-25 | 16-32 | 28, 20-21 | 29, 20-21 | 30, 18-19 | 31, 22-23}
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
    const char* talk() const { return "I am Grut"; }
};

template <typename T>
void PrintMe(T t) {
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
```
<!-- end_slide -->
# std::void_t
Another thing we got is `void_t` which is a super simple `utility metafunction` that helps us implement type traits.

It looks like this:
<!-- pause -->
```cpp
template <class... >
using void_t = void;
```

<!-- pause -->
What happens here?.. It takes anything as it's argument and returns void...
Can you guess how it helps us?
<!-- pause -->
Well... It is just another dirty SFINAE trick for template instantiation-deduction check...

<!-- pause -->
Some examples:
## Class T has `typename type` (typename T::type exists)
```cpp {1-6 | 1-2 | 5-6 | 1-6}
template<class T, class = void>
struct has_type_member : std::false_type {};
 
// specialization recognizes types that do have a nested ::type member:
template<class T>
struct has_type_member<T, std::void_t<typename T::type>> : std::true_type {};
```
<!-- pause -->

## Class T has pre increment operator (++T)
```cpp {1-8 | 1-2 | 4-8 | 1-8}
template<class, class = void>
struct has_pre_increment_member : std::false_type {};
 
// specialization recognizes types that do support pre-increment:
template<class T>
struct has_pre_increment_member<T,
           std::void_t<decltype( ++std::declval<T&>() )>
       > : std::true_type {};
```
<!-- end_slide -->
<!-- jump_to_middle -->

Chapter 6: CONCEPTS
===
<!-- end_slide -->
# defining concepts

in C++20 we have a new concept called "concepts" which is a way to express **constraints** on template parameters.
<!-- pause -->
It means that we can forget about 85% of everything we learned about SFINAE and type traits and just use concepts instead
<!-- pause -->

concepts are in the end some kind of compile time bool according to some compile time condition,
very much like type traits, but has better syntax and are more powerful.
<!-- pause -->
Let's define a concept that says that a type has a c_str() method that returns a const char*
we can use predefined concepts inside our own concepts like std::same_as, std::convertible_to, std::derived_from, etc.

<!-- pause -->
## concept HasCStr
```cpp
template <typename T>
concept HasCStr = requires(T t) {
    { t.c_str() } -> std::same_as<const char*>;
    //{ t.c_str() } -> std::convertible_to<const char*>;
};
```

<!-- pause -->
please note that std::same_as is not same as std::is_same (shit!)
std::same_as is a concept while std::is_same is a type_trait
<!-- pause -->

<!-- end_slide -->
# using concepts

Now let's see how we can use this concept in a function

<!-- pause -->
## using `requires` clause
```cpp
template <class T>
requires HasCStr<T>
void PrintMe(T t) {
    std::cout << t.c_str() << std::endl;
}
```

<!-- pause -->
## template arg line syntax
```cpp
template <HasCStr T>
void PrintMe_2(T t) {
    std::cout << t.c_str() << std::endl;
}
```

<!-- pause -->
## constrained auto syntax
```cpp
void PrintMe_3(HasCStr auto t) {
    std::cout << t.c_str() << std::endl;
}
```

<!-- pause -->
Above three functions are exactly the same!!!

<!-- end_slide -->
# the power of concpets
Concepts are a very powerful tool.
Togather with constexper/consteval they are a huge step for compile time programming.

<!-- pause -->
With concepts you don't need to pass a type to a function, 
you can pass any thing that is convertible to that type or only what the type should do
and create more generic code in compile time.

```cpp {1-19 | 8-10 | 1-6 | 1-21 }
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
    AddToJson("one", 1);

    AddToJson("two", "two");
    AddToJson("three", nullptr);
    AddToJson("four", nullptr);

    std::string four = "four";
    AddToJson("four", four);
}
```
<!-- pause -->
Another part of the magic is that if we fail to match the constraints, we get a much clearer compiler error than with usual template failures.

<!-- end_slide -->
# duck typing

```
< C++ CONCEPTS ARE STILL DUCK TYPED! >
 ------------------------------------
 \
  \
   \ >()_
      (__)__ _
```

<!-- pause -->
What is duck typing?
Why is duck typing a problem?

<!-- end_slide -->
<!-- jump_to_middle -->

Chapter 7: RUST!!!!!!!!!!!!!
===
<!-- end_slide -->

# rust traits

Rust is a new (compared to C++) programming language that aims to be as efficient as C++ without all the C inherited problems.
<!-- pause -->
One of the main features of rust is the trait system that is much like the C++ concept system but not duck typed.
<!-- end_slide -->
Here is some rust primer code
```rust {1-41 | 1-3 | 5-14 | 16-18 | 20-31 | 16-18, 33-35 | 37-40}
struct StringStruct {
    string: [u8; 80],
}

impl StringStruct {
    fn new(val: &'static str) -> StringStruct {
        let mut string = [0; 80];
        for (i, c) in val.bytes().enumerate() {
            string[i] = c;
        }

        StringStruct { string }
    }
}

trait ToStr {
    fn c_str<'a>(&'a self) -> &'a str;
}

impl ToStr for StringStruct {
    fn c_str<'a>(&'a self) -> &'a str {
        let mut len = 0;
        for i in 0..80 {
            if self.string[i] == 0 {
                break;
            }
            len += 1;
        }
        unsafe { core::str::from_utf8_unchecked(&self.string[..len]) }
    }
}

fn print_me(s: impl ToStr) {
    println!("{}", s.c_str());
}

fn main() {
    let s = StringStruct::new("Hello World");
    print_me(s);
}
```
<!-- end_slide -->
<!-- jump_to_middle -->

QUESTIONS?
===
