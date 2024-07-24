## TODO

- [ ] include/fifechan/version.hpp -> template for configure_file, insert version data from vcpkg.json
- [ ] include/util/utf8/utf8 is a third-party dependency and needs a vcpkg replacement
- [ ] tests/integration also needs resource installation, e.g. of font or images
  - create helper function copy_resources(target, res-src, res-dest)
  - where target refers to the parent_target and automatically add_dependency()
- [ ] tests/integration adjust install locations to respect relative packaging (${CMAKE_INSTALL_INCLUDEDIR})


