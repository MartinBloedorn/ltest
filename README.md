# L_TEST  

`L_TEST` is a minimalistic and simple Unit-Test framework, designed with bare-metal embedded targets in mind. 

<p align="center">
<img src="https://github.com/MartinBloedorn/ltest/blob/master/examples/screen0.png" alt="What it looks like" width="600">
</p>

This implementation is heavily inspired by __Martin Dørum's__ great work on [Snow](https://github.com/mortie/snow), check it out.
I've essentially implemeneted a very stripped down version of it, both as an exercise and as a means to meet my immediate needs on an ongoing project.

# Using it

## Setup

Add `l_test.h` and `l_test.c` to your project. 
In `l_test.h`, supply a `printf` compliant function, as well as a fuction to measure system uptime in millseconds (i.e., an Arduino-like `millis()`):

```c
    /// Supply your printf compliant function
    #define __L_PRINTF(...)    printf(__VA_ARGS__)

    // Supply a function to return a wall time (in ms)
    #define __L_MILLIS         millis
```  
    
## Defining tests

Test cases are grouped in modules, which kind of follows the syntax format in [Snow](https://github.com/mortie/snow).
Define a module, and within it, appropriate test cases. For example:

```c
    int add(int a, int b) {
      return a + b;
    }

    L_TEST_MODULE(simple_test, "Test routines for some basic functions.") {

      L_TEST_CASE("Simple integer math") {
        int a = 3, b = 2;

        L_TEST_ASSERT(add(a, b) > a);
        L_TEST_ASSERTEQ_INT(add(a, b), 5);
      }
    }
```

## Enable and run 

Somewhere in your project (global scope `#define`, or with a `-D` compilation flag), define `L_UNIT_TESTS`. 

Then, execute `l_test_main()`. That's it. 

## Macros

#### L_TEST_MODULE(mname, mdesc)

Define a test module, with a name and a string description. Test cases should be defined within its body.

#### L_TEST_CASE(desc)

Define a test case with a description. Test cases can only be defined withen a L_TEST_MODULE body. 

#### L_TEST_ASSERT(val)

Asserts that the value is non-zero. If the assertion fails, an error message is printed and the test case is aborted.

#### L_TEST_ASSERTN(val)

Asserts that the value is zero. If the assertion fails, an error message is printed and the test case is aborted.

#### L_TEST_ASSERTEQ_INT(val, ref)

Asserts that `val == ref`, assuming integer values. 
If the assertion fails, the expected and current values will be printed, and the test case will be aborted.

#### L_TEST_ASSERTEQ_FLT(val, ref)

Asserts that the [RPD](https://en.wikipedia.org/wiki/Relative_change_and_difference) between `val` and `ref` is less than 0.1%. 
If the assertion fails, the expected and current values will be printed, and the test case will be aborted.

#### L_TEST_ASSERTEQ_STR(val, ref)

Executes a `strcmp` between the two supplied strings. 
If the assertion fails, the expected string and the supplied string are printed.

#### L_TEST_ASSERTEQ_BUF(val, ref, size)

Executes a `memcmp` between the two supplied buffers. If the assertion fails, an error message is printed and the test case is aborted.

# Prerequisites and caveats

In its current form, this library does require a `printf` compliant call. Support for floating point prints is not assumed. 

Also, the library makes use of two GCC extensions, `__attribute__((unused))` and `__attribute__((constructor))`, 
so make sure that these are available on your system. 

Lastly, the constructor calls link the test modules in a queue for execution, which means that, AFAIK, 
you can't fully define the order in which modules will be run. 
