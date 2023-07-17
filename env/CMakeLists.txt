cmake_minimum_required(VERSION 3.20)
# proj
project(TermArgs
    VERSION 3.0.2
    DESCRIPTION "cli argument parsing single-header-only-library for cxx"
    ) # project
set(vProjName ${PROJECT_NAME})
set(vProjVnum ${PROJECT_VERSION})
# lang
set(vLangVnum 20)
set(CMAKE_CXX_STANDARD ${vLangVnum})
set(CMAKE_CXX_STANDARD_REQUIRED ${vLangVnum})
# file
set(vFileDirLoc ${PROJECT_SOURCE_DIR}/..)
set(vFileDirSrc ${vFileDirLoc}/src)
set(vFileDirGen ${vFileDirLoc}/gen)
# make
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_library("l${vProjName}" INTERFACE)
target_compile_definitions("l${vProjName}" INTERFACE
    "$<$<CXX_COMPILER_ID:MSVC>:>"
    "$<$<CXX_COMPILER_ID:GNU,Clang>:>"
    dTermArgsProjNameStr=\"${vProjName}\"
    dTermArgsProjVnumStr=\"${vProjVnum}\"
    ) # target_compile_definitions
# test
enable_testing()
if(1)
    add_executable(eTest ${vFileDirSrc}/eTest.cxx)
    add_test(NAME tMain
        COMMAND ${vFileDirGen}/eTest
        WORKING_DIRECTORY ${vFileDirLoc}
        ) # add_test
    set_tests_properties(tMain PROPERTIES
        SKIP_RETURN_CODE 0
        SKIP_REGULAR_EXPRESSION ")=[fTest]"
        SKIP_REGULAR_EXPRESSION ")=[fTestOpt]"
        SKIP_REGULAR_EXPRESSION ")=[fTestCmd]"
        ) # set_tests_properties
endif()
if(1)
    add_executable(eExam1 ${vFileDirSrc}/eExam1.cxx)
endif() # eExam1
if(1)
    add_executable(eExam2 ${vFileDirSrc}/eExam2.cxx)
endif() # eExam2
if(1)
    add_executable(eExam3 ${vFileDirSrc}/eExam3.cxx)
endif() # eExam3
