message(STATUS "---------------------------------------------------------------")
message(STATUS "[ 🛠️ ] Building Examples")
message(STATUS "---------------------------------------------------------------")

add_subdirectory(ff)

# Example: fps
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/fps/CMakeLists.txt")
  add_subdirectory(fps)
else()
  message(STATUS "[INFO] Skipping example 'fps': no CMakeLists.txt found.")
endif()

add_subdirectory(simple)

add_subdirectory(spacers)
