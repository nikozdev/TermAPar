cmake_minimum_required(VERSION 3.3)
# mods
include(CheckCXXSourceCompiles)
include(cmake/mHelpRegex.cmake)
include(cmake/mHelpOpt.cmake)
# proj
project(TermAPar
    VERSION 4.1.0
    DESCRIPTION
    "single-header-only-library for cli argument parsing in cxx"
    ) # project
enable_language(CXX)
set(vProjName ${PROJECT_NAME})
set(vProjNameLib "l${vProjName}") # library target
set(vProjNameExe "e${vProjName}") # executable target
set(vProjVnum ${PROJECT_VERSION})
set(vProjDesc ${PROJECT_DESCRIPTION})
# lang
set(vLangVnum 23)
set(CMAKE_CXX_STANDARD ${vLangVnum})
set(CMAKE_CXX_STANDARD_REQUIRED ${vLangVnum})
# file
set(vFdirLoc ${PROJECT_SOURCE_DIR}/..)
set(vFdirSrcRoot ${vFdirLoc}/src)
set(vFdirSrcBase ${vFdirSrcRoot}/d${vProjName}.dir)
set(vFdirGen ${vFdirLoc}/gen)
# make
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
option(oMakeProjExe "make project executable" 1)
option(oTestProjExe "test project executable" 1)
## lib
add_library(${vProjNameLib} INTERFACE)
set(vProjDefsLib
    "$<$<CXX_COMPILER_ID:MSVC>:>"
    "$<$<CXX_COMPILER_ID:GNU,Clang>:>"
    "d${vProjName}ProjName=\"${vProjName}\""
    "d${vProjName}ProjNameLib=\"${vProjNameLib}\""
    "d${vProjName}ProjNameExe=\"${vProjNameExe}\""
    "d${vProjName}ProjVnum=\"${vProjVnum}\""
    "d${vProjName}ProjDesc=\"${vProjDesc}\""
    "d${vProjName}MakeProjExe=${oMakeProjExe}"
    "d${vProjName}TestProjExe=${oTestProjExe}"
    ) # vProjDefsLib
### libs
set(oLibsPickFormat "fmt")
set(vLibsListFormat "std;fmt;")
fOptEnum(oLibsPickFormat vLibsListFormat  "library pick for formatting")
if(NOT oLibsPickFormat IN_LIST vLibsListFormat)
    message(WARNING "library pick for format is not defined")
elseif(oLibsPickFormat STREQUAL "fmt")
    find_package(fmt CONFIG REQUIRED)
    target_link_libraries(${vProjNameLib} INTERFACE fmt::fmt)
    list(APPEND vProjDefsLib "d${vProjName}LibsPickFormatFmt")
elseif(oLibsPickFormat STREQUAL "std")
    if(${oLibsPickFormatStdCompiles})
    else()
        set(CMAKE_REQUIRED_FLAGS ${CMAKE_REQUIRED_FLAGS} "--std=c++20")
        check_cxx_source_compiles(
            "#include <format> int main() { return 0; }"
            oLibsPickFormatStdCompiles
            ) # check_cxx_source_compiles
        if(${oLibsPickFormatStdCompiles})
            list(APPEND vProjDefsLib "d${vProjName}LibsPickFormatStd")
        else()
            message(WARNING
                "std::format does not compile"
                " - choose another LibraryPickFormat"
                ) # message
        endif()
    endif() #oLibsPickFormatStdCompiles
else(oLibsPickFormat)
    message(WARNING "library pick for format is not handled")
endif(NOT oLibsPickFormat IN_LIST vLibsListFormat)
### last
target_compile_definitions(${vProjNameLib} INTERFACE ${vProjDefsLib})
target_include_directories(${vProjNameLib} INTERFACE ${vFdirSrcRoot})
## exe
if(oMakeProjExe)
    add_executable(${vProjNameExe} ${vFdirSrcBase}/fMain.cxx)
    # test
    if(oTestProjExe)
        enable_testing()
        #
        add_test(NAME tUnit
            COMMAND
            ${vFdirGen}/${vProjNameExe} TestUnit
            ) # add_test
        set(vPassRegex ".*)=[fTestUnit]")
        fRegexIgnoreABracket(vPassRegex)
        set_tests_properties(tUnit PROPERTIES
            PASS_REGULAR_EXPRESSION ${vPassRegex}
            ) # set_tests_properties
        #
        add_test(NAME tTime
            COMMAND
            ${vFdirGen}/${vProjNameExe} TestTime
            ) # add_test
        set_tests_properties(tTime PROPERTIES
            TIMEOUT 16
            ) # set_tests_properties
    endif(oTestProjExe)
    # libs
    target_link_libraries(${vProjNameExe} PRIVATE ${vProjNameLib})
endif(oMakeProjExe)
