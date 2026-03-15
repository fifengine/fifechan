include_guard(GLOBAL)

include(CMakeParseArguments)

function(fifechan_install_example example_name)
  set(options)
  set(oneValueArgs)
  set(multiValueArgs TARGETS DIRECTORIES FILES WINDOWS_RUNTIME_FILES)
  cmake_parse_arguments(FIFECHAN_EXAMPLE "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  set(example_destination "${FIFECHAN_EXAMPLE_INSTALL_BASE}/${example_name}")

  if(FIFECHAN_EXAMPLE_TARGETS)
    install(TARGETS ${FIFECHAN_EXAMPLE_TARGETS}
      RUNTIME DESTINATION "${example_destination}"
    )

    foreach(example_target IN LISTS FIFECHAN_EXAMPLE_TARGETS)
      set_property(TARGET ${example_target} APPEND PROPERTY INSTALL_RPATH "\$ORIGIN/../../../../${CMAKE_INSTALL_LIBDIR}")

      if(WIN32 AND BUILD_SHARED_LIBS)
        add_custom_command(TARGET ${example_target} POST_BUILD
          # Ensure runtime dependencies (for example freetype.dll and libpng16.dll)
          # are available next to each built example executable on Windows.
          COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:${example_target}>
            $<TARGET_FILE_DIR:${example_target}>
          COMMAND_EXPAND_LISTS
        )

        install(FILES $<TARGET_RUNTIME_DLLS:${example_target}>
          DESTINATION "${example_destination}"
        )

        if(FIFECHAN_EXAMPLE_WINDOWS_RUNTIME_FILES)
          add_custom_command(TARGET ${example_target} POST_BUILD
            # Some imported libraries flatten transitive runtime DLLs into raw paths,
            # which TARGET_RUNTIME_DLLS cannot discover. Allow examples to pass those explicitly.
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
              ${FIFECHAN_EXAMPLE_WINDOWS_RUNTIME_FILES}
              $<TARGET_FILE_DIR:${example_target}>
            COMMAND_EXPAND_LISTS
          )

          install(FILES ${FIFECHAN_EXAMPLE_WINDOWS_RUNTIME_FILES}
            DESTINATION "${example_destination}"
          )
        endif()
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
