include(FetchContent)

FetchContent_Declare(
  kanzi
  URL https://github.com/flanglet/kanzi-cpp/archive/refs/tags/2.4.0.zip
  URL_MD5 ec451cdab40362e9e4af8e5ca780a07d
)

add_subdirectory(kanzi)

# find_package(tinyxml2 REQUIRED)
# target_link_libraries(${PROJECT_NAME} PRIVATE tinyxml2::tinyxml2)
