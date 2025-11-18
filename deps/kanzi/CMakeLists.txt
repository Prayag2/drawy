message(STATUS "Fetching kanzi...")

FetchContent_GetProperties(kanzi)
FetchContent_MakeAvailable(kanzi)

message(STATUS "kanzi_SOURCE_DIR=${kanzi_SOURCE_DIR}")

add_subdirectory(${kanzi_SOURCE_DIR}/src ${kanzi_BINARY_DIR})

target_link_libraries(drawy PRIVATE libkanzi)
target_include_directories(drawy PRIVATE ${kanzi_SOURCE_DIR}/src)
