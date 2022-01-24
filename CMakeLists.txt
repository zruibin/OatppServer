cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

PROJECT(OatppServer)

# SET(CMAKE_BUILD_TYPE "Debug")
# SET(CMAKE_BUILD_TYPE "Release")
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()
message(STATUS "Build type was ${CMAKE_BUILD_TYPE}")


# 检查c++编译器标志，设置c++11支持变量
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)

# 使用变量设置编译标志
if(COMPILER_SUPPORTS_CXX14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
elseif(COMPILER_SUPPORTS_CXX0X)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++14 support. Please use a different C++ compiler.")
endif()

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")  #设置c++的编译选项
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")  #设置c的编译选项

if(CMAKE_BUILD_TYPE MATCHES "Debug")
    add_definitions("-Wall")
    add_definitions("-fPIC")
    add_definitions("-Wl,-lm")
    add_definitions("-Wno-dev")  
endif()

set(src_dir "${PROJECT_SOURCE_DIR}/src")
set(libs_dir "${PROJECT_SOURCE_DIR}/libs")
include_directories(
    "${src_dir}"
    "${libs_dir}"
)

# 相当于g++命令的-L选项的作用
link_directories(
    "/usr/local"
    "${src_dir}"
    "${libs_dir}"
)

link_libraries(pthread dl)

function(deleteFiles)
    foreach(FILE IN LISTS ARGN) 
        if ("${FILE}" MATCHES ".DS_Store")
            message("删除文件: ${FILE}")
            file(REMOVE ${FILE})
        endif()
    endforeach()
endfunction()
file(GLOB_RECURSE delete_seerch_all_file "*")
deleteFiles(${delete_seerch_all_file})


# 按文件层次结构显示
function(sourceGroup prefix)
    message("prefix: ${prefix}")
    foreach(FILE IN LISTS ARGN) 
        get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)
        string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")
        string(REPLACE "${prefix}" "" GROUP "${GROUP}")
        string(REPLACE "/" "\\" GROUP "${GROUP}")

        if ("${FILE}" MATCHES ".*\\.cpp" AND ".*\\.cc" AND ".*\\.c")
            set(GROUP "Source Files${GROUP}")
        elseif("${FILE}" MATCHES ".*\\.h" AND ".*\\.hpp")
            set(GROUP "Header Files${GROUP}")
        endif()

        source_group("${GROUP}" FILES "${FILE}")
    endforeach()
endfunction()

# 开关，是否要源码依赖方式
set(USE_DEPENDS_MODEL TRUE)

# libs
include("${libs_dir}/oatpp/CMakeLists.txt")
# include("${libs_dir}/oatpp-websocket/CMakeLists.txt")
include("${libs_dir}/asio/CMakeLists.txt")
include("${libs_dir}/websocketpp/CMakeLists.txt")
include("${libs_dir}/kcp/CMakeLists.txt")
include("${libs_dir}/lua/CMakeLists.txt")
# modules
include("${src_dir}/log/CMakeLists.txt")
include("${src_dir}/handler/CMakeLists.txt")
include("${src_dir}/socket/CMakeLists.txt")


if(USE_DEPENDS_MODEL)

set(libs_list 
)

set(modules_list 
)

set(DEPENDS_LIBS 
    ${oatpp_all_file}
    ${asio_all_file}
    ${kcp_all_file}
    ${lua_all_file}
    ${websocketpp_all_file}
    ${log_module_all_file}
    ${handler_module_all_file}
    ${socket_module_all_file}
)

else()

set(libs_list 
    oatpp
    # oatpp-websocket
    asio
    kcp
    lua
)

set(modules_list 
    log_module
    handler_module
    socket_module
)

set(DEPENDS_LIBS 
    ${websocketpp_all_file}
)

endif()



set(all_list ${libs_list} ${modules_list})
message("all_list: ${all_list}")

set(HEADERS
    src/app.h
)

set(SOURCES
    src/main.cc
    src/app.cc
)

source_group(TREE "${CMAKE_SOURCE_DIR}" FILES ${HEADERS} ${SOURCES})


set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR}/Lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR}/Lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}/Bin)

set(TARGET ${PROJECT_NAME})

add_executable(${TARGET} ${DEPENDS_LIBS} ${HEADERS} ${SOURCES})

target_link_libraries(${TARGET} PRIVATE ${all_list})



#-------------------------------------------------------------------------------
message(STATUS "origin installed directory: ${CMAKE_INSTALL_PREFIX}")
if(${CMAKE_INSTALL_PREFIX} MATCHES "/usr/local")
    set(install_directory "${PROJECT_SOURCE_DIR}/install")
    file(MAKE_DIRECTORY ${install_directory})
    set(CMAKE_INSTALL_PREFIX ${install_directory})
    message(STATUS "Create installed directory: ${CMAKE_INSTALL_PREFIX}")
endif()
message(STATUS "Project will be installed to ${CMAKE_INSTALL_PREFIX}")

install(
    TARGETS ${PROJECT_NAME} ${all_list}
    RUNTIME DESTINATION bin 
    LIBRARY DESTINATION lib
)

