cmake_minimum_required (VERSION 3.20)

set(PROJECT_NAME LuaAnalysis)
project (${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 23)

find_package(BISON)
find_package(FLEX)

BISON_TARGET(MyParser parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
             DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.hpp)
FLEX_TARGET(MyScanner code.l  ${CMAKE_CURRENT_BINARY_DIR}/lex.cpp)
ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)

message("Flex found: " ${FLEX_FOUND})
message("Bison found: " ${BISON_FOUND})
message(${BISON_MyParser_OUTPUTS})

set(SRC_FILES
Tree.cpp
TreePrint.cpp
ClassTable.cpp
MainClassTable.cpp
FunctionClassTable.cpp
${BISON_MyParser_OUTPUT_SOURCE}
${FLEX_MyScanner_OUTPUTS}
)

set(INCLUDE_DIRS
${CMAKE_CURRENT_LIST_DIR}
${BISON_INCLUDE_DIRS}
)

add_executable (${PROJECT_NAME} ${SRC_FILES})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 23)

include_directories(${INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME})