cmake_minimum_required(VERSION 3.16)

project(drawy VERSION 0.1 LANGUAGES CXX)

# Enable automatic handling of UI, MOC, and RCC files.
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Find Qt6
find_package(Qt6 REQUIRED
    COMPONENTS
        OpenGLWidgets
        LinguistTools
)

# Translation files.
set(TS_FILES ${CMAKE_SOURCE_DIR}/translations/drawy_en_GB.ts)

# Source directory.
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

# Use globbing to recursively collect all source, header, and resource files from src/.
file(GLOB_RECURSE SRC_FILES
    "${SRC_DIR}/*.cpp"
    "${SRC_DIR}/*.hpp"
    "${SRC_DIR}/*.qrc"
    "${SRC_DIR}/*.qss"
)

# Combine translation files with the source files.
set(PROJECT_SOURCES ${TS_FILES} ${SRC_FILES})

qt_add_executable(${PROJECT_NAME}
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

add_subdirectory(deps)

qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::OpenGLWidgets)

# Set bundle properties for macOS / iOS.
if (APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER com.example.${PROJECT_NAME}
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
        MACOSX_BUNDLE TRUE
    )
endif()

if (WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if (UNIX AND NOT APPLE)
  install(PROGRAMS deploy/linux/io.github.prayag2.Drawy.desktop.in DESTINATION ${CMAKE_INSTALL_FULL_DATADIR}/applications RENAME io.github.prayag2.Drawy.desktop)
  install(FILES assets/logo.svg DESTINATION ${CMAKE_INSTALL_FULL_DATADIR}/icons/hicolor/scalable/apps RENAME io.github.prayag2.Drawy.svg)
  install(FILES deploy/linux/io.github.prayag2.Drawy.metainfo.xml.in DESTINATION ${CMAKE_INSTALL_FULL_DATADIR}/metainfo/ RENAME io.github.prayag2.Drawy.metainfo.xml)
endif()
