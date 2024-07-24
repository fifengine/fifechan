#
# Copyright 2024 Jens A. Koch.
# SPDX-License-Identifier: MIT
# This file is part of fifengine/fifechan.
#

#-----------------------------------------------------------------------------
# Function to copy resources
#
# Usage:
# set(IMAGE_SOURCE      "${PROJECT_SOURCE_DIR}/tests/resources/fixedfont.bmp")
# set(IMAGE_DESTINATION "$<TARGET_FILE_DIR:sdlhelloworld>/fixedfont.bmp")
# copy_resources(sdlhelloworld "${IMAGE_SOURCE}" "${IMAGE_DESTINATION}")
# install(FILES "${IMAGE_SOURCE}" DESTINATION "${CMAKE_INSTALL_BINDIR}")
#
# Reminder: do not forget the install step.
#
#-----------------------------------------------------------------------------
function(copy_resources target res_src res_dest)
    # Add a custom command to copy the resource file to the destination directory
    add_custom_command(
        TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --cyan "Copying resources for '${target}'"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${res_src}"
            "${res_dest}"
        COMMENT "Copying resource file ${res_src} => ${res_dest}"
        VERBATIM
    )

    # Add a custom target to ensure the resource file is copied before the target is built
    set(copy_target_name "${target}_copy_resources")
    add_custom_target(${copy_target_name} ALL DEPENDS ${res_src})
    add_dependencies(${target} ${copy_target_name})
endfunction(copy_resources)
