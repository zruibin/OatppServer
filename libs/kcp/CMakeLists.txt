
message("kcp version was 1.7.0")

set(kcp_name kcp)

file (GLOB_RECURSE kcp_all_file ${libs_dir}/kcp/*)
add_library (${kcp_name} ${kcp_all_file})
sourceGroup("/libs/kcp" ${kcp_all_file})
