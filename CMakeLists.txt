project(MongooseMVCFrameworkGen)
cmake_minimum_required(VERSION 2.8)
find_package(Boost REQUIRED COMPONENTS program_options regex)
aux_source_directory(src SRC)
list(APPEND FULL_SRC
	${SRC} )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")

include_directories(${Boost_INCLUDE_DIRS}) 
add_executable(${PROJECT_NAME} ${FULL_SRC})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
