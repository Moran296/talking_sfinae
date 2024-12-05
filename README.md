# talking_sfinae
A presentation regarding template metaprogramming and sfinae - past, present and future.

Here it is presented at TandemG in Hebrew
https://youtu.be/UibvKYw4IDA

This is a short talk regarding SFINAE and template deduction from C++11 to C++20 (and some RUST).
Prior knowledge of CPP is assumed.

disclaimer: This is not a tutorial/guide. It is not written to be fully comprehensive only by reading.
I use this code while talking and explaining to illustrate ideas.

## running the slides:
There is a possibility of running this talk as a presentation in the terminal

To run the slides in the terminal one needs the next things:
- cowsay (`sudo apt install cowsay`)
- presenterm (`cargo install --locked presenterm`)
- nothing else to do for like 1.5 hours

This is how you run it then:
`presenterm -x slides.md`

## one.cpp
New CPP abilites required for understanding later concepts

## two.cpp
What is SFINAE, template deduction and using decltype to detect and sfinae

## three.cpp
Type traits

## four.cpp
enable_if, using type traits and enable if for sfinae

## four_and_a_half.cpp
A small exercise to show how to use enable_if to sfinae a function

## five.cpp
if constexper, using it instead enable_if

## six.cpp
concepts, requires

## seven
rust and rust traits











