cmake_minimum_required(VERSION 3.23)
project(Ships)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -ldl ")
#include_directories (/usr/include/freetype2)
# Set Boost options
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_LIBS ON)
#set(Boost_USE_STATIC_RUNTIME ON)
set(GLAD_INCLUDE_DIR "Graphics/glad/include/glad")
#set(GLAD_LIBRARY "/usr/lib/libGLAD.so")
set(SOIL_INCLUDE_DIR "/usr/include/SOIL")
set(SOIL_LIBRARY "/usr/lib/libSOIL.so")
set(PQXX_INCLUDE_DIR "/usr/include/pqxx")
set(PQXX_LIBRARY "/usr/lib/x86_64-linux-gnu/libpqxx.so")
set(FREETYPE2_INCLUDE_DIR "/usr/include/freetype2")
set(FREETYPE2_LIBRARY "/usr/lib/x86_64-linux-gnu/libfreetype.a")
add_library(SOIL STATIC IMPORTED)
add_library(GLAD STATIC IMPORTED)
add_library(PQXX STATIC IMPORTED)
add_library(FREETYPE2 STATIC IMPORTED)
set_target_properties(SOIL PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${SOIL_INCLUDE_DIR}"
        IMPORTED_LOCATION "${SOIL_LIBRARY}"
)
set_target_properties(PQXX PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${PQXX_INCLUDE_DIR}"
        IMPORTED_LOCATION "${PQXX_LIBRARY}"
)
set_target_properties(FREETYPE2 PROPERTIES
INTERFACE_INCLUDE_DIRECTORIES "${FREETYPE2_INCLUDE_DIR}"
        IMPORTED_LOCATION "${FREETYPE2_LIBRARY}"
)

#find_package(libpqxx REQUIRED)
find_package(Boost REQUIRED COMPONENTS system thread chrono serialization)
find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_search_module(GLFW REQUIRED glfw3)
find_package(ZLIB)
file(GLOB_RECURSE messagesFiles "Messages/*.h" "Messages/*.cpp")
file(GLOB_RECURSE utilFiles "Utils/*.h" "Utils/*.cpp")
file(GLOB_RECURSE netFiles "Networking/*.h" "Networking/*.cpp")
file(GLOB_RECURSE logicFiles "Logic/*.h" "Logic/*.cpp")
file(GLOB_RECURSE guiFiles "Gui/*.h" "Gui/*.cpp")
file(GLOB_RECURSE graphics "Graphics/Shader.h" "Graphics/Camera.h" "Graphics/glad.c")

add_executable(server Endpoints/Server.cpp ${messagesFiles} ${utilFiles} ${netFiles} ${logicFiles})
add_executable(client Endpoints/Client.cpp ${messagesFiles} ${utilFiles} ${netFiles} ${logicFiles} ${guiFiles} ${graphics})
add_executable(test Endpoints/ser_test.cpp ${messagesFiles} ${utilFiles})
find_package(Freetype REQUIRED)
target_include_directories(server PRIVATE
        ${Boost_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIRS}
        ${GLFW_INCLUDE_DIRS})
target_link_libraries(server  PRIVATE -static
        ${Boost_LIBRARIES})

target_include_directories(client PRIVATE
        ${Boost_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIRS}
        ${GLFW_INCLUDE_DIRS}
        #${FREETYPE2_INCLUDE_DIR}
        #${PQXX_INCLUDE_DIR}
        ${FREETYPE_INCLUDE_DIRS}
)
target_link_libraries(client PRIVATE
        ${OPENGL_LIBRARIES}
        ${GLEW_LIBRARIES}
        ${GLFW_LIBRARIES}
        ${PQXX_LIBRARY}
        SOIL
        #${FREETYPE2_LIBRARY}
        ${FREETYPE_LIBRARIES}
        #-lpng
        #ZLIB::ZLIB
)
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