## TODO

- [ ] include/util/utf8/utf8 is a third-party dependency and needs a vcpkg replacement
- [ ] tests/integration adjust install locations to respect relative packaging (${CMAKE_INSTALL_INCLUDEDIR})
- [x] enable build of tests/integration/opengl-sdl + refactor
- [ ] include/fifechan/keys.hpp: has an enum with -1000 init which causes tidy warnings
      - [ ] should it be refactored to a class enum?

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

#### cppcoreguidelines-macro-usage

The warning suggests to use constexpr instead of macros.
This is not always possible, e.g. for platform detection macros.
The following macros are used in the codebase and should be ignored:

- [x] add nolint for these macros to suppress the warnings
  - [x] platform.hpp:14 - FIFEGUI_OS_LINUX
  - [x] platform.hpp:25 - FIFEGUI_COMPILER_GNU
  - [x] platform.hpp:28 - FIFEGUI_COMPILER_CLANG
  - [ ] utf8/core.hpp:160 - UTF8_CPP_INCREASE_AND_RETURN_ON_ERROR (external lib)

#### cppcoreguidelines-owning-memory

The warning suggests to use smart pointers instead of raw pointers for owning memory.
This change is not trivial. It requires checking of ownership semantics and
potential refactoring of code towards RAII and value semantics.
Which also means to introduce move semantics in some places.
Number of reported issues: 62

- [ ] use smart pointers instead of raw pointers for owning memory

