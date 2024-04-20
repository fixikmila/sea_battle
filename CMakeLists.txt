cmake_minimum_required(VERSION 3.16)
project(Ships)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")

# Set Boost options
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_LIBS ON)
#set(Boost_USE_STATIC_RUNTIME ON)


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
file(GLOB_RECURSE guiFiles "Gui/*.h" "Gui/*.cpp")

add_executable(server Endpoints/Server.cpp ${messagesFiles} ${utilFiles} ${netFiles} ${logicFiles})
add_executable(client Endpoints/Client.cpp ${messagesFiles} ${utilFiles} ${netFiles} ${logicFiles} ${guiFiles})
add_executable(test Endpoints/ser_test.cpp ${messagesFiles} ${utilFiles})

target_include_directories(server PRIVATE
        ${Boost_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIRS}
        ${GLFW_INCLUDE_DIRS})
target_link_libraries(server  PRIVATE -static
        ${Boost_LIBRARIES})

target_include_directories(client PRIVATE
        ${Boost_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIRS}
        ${GLFW_INCLUDE_DIRS})
target_link_libraries(client PRIVATE
        ${OPENGL_LIBRARIES}
        ${GLEW_LIBRARIES}
        ${GLFW_LIBRARIES})
target_link_libraries(client PRIVATE
        ${Boost_LIBRARIES}
        )

target_include_directories(test PRIVATE
        ${Boost_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIRS}
        ${GLFW_INCLUDE_DIRS})
target_link_libraries(test PRIVATE
        ${OPENGL_LIBRARIES})
target_link_libraries(test PRIVATE
        ${Boost_LIBRARIES})