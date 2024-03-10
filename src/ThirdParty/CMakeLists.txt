AUX_SOURCE_DIRECTORY(./QCustomPlot QCUSTOMPLOT_SRCS)
set(LIB_NAME q_custom_plot)
file(GLOB_RECURSE QCUSTOMPLOT_HEADER
            QCustomPlot/*.h
        )
add_library(${LIB_NAME} STATIC
        ${QCUSTOMPLOT_SRCS}
        ${QCUSTOMPLOT_HEADER}
        )
target_link_libraries(${LIB_NAME}
#        ${P_NAME}
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::OpenGL
        Qt${QT_VERSION_MAJOR}::PrintSupport
        )
# set_target_properties(${LIB_NAME} PROPERTIES PUBLIC_HEADER includes/qgifimage.)
target_include_directories(${LIB_NAME} PUBLIC QCustomPlot)
#target_include_directories(${LIB_NAME} PRIVATE 3rdParty/giflib)
#include(GNUInstallDirs)
