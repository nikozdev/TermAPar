cmake_minimum_required(VERSION 3.3)
# mods
include(CheckCXXSourceCompiles)
include(cmake/mHelpRegex.cmake)
include(cmake/mHelpOpt.cmake)
# proj
project(TermArgs
    VERSION 3.2.0
    DESCRIPTION "cli argument parsing single-header-only-library for cxx"
    ) # project
enable_language(CXX)
set(vProjName ${PROJECT_NAME})
set(vProjNameL "l${vProjName}") # library target
set(vProjVnum ${PROJECT_VERSION})
set(vProjDesc ${PROJECT_DESCRIPTION})
# lang
set(vLangVnum 23)
set(CMAKE_CXX_STANDARD ${vLangVnum})
set(CMAKE_CXX_STANDARD_REQUIRED ${vLangVnum})
# file
set(vFileDirLoc ${PROJECT_SOURCE_DIR}/..)
set(vFileDirSrc ${vFileDirLoc}/src)
set(vFileDirSrcTest ${vFileDirLoc}/src/d${vProjName}Test)
set(vFileDirSrcExam ${vFileDirLoc}/src/d${vProjName}Exam)
set(vFileDirGen ${vFileDirLoc}/gen)
# make
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_library(${vProjNameL} INTERFACE)
set(vDefnList
    "$<$<CXX_COMPILER_ID:MSVC>:>"
    "$<$<CXX_COMPILER_ID:GNU,Clang>:>"
    "d${vProjName}ProjName=\"${vProjName}\""
    "d${vProjName}ProjVnum=\"${vProjVnum}\""
    "d${vProjName}ProjDesc=\"${vProjDesc}\""
    ) # vDefnList
##libs
set(oLibsPickFormat "fmt")
set(vLibsListFormat "std;fmt")
fOptEnum(oLibsPickFormat vLibsListFormat  "library pick for formatting")
if(NOT oLibsPickFormat IN_LIST vLibsListFormat)
    message(WARNING "library pick for format is not defined")
elseif(oLibsPickFormat STREQUAL "fmt")
    find_package(fmt CONFIG REQUIRED)
    target_link_libraries(${vProjNameL} INTERFACE fmt::fmt)
    list(APPEND vDefnList "d${vProjName}LibsPickFormatFmt")
elseif(oLibsPickFormat STREQUAL "std")
    if(${oLibsPickFormatStdCompiles})
    else()
        set(CMAKE_REQUIRED_FLAGS ${CMAKE_REQUIRED_FLAGS} "--std=c++20")
        check_cxx_source_compiles(
            "#include <format> int main() { return 0; }"
            oLibsPickFormatStdCompiles
            ) # check_cxx_source_compiles
        if(${oLibsPickFormatStdCompiles})
            list(APPEND vDefnList "d${vProjName}LibsPickFormatStd")
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
target_compile_definitions(${vProjNameL} INTERFACE ${vDefnList})
target_include_directories(${vProjNameL} INTERFACE ${vFileDirSrc})
##test
enable_testing()
option(oMakeTestUnit "make test-unit target" 0)
if(oMakeTestUnit)
    add_executable(eTestUnit ${vFileDirSrcTest}/eUnit.cxx)
    add_test(NAME tUnit
        COMMAND ${vFileDirGen}/eTestUnit
        ) # add_test
    set(vPassRegex ".*)=[fTest]")
    fRegexIgnoreABracket(vPassRegex ${vPassRegex})
    set_tests_properties(tUnit PROPERTIES
        PASS_REGULAR_EXPRESSION ${vPassRegex}
        ) # set_tests_properties
    target_link_libraries(eTestUnit PRIVATE ${vProjNameL})
endif(oMakeTestUnit)
option(oMakeTestTime "make test-time target" 0)
if(oMakeTestTime)
    add_executable(eTestTime ${vFileDirSrcTest}/eTime.cxx)
    add_test(NAME tTime
        COMMAND ${vFileDirGen}/eTestTime
        ) # add_test
    set_tests_properties(tTime PROPERTIES
        TIMEOUT 16
        ) # set_tests_properties
    target_link_libraries(eTestTime PRIVATE ${vProjNameL})
endif(oMakeTestTime)
##exam
option(oMakeExamArg "make example-argument target" 0)
if(oMakeExamArg)
    add_executable(eExamArg ${vFileDirSrcExam}/eArg.cxx)
    target_link_libraries(eExamArg PRIVATE ${vProjNameL})
    add_test(NAME tExamArg
        COMMAND ${vFileDirGen}/eExamArg a1 -- a2
        ) # add_test
    set(vPassRegex "[ArgArr]=(.*[0]=(a1).*[1]=(a2).*)=[ArgArr]")
    fRegexIgnoreABracket(vPassRegex ${vPassRegex})
    set_tests_properties(tExamArg PROPERTIES
        PASS_REGULAR_EXPRESSION ${vPassRegex}
        ) # set_tests_properties
endif(oMakeExamArg)
option(oMakeExamOpt "make example-option target" 0)
if(oMakeExamOpt)
    add_executable(eExamOpt ${vFileDirSrcExam}/eOpt.cxx)
    target_link_libraries(eExamOpt PRIVATE ${vProjNameL})
    add_test(NAME tExamOpt
        COMMAND ${vFileDirGen}/eExamOpt -o=1 a1 -- a2 -- -o=2
        ) # add_test
    set(vPassRegex ".*[OptTab]=(.*[o]=(2).*[opt]=(2).*)=[OptTab]")
    fRegexIgnoreABracket(vPassRegex ${vPassRegex})
    set_tests_properties(tExamOpt PROPERTIES
        PASS_REGULAR_EXPRESSION ${vPassRegex}
        ) # set_tests_properties
endif(oMakeExamOpt)
option(oMakeExamCmd "make example-command target" 0)
if(oMakeExamCmd)
    add_executable(eExamCmd ${vFileDirSrcExam}/eCmd.cxx)
    target_link_libraries(eExamCmd PRIVATE ${vProjNameL})
    add_test(NAME tExamCmd
        COMMAND ${vFileDirGen}/eExamCmd cmd -o=1 a1 -- a2 -- -o=2
        ) # add_test
    set(vPassRegex ".*[CmdTab]=(.*[c]=(.*)=[c].*[cmd]=(.*)=[cmd].*)=[CmdTab]")
    fRegexIgnoreABracket(vPassRegex ${vPassRegex})
    set_tests_properties(tExamCmd PROPERTIES
        PASS_REGULAR_EXPRESSION ${vPassRegex}
        ) # set_tests_properties
endif(oMakeExamCmd)
