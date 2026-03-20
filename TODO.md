## TODO

- [ ] include/util/utf8/utf8 is a third-party dependency and needs a vcpkg replacement
- [ ] tests/integration adjust install locations to respect relative packaging (${CMAKE_INSTALL_INCLUDEDIR})
- [x] enable build of tests/integration/opengl-sdl + refactor
- [ ] include/fifechan/keys.hpp: has an enum with -1000 init which causes tidy warnings
      - [ ] should it be refactored to a class enum?
- [ ] sync demonstrated features of tests/integration/opengl-sdl with tests/integration/sdl (has ttf rendering)

### Website

- [ ] update website
  - [ ] replicate intro text from README.md
  - [ ] add link to the API documentation (/api)

### Doxygen

- [ ] work on build-docs.yml
  - [ ] push doxygen html to gh-pages branch, into subfolder /api
  - [ ] auto publish api docs when new tag is pushed
    - [ ] but allow triggering an update manually (e.g. via on workflow_dispatch)

### Clang Tidy

#### Issues Summary

**Total Issues: 84**

| Done | Issue Category | # Issues | Severity |
|------|----------------|----------|----------|
| [ ] | cppcoreguidelines-owning-memory | 48 | high |
| [ ] | cppcoreguidelines-pro-bounds-pointer-arithmetic | 7 | high |
| [ ] | cppcoreguidelines-pro-type-vararg | 6 | high |
| [ ] | cppcoreguidelines-pro-type-reinterpret-cast | 4 | high |
| [ ] | bugprone-inc-dec-in-conditions | 4 | high |
| [ ] | bugprone-unused-return-value | 1 | high |
| [ ] | bugprone-empty-catch | 1 | high |
| [ ] | cppcoreguidelines-special-member-functions | 4 | mid |
| [ ] | cppcoreguidelines-pro-type-member-init | 3 | mid |
| [ ] | cppcoreguidelines-macro-usage | 1 | mid |
| [ ] | cppcoreguidelines-prefer-member-initializer | 2 | low |
| [ ] | readability-enum-initial-value | 1 | low |
| [ ] | readability-else-after-return | 1 | low |
| [ ] | performance-enum-size | 1 | low |

#### cppcoreguidelines-owning-memory

The warning suggests to use smart pointers instead of raw pointers for owning memory.
This change is not trivial. It requires checking of ownership semantics and
potential refactoring of code towards RAII and value semantics.
Which also means to introduce move semantics in some places.

- [ ] use smart pointers instead of raw pointers for owning memory

#### cppcoreguidelines-macro-usage

The warning suggests to use constexpr instead of macros.
This is not always possible, e.g. for platform detection macros.
The following macros are used in the codebase and should be ignored:

- [x] add nolint for these macros to suppress the warnings
  - [x] platform.hpp:14 - FIFEGUI_OS_LINUX
  - [x] platform.hpp:25 - FIFEGUI_COMPILER_GNU
  - [x] platform.hpp:28 - FIFEGUI_COMPILER_CLANG
  - [ ] utf8/core.hpp:160 - UTF8_CPP_INCREASE_AND_RETURN_ON_ERROR (external lib)
