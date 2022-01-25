
message("Platform Module.")

set(platform_module_name platform_module)

file (GLOB_RECURSE platform_module_all_file "${src_dir}/platform/*")
sourceGroup("/src" ${platform_module_all_file})

if(NOT USE_DEPENDS_MODEL)
    add_library(${platform_module_name} ${platform_module_all_file})
endif()