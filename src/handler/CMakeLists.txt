
message("handler Module.")

set(handler_module_name handler_module)

file(GLOB_RECURSE handler_module_all_file "${src_dir}/handler/*")
sourceGroup("/src" ${handler_module_all_file})

if(NOT USE_DEPENDS_MODEL)
    add_library(${handler_module_name} ${handler_module_all_file})
endif()