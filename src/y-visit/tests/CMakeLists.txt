
INCLUDE_DIRECTORIES(../../y-swamp)

FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-visit main.cpp ${test_srcs})

TARGET_LINK_YOCTO( test-visit swamp )
TARGET_LINK_VISIT( test-visit )
VISIT_CUSTOM_UI( test-visit loop.ui )

#ADD_EXECUTABLE(pjacobi pjacobi.c solvers.c pjacobi-visit.c solvers.h pjacobi.h)
#TARGET_LINK_LIBRARIES(pjacobi simV2 m dl ${MPI_LIBRARIES})


