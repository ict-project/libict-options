################################################################
cmake_minimum_required(VERSION 3.25)
################################################################
set(LIBRARY_NAME "options")
set(LIBRARY_DESC "Library for parsing of command-line arguments")

project(libict-${LIBRARY_NAME})

set(CMAKE_CXX_STANDARD 17)
find_package(Threads)

include(../libict-dev-tools/libs-include.cmake)
include(../libict-dev-tools/info-include.cmake)
include(../libict-dev-tools/test-include.cmake)
include(../libict-dev-tools/git-include.cmake)

file(GLOB CMAKE_HEADER_LIST . *.hpp)

set(CMAKE_SOURCE_FILES 
  info.cpp
  options.cpp
  suboptions.cpp
)

add_library(ict-static-${LIBRARY_NAME} STATIC ${CMAKE_SOURCE_FILES})
target_link_libraries(ict-static-${LIBRARY_NAME} ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(ict-static-${LIBRARY_NAME} ${CMAKE_LINK_LIBS})
set_target_properties(ict-static-${LIBRARY_NAME}  PROPERTIES OUTPUT_NAME ict-${LIBRARY_NAME})

add_library(ict-shared-${LIBRARY_NAME} SHARED ${CMAKE_SOURCE_FILES})
target_link_libraries(ict-shared-${LIBRARY_NAME} ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(ict-shared-${LIBRARY_NAME} ${CMAKE_LINK_LIBS})
set_target_properties(ict-shared-${LIBRARY_NAME}  PROPERTIES OUTPUT_NAME ict-${LIBRARY_NAME})

add_executable(${PROJECT_NAME}-test test.cpp ${CMAKE_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME}-test ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(${PROJECT_NAME}-test ${CMAKE_LINK_LIBS})
target_compile_definitions(${PROJECT_NAME}-test PUBLIC -DENABLE_TESTING)

################################################################
install(TARGETS ict-static-${LIBRARY_NAME} ict-shared-${LIBRARY_NAME} DESTINATION lib COMPONENT libraries)
install(
  FILES ${HEADER_LIST}
  DESTINATION include/libict/${LIBRARY_NAME} COMPONENT headers
)
################################################################
enable_testing()

add_test(NAME ict-options-tc1 COMMAND ${PROJECT_NAME}-test ict options tc1)
add_test(NAME ict-options-tc2 COMMAND ${PROJECT_NAME}-test ict options tc2)
add_test(NAME ict-options-tc3 COMMAND ${PROJECT_NAME}-test ict options tc3)
add_test(NAME ict-options-tc4 COMMAND ${PROJECT_NAME}-test ict options tc4)
add_test(NAME ict-options-tc5 COMMAND ${PROJECT_NAME}-test ict options tc5)
add_test(NAME ict-options-tc6 COMMAND ${PROJECT_NAME}-test ict options tc6)
add_test(NAME ict-options-tc7 COMMAND ${PROJECT_NAME}-test ict options tc7)
add_test(NAME ict-options-tc8 COMMAND ${PROJECT_NAME}-test ict options tc8)
add_test(NAME ict-options-tc9 COMMAND ${PROJECT_NAME}-test ict options tc9)
add_test(NAME ict-options-tc10 COMMAND ${PROJECT_NAME}-test ict options tc10)
add_test(NAME ict-options-tc11 COMMAND ${PROJECT_NAME}-test ict options tc11)
add_test(NAME ict-options-tc12 COMMAND ${PROJECT_NAME}-test ict options tc12)
add_test(NAME ict-options-tc13 COMMAND ${PROJECT_NAME}-test ict options tc13)
add_test(NAME ict-options-tc14 COMMAND ${PROJECT_NAME}-test ict options tc14)
add_test(NAME ict-options-tc15 COMMAND ${PROJECT_NAME}-test ict options tc15)
add_test(NAME ict-options-tc16 COMMAND ${PROJECT_NAME}-test ict options tc16)
add_test(NAME ict-options-tc17 COMMAND ${PROJECT_NAME}-test ict options tc17)
add_test(NAME ict-options-tc18 COMMAND ${PROJECT_NAME}-test ict options tc18)
add_test(NAME ict-suboptions-tc1 COMMAND ${PROJECT_NAME}-test ict suboptions tc1)
add_test(NAME ict-suboptions-tc2 COMMAND ${PROJECT_NAME}-test ict suboptions tc2)
add_test(NAME ict-suboptions-tc3 COMMAND ${PROJECT_NAME}-test ict suboptions tc3)
add_test(NAME ict-suboptions-tc5 COMMAND ${PROJECT_NAME}-test ict suboptions tc5)

################################################################
include(../libict-dev-tools/cpack-include.cmake)
################################################################
