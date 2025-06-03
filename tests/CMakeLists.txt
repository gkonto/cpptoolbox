include_directories(${CMAKE_SOURCE_DIR}/third_party)

set(ARENA_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/arena.t.cpp")
set(MALLOCATOR_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/mallocator.t.cpp")
set(SHORTALLOC_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/shortalloc.t.cpp")

add_executable(alltests 
    ${ARENA_SOURCES}
    ${MALLOCATOR_SOURCES}
    ${SHORTALLOC_SOURCES}
    )
target_link_libraries(alltests PRIVATE "mallocator" "arena" "shortalloc")
