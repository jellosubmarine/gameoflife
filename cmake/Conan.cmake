macro(run_conan)
# Download automatically, you can also just copy the conan.cmake file
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message(
    STATUS
      "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
  file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/release/0.17/conan.cmake"
       "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_run(
  REQUIRES
  ${CONAN_EXTRA_REQUIRES}
  # catch2/2.11.0
  ogg/1.3.4@madis/local
  zlib/1.2.11@madis/local
  flac/1.3.3@madis/local
  libpng/1.6.37@madis/local  
  docopt.cpp/0.6.3
  # fmt/6.2.1
  spdlog/1.8.5
  magic_enum/0.6.6
  imgui-sfml/2.5@bincrafters/stable
  glew/2.2.0
  eigen/3.3.7
  # yaml-cpp/0.6.3
  # nlohmann_json/3.7.3
  OPTIONS
  ${CONAN_EXTRA_OPTIONS}
  BASIC_SETUP
  CMAKE_TARGETS # individual targets to link to
  BUILD
  missing)
endmacro()
