
#добавил для тестов
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

set(TESTS_NAME TESTING)

#set(${TESTS_NAME}_RES
#        TestFiles/*.csv
#        )

AUX_SOURCE_DIRECTORY(./src ${TESTS_NAME}_SRCS)
add_executable(${TESTS_NAME}
        ${${TESTS_NAME}_SRCS}
        )
target_link_libraries(${TESTS_NAME}
        models
#        controllers
        views
        q_custom_plot
#        Qt${QT_VERSION_MAJOR}::Gui
#        Qt${QT_VERSION_MAJOR}::Core
#        Qt${QT_VERSION_MAJOR}::Widgets
#        Qt${QT_VERSION_MAJOR}::OpenGL
        GTest::GTest
        Qt${QT_VERSION_MAJOR}::PrintSupport
        )
include(GoogleTest)
gtest_discover_tests(${TESTS_NAME})
add_test(NAME MyTests COMMAND MyTests)

#add_test(NAME ${P_NAME} ${P_NAME} WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/TestFiles")

#set_tests_properties(MyTests PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
