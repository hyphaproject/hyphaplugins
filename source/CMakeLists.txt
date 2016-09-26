
# 
# Configuration for all sub-projects
# 

# Generate version-header
string(TOUPPER ${META_PROJECT_NAME} META_PROJECT_NAME_UPPER)
configure_file(version.h.in ${CMAKE_CURRENT_BINARY_DIR}/include/${META_PROJECT_NAME}/${META_PROJECT_NAME}-version.h)

ADD_DEFINITIONS(
    -std=c++11 # Or -std=c++0x
    # Other flags
)
# 
# Sub-projects
# 

# Libraries
set(IDE_FOLDER "")
add_subdirectory(dht11)
add_subdirectory(deepnetvideo)
add_subdirectory(email)
add_subdirectory(espeak)
add_subdirectory(explorenfc)
add_subdirectory(fingerprintzfm)
add_subdirectory(flame)
add_subdirectory(lightsensor)
add_subdirectory(pythonplugin)
add_subdirectory(rfid)
add_subdirectory(rpianalogsensor)
add_subdirectory(rpigpio)
add_subdirectory(rpiled)
add_subdirectory(rpipir)
add_subdirectory(video)
add_subdirectory(watersensor)
add_subdirectory(wifi)

# Examples
set(IDE_FOLDER "Examples")
add_subdirectory(examples)

# Tests
set(IDE_FOLDER "Tests")
add_subdirectory(tests)


# 
# Deployment
# 

# Deploy generated headers
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/${META_PROJECT_NAME} DESTINATION include COMPONENT dev)
