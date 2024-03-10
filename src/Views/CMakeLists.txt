AUX_SOURCE_DIRECTORY(./src/ConfigSettings VIEWS_SRCS)
AUX_SOURCE_DIRECTORY(./src/Draw VIEWS_SRCS)
AUX_SOURCE_DIRECTORY(./src/Graph VIEWS_SRCS)
AUX_SOURCE_DIRECTORY(./src/MainWindow VIEWS_SRCS)
#AUX_SOURCE_DIRECTORY(../main.cc VIEWS_SRCS)
#set(VIEWS_SRCS ../main.cc)

set(LIB_NAME views)
set(CMAKE_AUTOUIC_SEARCH_PATHS ./UI)
file(GLOB_RECURSE VIEWS_HEADER
        include/*.h
        )
file(GLOB_RECURSE VIEWS_UIS
        Ui/*.ui
        )

add_library(${LIB_NAME} STATIC
        ${VIEWS_UIS}
        ${VIEWS_SRCS}
        ${VIEWS_HEADER}
        )

target_link_libraries(${LIB_NAME} PRIVATE
#        ${P_NAME}
        q_custom_plot
        models
        controllers
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::OpenGL
        Qt${QT_VERSION_MAJOR}::PrintSupport
        )

target_include_directories(${LIB_NAME} PUBLIC include)

#    set(PROJECT_SOURCES
#            ${VIEWS_SRCS}
#            ${VIEWS_HEADER}
#            ${VIEWS_UIS}
#            )

#    if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
#            qt_add_executable(MLP
#                    MANUAL_FINALIZATION
#                    MACOSX_BUNDLE
#                    ${PROJECT_SOURCES}
#                    )
#    else()
#            if(ANDROID)
#                    add_library(MLP SHARED
#                            ${PROJECT_SOURCES}
#                            )
#            else()
#                    add_executable(MLP
#                            ${PROJECT_SOURCES}
#                            )
#            endif()
#    endif()
#endforeach()


#set_target_properties(MLP PROPERTIES
#        MACOSX_BUNDLE_GUI_IDENTIFIER MLP_1.0
#        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
#        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
#        )
