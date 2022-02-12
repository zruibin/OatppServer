cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

PROJECT(OatppServer)

# SET(CMAKE_BUILD_TYPE "Debug")
# SET(CMAKE_BUILD_TYPE "Release")
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()
message(STATUS "Build type was ${CMAKE_BUILD_TYPE}")

# 生成compile_commands.json,用于在VSCode等编辑器/IDE中给C/C++代码做函数定义跳转支持
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# 检查c++编译器标志，设置c++11支持变量
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++17" COMPILER_SUPPORTS_CXX14)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)

# message("List of compile features: ${CMAKE_CXX_COMPILE_FEATURES}")
# 设置c++的编译选项
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++17" COMPILER_SUPPORTS_CXX17)
if(COMPILER_SUPPORTS_CXX17)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has C++17 support.")
# elseif(COMPILER_SUPPORTS_CXX0X)
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
#     message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has C++0x support.")
else()
    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++17 support. Please use a different C++ compiler.")
endif()
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")  #设置c的编译选项


if(CMAKE_BUILD_TYPE MATCHES "Debug")
    message(WARNING "Debug Model.")
    add_definitions("-Wall")
    add_definitions("-fPIC")
    add_definitions("-Wl,-lm")
    add_definitions("-Wno-dev")  
    list(APPEND flags "-fPIC" "-Wall")
    if(NOT WIN32)
        list(APPEND flags "-Wextra" "-Wpedantic")
    endif()
endif()


message(STATUS "Is the C++ compiler loaded? ${CMAKE_CXX_COMPILER_LOADED}")
if(CMAKE_CXX_COMPILER_LOADED)
    message(STATUS "The C++ compiler ID is: ${CMAKE_CXX_COMPILER_ID}")
    # message(STATUS "Is the C++ from GNU? ${CMAKE_COMPILER_IS_GNUCXX}")
    message(STATUS "The C++ compiler version is: ${CMAKE_CXX_COMPILER_VERSION}")
endif()
message(STATUS "Is the C compiler loaded? ${CMAKE_C_COMPILER_LOADED}")
if(CMAKE_C_COMPILER_LOADED)
    message(STATUS "The C compiler ID is: ${CMAKE_C_COMPILER_ID}")
    # message(STATUS "Is the C from GNU? ${CMAKE_COMPILER_IS_GNUCC}")
    message(STATUS "The C compiler version is: ${CMAKE_C_COMPILER_VERSION}")
endif()
message(STATUS "C flags, Debug configuration: ${CMAKE_C_FLAGS_DEBUG}")
message(STATUS "C flags, Release configuration: ${CMAKE_C_FLAGS_RELEASE}")
message(STATUS "C flags, Release configuration with Debug info: ${CMAKE_C_FLAGS_RELWITHDEBINFO}")
message(STATUS "C flags, minimal Release configuration: ${CMAKE_C_FLAGS_MINSIZEREL}")
message(STATUS "C++ flags, Debug configuration: ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "C++ flags, Release configuration: ${CMAKE_CXX_FLAGS_RELEASE}")
message(STATUS "C++ flags, Release configuration with Debug info: ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
message(STATUS "C++ flags, minimal Release configuration: ${CMAKE_CXX_FLAGS_MINSIZEREL}")


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

# 添加依赖
if(EXISTS "${PROJECT_SOURCE_DIR}/deps.cmake")
    include("deps.cmake")
endif()

get_property(include_dirs DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
message(WARNING ">>>include_dirs=${include_dirs}")
get_property(link_dirs DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY LINK_DIRECTORIES)
message(WARNING ">>>link_dirs=${link_dirs}")

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
include("${libs_dir}/nlohmann/CMakeLists.txt")
include("${libs_dir}/asio/CMakeLists.txt")
include("${libs_dir}/websocketpp/CMakeLists.txt")
include("${libs_dir}/kcp/CMakeLists.txt")
include("${libs_dir}/lua/CMakeLists.txt")
include("${libs_dir}/sigslot/CMakeLists.txt")
# modules
include("${src_dir}/log/CMakeLists.txt")
include("${src_dir}/handler/CMakeLists.txt")
include("${src_dir}/socket/CMakeLists.txt")
include("${src_dir}/platform/CMakeLists.txt")
include("${src_dir}/util/CMakeLists.txt")


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
    ${nlohmann_json_all_file}
    ${sigslot_all_file}

    ${log_module_all_file}
    ${handler_module_all_file}
    ${socket_module_all_file}
    ${platform_module_all_file}
    ${util_module_all_file}
)

else()

set(libs_list 
    oatpp
    asio
    kcp
    lua
)

set(modules_list 
    log_module
    handler_module
    socket_module
    platform_module
    util_module
)

set(DEPENDS_LIBS 
    ${websocketpp_all_file}
    ${nlohmann_json_all_file}
    ${sigslot_all_file}
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
message(STATUS "OUTPUT_DIR: ${OUTPUT_DIR}")
message(STATUS "ARCHIVE_OUTPUT_DIRECTORY: ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
message(STATUS "LIBRARY_OUTPUT_DIRECTORY: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
message(STATUS "RUNTIME_OUTPUT_DIRECTORY: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

set(TARGET ${PROJECT_NAME})

set(TARGET_STATIC_LIB "${PROJECT_NAME}_static")
add_library(${TARGET_STATIC_LIB} STATIC ${DEPENDS_LIBS} ${HEADERS} ${SOURCES} ${all_list})
set(TARGET_SHARED_LIB "${PROJECT_NAME}_shared")
add_library(${TARGET_SHARED_LIB} SHARED ${DEPENDS_LIBS} ${HEADERS} ${SOURCES} ${all_list})

add_executable(${TARGET} ${DEPENDS_LIBS} ${HEADERS} ${SOURCES})

target_compile_options(${TARGET} PRIVATE ${flags})
target_link_libraries(${TARGET} PRIVATE ${all_list} ${deps_list})



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
    TARGETS ${TARGET} ${TARGET_STATIC_LIB} ${TARGET_SHARED_LIB}
    RUNTIME DESTINATION bin 
    LIBRARY DESTINATION lib
)
set(output_head_files
    "${src_dir}/app.h"
)
install(FILES ${output_head_files} DESTINATION include)

#-------------------------------------------------------------------------------
add_custom_target(run
    COMMAND ./${TARGET}
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    COMMENT "Intermediate Run target: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
)

