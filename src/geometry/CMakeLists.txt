set(GEOMETRY_SOURCES geometry.cpp math_utils.cpp geom_structs.cpp)
set(GEOMETRY_HEADERS geom_structs.hpp geometry.hpp math_utils.hpp )

add_library(geometry STATIC ${GEOMETRY_SOURCES} ${GEOMETRY_HEADERS})
target_include_directories(geometry PUBLIC "./")
