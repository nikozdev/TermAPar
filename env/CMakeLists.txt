cmake_minimum_required(VERSION 3.20)
# mods
include(mRegex.cmake)
# proj
project(TermArgs
    VERSION 3.1.1
    DESCRIPTION "cli argument parsing single-header-only-library for cxx"
    ) # project
set(vProjName ${PROJECT_NAME})
set(vProjNameL "l${vProjName}")
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
# libs
find_package(fmt CONFIG REQUIRED)
# make
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_library(${vProjNameL} INTERFACE)
target_compile_definitions(${vProjNameL} INTERFACE
    "$<$<CXX_COMPILER_ID:MSVC>:>"
    "$<$<CXX_COMPILER_ID:GNU,Clang>:>"
    "d${vProjName}ProjName=\"${vProjName}\""
    "d${vProjName}ProjVnum=\"${vProjVnum}\""
    "d${vProjName}ProjDesc=\"${vProjDesc}\""
    ) # target_compile_definitions
target_include_directories(${vProjNameL} INTERFACE ${vFileDirSrc})
target_link_libraries(${vProjNameL} INTERFACE fmt::fmt)
# test
enable_testing()
if(1)
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
endif()
if(1)
    add_executable(eTestTime ${vFileDirSrcTest}/eTime.cxx)
    add_test(NAME tTime
        COMMAND ${vFileDirGen}/eTestTime
        ) # add_test
    set_tests_properties(tTime PROPERTIES
        TIMEOUT 16
        ) # set_tests_properties
    target_link_libraries(eTestTime PRIVATE ${vProjNameL})
endif()
# exam
if(1)
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
endif() # eExamArg
if(1)
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
endif() # eExamOpt
if(1)
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
endif() # eExamCmd
message(${CMAKE_ROOT})
