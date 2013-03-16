include_directories(${CMAKE_SOURCE_DIR}/NetService/include)
file(GLOB program_options_SRCS "*.cpp")
#list(APPEND program_options_SRCS detail/utf8_codecvt_facet.cpp)
add_library(boost_program_options STATIC ${program_options_SRCS})