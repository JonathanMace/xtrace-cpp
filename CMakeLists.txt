cmake_minimum_required(VERSION 3.4.1)

project(libxtrace)

include_directories( include )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 ")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(Boost COMPONENTS system filesystem REQUIRED)

find_package(Protobuf 2 EXACT REQUIRED)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS proto/xtrace.proto)

add_library (libxtrace SHARED 
	src/atomlayer.cpp
	src/thread_local_baggage.cpp
	src/pubsub.cpp
	src/xtrace.cpp
	src/xtrace_baggage.cpp
	src/lexvarint.cpp
	${PROTO_SRCS}
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



install(DIRECTORY "include/"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/xtrace"
        FILES_MATCHING
        PATTERN "*.h")


install(TARGETS libxtrace DESTINATION "${CMAKE_INSTALL_LIBDIR}")

# Compile the Lua library

find_package(Lua51 REQUIRED)

add_library(luaxtrace SHARED
	src/luaxtrace.cpp
	src/module.cpp
	src/lua_baggage.cpp
)

target_include_directories(luaxtrace PRIVATE ${LUA_INCLUDE_DIR} /usr/local/include/xtrace)

set_target_properties(luaxtrace PROPERTIES PREFIX "")
set_target_properties(luaxtrace PROPERTIES SUFFIX ".so")

target_link_libraries(luaxtrace libxtrace Threads::Threads protobuf boost_system )

install(TARGETS luaxtrace DESTINATION ${CMAKE_INSTALL_LIBDIR})

# Compile the main example program
add_executable (main src/main.cc )

target_include_directories(main PRIVATE /usr/local/include/xtrace)
target_link_libraries(main libxtrace Threads::Threads protobuf boost_system  )
