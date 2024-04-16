cmake_minimum_required(VERSION 3.16)
project(Ships)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost REQUIRED COMPONENTS system thread chrono serialization)
find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_search_module(GLFW REQUIRED glfw3)

file(GLOB_RECURSE messagesFiles "Messages/*.h" "Messages/*.cpp")
file(GLOB_RECURSE utilFiles "Utils/*.h" "Utils/*.cpp")
file(GLOB_RECURSE netFiles "Networking/*.h" "Networking/*.cpp")
file(GLOB_RECURSE logicFiles "Logic/*.h" "Logic/*.cpp")

add_executable(server Endpoints/Server.cpp ${messagesFiles} ${utilFiles} ${netFiles})
add_executable(client Endpoints/Client.cpp ${messagesFiles} ${utilFiles} ${netFiles} ${logicFiles})
add_executable(test Endpoints/ser_test.cpp ${messagesFiles} ${utilFiles})

target_include_directories(server PRIVATE
        ${Boost_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIRS}
        ${GLFW_INCLUDE_DIRS})
target_link_libraries(server PRIVATE
        ${OPENGL_LIBRARIES}
        ${Boost_LIBRARIES})

target_include_directories(client PRIVATE
        ${Boost_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIRS}
        ${GLFW_INCLUDE_DIRS})
target_link_libraries(client PRIVATE
        ${OPENGL_LIBRARIES}
        ${Boost_LIBRARIES}
        ${GLEW_LIBRARIES}
        ${GLFW_LIBRARIES})

target_include_directories(test PRIVATE
        ${Boost_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIRS}
        ${GLFW_INCLUDE_DIRS})
target_link_libraries(test PRIVATE
        ${OPENGL_LIBRARIES}
        ${Boost_LIBRARIES})
