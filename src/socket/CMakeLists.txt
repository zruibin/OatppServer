
message("socket Module.")

set(socket_module_name socket_module)

file(GLOB_RECURSE socket_module_all_file "${src_dir}/socket/*")
sourceGroup("/src" ${socket_module_all_file})

if(NOT USE_DEPENDS_MODEL)
    add_library(${socket_module_name} ${socket_module_all_file})
endif()