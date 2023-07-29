# HYPED 2023

![Logo](https://uploads-ssl.webflow.com/5e88b996ec626f7dad28c40f/5eda270ec8019d2f43ed1b39_New-Logo-2020-(transparent-background).png)

![Build Shield](https://github.com/Hyp-ed/hyped-2023/actions/workflows/build.yml/badge.svg) ![TODO Shield](https://img.shields.io/github/search/hyp-ed/hyped-2023/TODOLater?color=red&label=TODO%20counter)

## Dependencies

- `clang`
- `clang-format`
- `CMake >= 3.12`
- `eigen3`
- `boost`
- `rapidjson`

## Usage

  To pepare your project:

  ```
  ./scripts/setup
  ```

  To create the build files:

  ```
  mkdir build
  cd build
  cmake ..
  ```

  To build all the binaries (in `./build`):

  ```
    make -j
  ```

  To run tests (in `./build`):

  ```
  ctest
  ```

## Contributing

  All the C++ code should follow the [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html).

  Further, we aim to maintain the following rules which are based on the [NASA's Ten Rules for Safety Critical Coding](https://pixelscommander.com/wp-content/uploads/2014/12/P10.pdf).

  1. All loops must have a fixed upper-bound. It must be trivially possible for a checking tool to prove statically that a preset upper-bound on the number of iterations of a loop cannot be exceeded. If the loop-bound cannot be proven statically, the rule is considered violated.
  2. Do not use dynamic memory allocation after initialisation.
  3. No function should be longer than 60 lines of code.
  4. The assertion density of the code should average to a minimum of two assertions per function. Assertions are used to check for anomalous conditions that should never happen in real-life executions. Assertions must always be side-effect free and should be defined as Boolean tests. When an assertion fails, an explicit recovery action must be taken, e.g., by returning an error condition to the caller of the function that executes the failing assertion. Any assertion for which a static checking tool can prove that it can never fail or never hold violates this rule. (I.e., it is not possible to satisfy the rule by adding unhelpful “assert(true)” statements.)
  5. Data objects must be declared at the smallest possible level of scope.
  6. The return value of non-void functions must be checked by each calling function,  and the validity of parameters must be checked inside each function.
  7. The use of the preprocessor must be limited to the inclusion of header files and  compile time arguments. (e.g. operating system and architecture)
  8. The use of pointers should be restricted. In particular, the keywords `new` and `delete`  shall never be used and raw pointers shall only be viable as an alternative to `std::optional<T&>`, which is ambiguous and therefore an illegal type.
  9. All code must be compiled, from the first day of development, with all compiler warnings enabled  at the compiler’s most pedantic setting. All code must compile with these setting without any warnings. All code must be checked daily with at least one, but preferably more than one, state-of-the-art static  source code analyzer and should pass the analyses with zero warnings.