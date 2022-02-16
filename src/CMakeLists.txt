add_executable(gameoflife_cuda
    main.cpp
    options.cpp
    scenes/full_screen_opengl.cpp
    # scenes/full_screen_opengl.cu
)
target_compile_options(gameoflife_cuda PRIVATE                        
    $<$<COMPILE_LANGUAGE:CUDA>:--std=c++17 -lineinfo --expt-relaxed-constexpr>
)
set_target_properties(gameoflife_cuda PROPERTIES CUDA_SEPARABLE_COMPILATION ON)

if(OpenMP_CXX_FOUND)
    target_link_libraries(gameoflife_cuda PUBLIC OpenMP::OpenMP_CXX)
endif()

target_include_directories(gameoflife_cuda SYSTEM PUBLIC ${CUDA_INCLUDE_DIRS})
target_link_libraries(gameoflife_cuda PRIVATE project_options project_warnings ${CUDA_LIBRARIES})

target_link_libraries_system(
  gameoflife_cuda PRIVATE  
  CONAN_PKG::docopt.cpp
  CONAN_PKG::fmt
  CONAN_PKG::spdlog
  CONAN_PKG::imgui-sfml
  CONAN_PKG::glew
  CONAN_PKG::eigen
)

