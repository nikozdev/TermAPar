cmake_minimum_required(VERSION 3.3...3.8)
include_guard(GLOBAL)
# actions
function(fPackFind vPackName vPackVnum)
    find_package(${vPackName} ${vPackVnum} REQUIRED)
    mark_as_advanced(FORCE ${vPackName}_DIR)
endfunction(fPackFind)
