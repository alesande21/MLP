cmake_minimum_required(VERSION 3.19)
set(P_NAME MLP)
project(${P_NAME} VERSION 1.0.0 LANGUAGES CXX C)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}${EXE_DIR})

if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Release)
endif()
set(CURRENT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native")

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()
include_directories(${GTEST_INCLUDE_DIR})
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Gui Core PrintSupport LinguistTools OpenGL)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Gui Core PrintSupport LinguistTools OpenGL)


    if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
            qt_add_executable(${P_NAME}
                    MANUAL_FINALIZATION
                    MACOSX_BUNDLE
                    main.cc
                    )
    else()
            if(ANDROID)
                    add_library(${P_NAME} SHARED
                            main.cc
                            )
            else()
                    add_executable(${P_NAME}
                            main.cc
                            )
            endif()
    endif()
#    endforeach()

set_target_properties(${P_NAME} PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
)


add_subdirectory(Models)
add_subdirectory(Controllers)
add_subdirectory(Views)
add_subdirectory(ThirdParty)
#add_subdirectory(Tests)

target_link_libraries(
        ${P_NAME} PRIVATE
        models
        controllers
        views
        q_custom_plot
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::OpenGL
        Qt${QT_VERSION_MAJOR}::PrintSupport
)


