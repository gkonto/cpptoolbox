include_directories(${CMAKE_SOURCE_DIR}/third_party)

set(ARENA_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/arena.t.cpp")
set(MALLOCATOR_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/mallocator.t.cpp")
set(SHORTALLOC_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/shortalloc.t.cpp")
set(GEOMETRY_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/geometry.t.cpp")

add_executable(
    alltests
    ${ARENA_SOURCES}
    ${MALLOCATOR_SOURCES}
    ${SHORTALLOC_SOURCES}
    ${GEOMETRY_SOURCES}
    )

target_link_libraries(
    alltests
    PRIVATE "mallocator"
            "arena"
            "shortalloc")
target_link_libraries(
    alltests
    PUBLIC "geometry")

target_include_directories(geometry PUBLIC ${TEST_INCLUDES})
