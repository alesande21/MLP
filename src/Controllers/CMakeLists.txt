AUX_SOURCE_DIRECTORY(./src CONTROLLER_SRCS)
set(LIB_NAME controllers)

file(GLOB_RECURSE CONTROLLER_HEADER
        include/*.h
        )

add_library(${LIB_NAME} STATIC
        ${CONTROLLER_SRCS}
        ${CONTROLLER_HEADER}
        )
target_link_libraries(${LIB_NAME}
        models
        views
        q_custom_plot
#        ${P_NAME}
        Qt${QT_VERSION_MAJOR}::Widgets
        )
# set_target_properties(${LIB_NAME} PROPERTIES PUBLIC_HEADER includes/qgifimage.)
target_include_directories(${LIB_NAME} PUBLIC include)
#target_include_directories(${LIB_NAME} PRIVATE 3rdParty/giflib)
#include(GNUInstallDirs)



