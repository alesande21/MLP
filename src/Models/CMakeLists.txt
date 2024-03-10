set(LIB_NAME models)
file(GLOB_RECURSE MODEL_HEADER
        include/*.h
        )
file(GLOB_RECURSE MODEL_SRCS
    src/*.cc
)
add_library(${LIB_NAME} STATIC
        ${MODEL_SRCS}
        ${MODEL_HEADER}
        )
target_link_libraries(${LIB_NAME} PRIVATE
#        ${P_NAME}
        views
        q_custom_plot
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::PrintSupport
        )
target_include_directories(${LIB_NAME} PUBLIC include)
