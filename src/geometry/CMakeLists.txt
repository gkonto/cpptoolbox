set(GEOMETRY_SOURCES barycentric.cpp)
<<<<<<< HEAD
set(GEOMETRY_HEADERS barycentric.hpp matrix.hpp vector.hpp plane.hpp utils.hpp)
=======
set(GEOMETRY_HEADERS barycentric.hpp matrix.hpp convex.hpp)
>>>>>>> bb5ab37 (convex functions checking)

add_library(geometry STATIC ${GEOMETRY_SOURCES} ${GEOMETRY_HEADERS})
target_include_directories(geometry PUBLIC "./")
