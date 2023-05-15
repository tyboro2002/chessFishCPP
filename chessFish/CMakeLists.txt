# CMakeList.txt : CMake project for chessFish, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (chessFish "chessFish.cpp" "chessFish.h" "thc.h" "thc.cpp" "chessEngine.cpp" "chessEngine.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET chessFish PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
