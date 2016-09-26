
# 
# Executable name and options
# 


# Target name
set(testmodule deepnetvideo)
set(target ${testmodule}-test)
message(STATUS "Test ${target}")


find_package(OpenCV 3)
if ( NOT OpenCV_FOUND )
  message(WARNING "Package OpenCV required, but not found!")
  return()
endif(NOT OpenCV_FOUND )

find_package(dlib)
if ( NOT dlib_FOUND )
  message(WARNING "Package dlib required, but not found!")
  return()
endif(NOT dlib_FOUND )

find_library(deepnetvideo_FOUND ${META_PROJECT_NAME}::${testmodule})
if ( NOT deepnetvideo_FOUND )
  message(WARNING ${testmodule} " required, but not found! Just build and then rerun cmake and rebuild after.")
  return()
endif(NOT deepnetvideo_FOUND )

# 
# Sources
# 

set(sources
    main.cpp
    ${testmodule}_test.cpp
)


# 
# Create executable
# 

# Build executable
add_executable(${target}
    ${sources}
)

# Create namespaced alias
add_executable(${META_PROJECT_NAME}::${target} ALIAS ${target})


# 
# Project options
# 

set_target_properties(${target}
    PROPERTIES
    ${DEFAULT_PROJECT_OPTIONS}
    FOLDER "${IDE_FOLDER}"
)


# 
# Include directories
# 

target_include_directories(${target}
    PRIVATE
    ${DEFAULT_INCLUDE_DIRECTORIES}
    ${PROJECT_BINARY_DIR}/source/include
)


# 
# Libraries
# 

target_link_libraries(${target}
    PRIVATE
    ${DEFAULT_LIBRARIES}
    ${META_PROJECT_NAME}::${testmodule}
    libgmock
)


# 
# Compile definitions
# 

target_compile_definitions(${target}
    PRIVATE
    ${DEFAULT_COMPILE_DEFINITIONS}
)


# 
# Compile options
# 

target_compile_options(${target}
    PRIVATE
    ${DEFAULT_COMPILE_OPTIONS}
)


# 
# Linker options
# 

target_link_libraries(${target}
    PRIVATE
    ${DEFAULT_LINKER_OPTIONS}
)
