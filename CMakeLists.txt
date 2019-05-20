cmake_minimum_required(VERSION 3.5.1)

project(libxtrace)

include_directories( include )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 ")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(Boost COMPONENTS system filesystem REQUIRED)

find_package(Protobuf REQUIRED)

add_library (libxtrace SHARED 
	src/atomlayer.cpp
	src/thread_local_baggage.cpp
	src/pubsub.cpp
	src/xtrace.cpp
	src/xtrace.pb.cc
	src/xtrace_baggage.cpp
	src/lexvarint.cpp
)
set_target_properties(libxtrace
	PROPERTIES
	OUTPUT_NAME "xtrace"
	PREFIX "lib"
)
target_link_libraries(libxtrace Threads::Threads ${Protobuf_LIBRARIES} ${Boost_LIBRARIES} )

# Introduce variables:
# * CMAKE_INSTALL_LIBDIR
# * CMAKE_INSTALL_BINDIR
# * CMAKE_INSTALL_INCLUDEDIR
include(GNUInstallDirs)



install(DIRECTORY "include"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/xtrace"
        FILES_MATCHING
        PATTERN "*.h")


install(TARGETS libxtrace DESTINATION "${CMAKE_INSTALL_LIBDIR}")


add_executable (main src/main.cc )

target_include_directories(main PRIVATE /usr/local/include/xtrace)
target_link_libraries(main libxtrace Threads::Threads ${Protobuf_LIBRARIES} ${Boost_LIBRARIES}  )
