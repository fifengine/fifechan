include_guard(GLOBAL)

include(CMakeParseArguments)

function(fifechan_install_example example_name)
  set(options)
  set(oneValueArgs)
  set(multiValueArgs TARGETS DIRECTORIES FILES)
  cmake_parse_arguments(FIFECHAN_EXAMPLE "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  set(example_destination "${FIFECHAN_EXAMPLE_INSTALL_BASE}/${example_name}")

  if(FIFECHAN_EXAMPLE_TARGETS)
    install(TARGETS ${FIFECHAN_EXAMPLE_TARGETS}
      RUNTIME DESTINATION "${example_destination}"
    )

    foreach(example_target IN LISTS FIFECHAN_EXAMPLE_TARGETS)
      set_property(TARGET ${example_target} APPEND PROPERTY INSTALL_RPATH "\$ORIGIN/../../../../${CMAKE_INSTALL_LIBDIR}")

      if(WIN32)
        install(FILES $<TARGET_RUNTIME_DLLS:${example_target}>
          DESTINATION "${example_destination}"
        )
      endif()
    endforeach()
  endif()

  foreach(example_directory IN LISTS FIFECHAN_EXAMPLE_DIRECTORIES)
    install(DIRECTORY "${example_directory}"
      DESTINATION "${example_destination}"
    )
  endforeach()

  foreach(example_file IN LISTS FIFECHAN_EXAMPLE_FILES)
    install(FILES "${example_file}"
      DESTINATION "${example_destination}"
    )
  endforeach()
endfunction()
