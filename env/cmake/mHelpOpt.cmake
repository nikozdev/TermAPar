cmake_minimum_required(VERSION 3.3)
# actions
function(fOptEnum oOptPick vOptEnum vOptDesc)
    set(${oOptPick} ${${oOptPick}} CACHE STRING ${vOptDesc})
    set_property(CACHE ${oOptPick} PROPERTY STRINGS ${${vOptEnum}})
endfunction(fOptEnum)
