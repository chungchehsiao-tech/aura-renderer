cmake_minimum_required(VERSION 3.18)
# Enabling CUDA 
project(AuraRenderer LANGUAGES CXX CUDA)
set(CMAKE_CXX_STANDARD 17)

#Add OWL
set(owl_dir ${PROJECT_SOURCE_DIR}/external/owl)
# EXCLUDE_FROM_ALL prevents automatically build any owl samples 
add_subdirectory(${owl_dir} EXCLUDE_FROM_ALL)

# Compile device.cu to PTX and add PTX_LINK_LIBRARIES
embed_ptx(
    OUTPUT_TARGET aura_renderer_ptx
    PTX_LINK_LIBRARIES owl::owl
    SOURCES device.cu
)

# 3. Create host executable
add_executable(aura_renderer main.cpp)

# Link everything for the host application
target_link_libraries(aura_renderer PRIVATE aura_renderer_ptx owl::owl)

# Need to copy owl.dll to the executable folder after building
add_custom_command(TARGET aura_renderer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:owl>
    $<TARGET_FILE_DIR:aura_renderer>
)