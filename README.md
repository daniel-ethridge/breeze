# Breeze

The goal of this library is to help make coding a breeze through supplying libraries, templated classes and templated
methods that address coding difficulties associate with more obscure C++ concepts.

## Building

This library uses CMake as a build system, though in its infancy it does not need to be built to be used. You only need
to build if you want to run the tests. To run the tests (or at this point test), from the root directory, run:
```
  mkdir build
  cd build
  cmake ..
  make
  ctest
  ```

## Current modules
### Bitflags

```#include <breeze/bitflags.h```

Bit flags are a useful tool to represent multiple boolean values or states in a single integer. They are commonly
created using enums, but enums are an unscoped type. Often enum classes are preferred due to being a scoped type, but
this makes it a bit trickier to use them as Bitflags. <i>Breeze</i> includes a templated class called
BitFlags to make creating them simple. Check out the documentation (coming soon) for more information!
