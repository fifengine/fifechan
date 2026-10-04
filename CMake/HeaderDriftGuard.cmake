include_guard(GLOBAL)

# Verifies that the hand-maintained umbrella header includes every header of the
# given list. Ensures that a new widget cannot be built and installed while being
# unreachable via #include <fifechan.hpp>.
#
# fifechan_check_umbrella_header(<umbrella header> <header> [<header> ...])
#
# Each header path is relative to the source directory, i.e. starts with
# "include/", and is matched verbatim against the "#include <...>" lines of the
# umbrella header. Fails the configure step if either list has an entry the other
# one lacks.
function(fifechan_check_umbrella_header umbrella_header)
  if(NOT EXISTS "${umbrella_header}")
    message(FATAL_ERROR "Umbrella header not found: ${umbrella_header}")
  endif()

  set(_declared "${ARGN}")
  if(NOT _declared)
    message(FATAL_ERROR "fifechan_check_umbrella_header: no headers given for ${umbrella_header}")
  endif()

  file(READ "${umbrella_header}" _umbrella_contents)
  string(REGEX MATCHALL "#include <fifechan/[^>]+>" _umbrella_includes "${_umbrella_contents}")

  # The umbrella header spells its includes relative to include/, the given list
  # relative to the source directory, so drop the leading include/ to compare.
  set(_expected "")
  foreach(_header IN LISTS _declared)
    if(NOT _header MATCHES "^include/")
      message(FATAL_ERROR
        "Header path must be relative to the source directory and start with "
        "'include/': ${_header}")
    endif()
    string(REGEX REPLACE "^include/" "" _include "${_header}")
    list(APPEND _expected "#include <${_include}>")
  endforeach()

  set(_missing "")
  set(_stale "")
  foreach(_include IN LISTS _expected)
    list(FIND _umbrella_includes "${_include}" _pos)
    if(_pos EQUAL -1)
      string(REGEX REPLACE "^#include <|>$" "" _include "${_include}")
      string(APPEND _missing "\n    ${_include}")
    endif()
  endforeach()
  foreach(_include IN LISTS _umbrella_includes)
    list(FIND _expected "${_include}" _pos)
    if(_pos EQUAL -1)
      string(REGEX REPLACE "^#include <|>$" "" _include "${_include}")
      string(APPEND _stale "\n    ${_include}")
    endif()
  endforeach()

  if(NOT _missing AND NOT _stale)
    return()
  endif()

  get_filename_component(_umbrella_name "${umbrella_header}" NAME)
  set(_details "")
  if(_missing)
    string(APPEND _details "\n  built and installed, but missing from ${_umbrella_name}:${_missing}")
  endif()
  if(_stale)
    string(APPEND _details "\n  included by ${_umbrella_name}, but not in the given header list:${_stale}")
  endif()
  message(FATAL_ERROR "${_umbrella_name} and its header list disagree:${_details}")
endfunction()

# Verifies that every header in a source directory is listed, so a header cannot
# exist and be built while never being installed.
#
# fifechan_check_header_coverage(<include dir> LIST <list name> [<list name> ...]
#                                HEADERS <header> [<header> ...]
#                                [EXCLUDE <header> [<header> ...]])
#
# HEADERS are paths relative to the source directory, i.e. starting with
# "include/". Headers under include/fifechan/backends/ are skipped: they are
# installed through the extension file sets, not the core one.
function(fifechan_check_header_coverage include_dir)
  cmake_parse_arguments(PARSE_ARGV 1 _fifechan "" "" "LIST;HEADERS;EXCLUDE")

  if(_fifechan_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR
      "fifechan_check_header_coverage: unexpected arguments: ${_fifechan_UNPARSED_ARGUMENTS}")
  endif()
  if(NOT _fifechan_LIST OR NOT _fifechan_HEADERS)
    message(FATAL_ERROR
      "fifechan_check_header_coverage: LIST and HEADERS are required for ${include_dir}")
  endif()

  file(GLOB_RECURSE _on_disk
    LIST_DIRECTORIES false
    "${include_dir}/*.hpp"
    "${include_dir}/*.h"
  )

  set(_unlisted "")
  foreach(_path IN LISTS _on_disk)
    # Backend headers belong to the extension file sets, not the core one.
    if(_path MATCHES "/fifechan/backends/")
      continue()
    endif()
    file(RELATIVE_PATH _header "${PROJECT_SOURCE_DIR}" "${_path}")
    if(NOT _header MATCHES "^include/")
      message(FATAL_ERROR
        "Header path must be relative to the source directory and start with "
        "'include/': ${_header}")
    endif()
    list(FIND _fifechan_HEADERS "${_header}" _pos)
    if(_pos EQUAL -1 AND NOT _header IN_LIST _fifechan_EXCLUDE)
      string(APPEND _unlisted "\n    ${_header}")
    endif()
  endforeach()

  if(NOT _unlisted)
    return()
  endif()

  message(FATAL_ERROR
    "These headers are in ${include_dir} but in no header list, so they build but are never installed.\n"
    "Consumers of the installed library cannot include them:${_unlisted}\n"
    "  Add them to one of: ${_fifechan_LIST}\n"
    "  Or pass them as EXCLUDE if they are intentionally private.")
endfunction()
