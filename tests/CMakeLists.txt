include_directories(${CMAKE_SOURCE_DIR}/third_party)
set(TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/arena.t.cpp")
add_executable("arenatest" ${TEST_SOURCES})
target_link_libraries("arenatest" PRIVATE "arena")