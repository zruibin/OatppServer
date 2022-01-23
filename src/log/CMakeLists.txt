
message("Log Manager Module.")

set(log_module_name log_module)

file (GLOB_RECURSE log_module_all_file ${src_dir}/log/*)
add_library(${log_module_name} ${log_module_all_file})
sourceGroup("/src" ${log_module_all_file})
