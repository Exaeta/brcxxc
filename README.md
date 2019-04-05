# Binary Resources for C++ Compiler (BRCC)

Version 1.2.0.

The Binary Resources for C++ Compiler is a simple cross platform compiler that
converts binary files to C++ header and source files for inclusion in other 
projects.

I found other solutions to this problem deficient. xxd doesn't allow you to
set the name of the variables to anything other than the files name. objdump,
ld, both have the same issues in general.

BRCC is a cross platform, more predictable solution. It allows you to compile 
as many binaries into a single output .cpp file as you want, and creates a
corresponding .hpp file. You can manually set the header guard and it doesn't 
gum up your code with autogeneration comments.

Usage: brcc output_basename HEADER_GUARD [(-c|-i|-s|-v) symbol_prefix binary_file]...

Example: 

    brcc ./build/gen/binary_resources GAME_BINARY_RESOURCES_HH -s bin_vertex_shader ./src/vertex_shader.glsl \
    bin_fragment_shader ./src/fragment_shader.glsl

Creates files './build/gen/binary_resources.hpp' and './build/gen/binary_resources.cpp', which have std::strings that import the data.

Options:

    * `-s` Stores the binary data in a `std::string`.
    * `-c` Stores the binary data in a C style array.
    * `-i` Stores the binary data in a `std::initializer_list<std::uint8_t>`.
    * `-v` Stores the binary data in a `std::vector<std::uint8_t>`.

The output `test-brcc.cpp` file after I ran `brcc ./test-brcc GUARD_HH bin_test ~/test_binary1.bin arbitrary_symbol /etc/issue`:

```
#ifndef GUARD_HH
#define GUARD_HH
#include <cstdint>

extern std::uint8_t const arbitrary_symbol[30];
extern std::uint8_t const bin_test[13];

#endif
std::uint8_t const arbitrary_symbol[30] = {
  0x4c, 0x69, 0x6e, 0x75, 0x78, 0x20, 0x4d, 0x69, 0x6e, 0x74, 0x20, 0x31, 0x38, 0x2e, 0x33, 0x20,
  0x53, 0x79, 0x6c, 0x76, 0x69, 0x61, 0x20, 0x5c, 0x6e, 0x20, 0x5c, 0x6c, 0x0a, 0x0a
};
std::uint8_t const bin_test[13] = {
  0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x77, 0x6f, 0x72, 0x6c, 0x64, 0x21, 0x0a
};

```
