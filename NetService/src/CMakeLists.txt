include_directories(${CMAKE_SOURCE_DIR}/NMessage) 
include_directories(${CMAKE_SOURCE_DIR}/NetService/include)
add_subdirectory(mysql)

find_package(Threads)

file(GLOB SRCS "*.cpp")
add_executable(server ${SRCS})
add_dependencies(server nmessage mysql-wrapper mysqlclient boost_program_options)
target_link_libraries(server ${CMAKE_THREAD_LIBS_INIT} nmessage mysql-wrapper mysqlclient boost_program_options)
