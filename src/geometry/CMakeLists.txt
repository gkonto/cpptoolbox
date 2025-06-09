set(GEOMETRY_SOURCES barycentric.cpp)
set(GEOMETRY_HEADERS barycentric.hpp matrix.hpp vector.hpp plane.hpp utils.hpp)

add_library(geometry STATIC ${GEOMETRY_SOURCES} ${GEOMETRY_HEADERS})
target_include_directories(geometry PUBLIC "./")
