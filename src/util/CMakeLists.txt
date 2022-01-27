
message("Util Module.")

set(util_module_name util_module)

file (GLOB_RECURSE util_module_all_file "${src_dir}/util/*")
sourceGroup("/src" ${util_module_all_file})

if(NOT USE_DEPENDS_MODEL)
    add_library(${util_module_name} ${util_module_all_file})
endif()
