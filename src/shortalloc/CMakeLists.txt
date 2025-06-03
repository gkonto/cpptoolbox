add_library("shortalloc" INTERFACE)
target_include_directories("shortalloc" INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(shortalloc INTERFACE "arena")