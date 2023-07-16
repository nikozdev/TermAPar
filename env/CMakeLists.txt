cmake_minimum_required(VERSION 3.20)
# proj
project(lTermArgs
    VERSION 2.1.0
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
add_library(${vProjName} INTERFACE)
target_compile_definitions(${vProjName} INTERFACE
    "$<$<CXX_COMPILER_ID:MSVC>:>"
    "$<$<CXX_COMPILER_ID:GNU,Clang>:>"
    dTermArgsProjNameStr=\"${vProjName}\"
    dTermArgsProjVnumStr=\"${vProjVnum}\"
    ) # target_compile_definitions
if(1)
    add_executable(eTest
        ${vFileDirSrc}/eTest.cxx
        ) # add_executable
endif() # eTest
if(1)
    add_executable(eExam1
        ${vFileDirSrc}/eExam1.cxx
        ) # add_executable
endif() # eExam1
if(1)
    add_executable(eExam2
        ${vFileDirSrc}/eExam2.cxx
        ) # add_executable
endif() # eExam2
if(1)
    add_executable(eExam3
        ${vFileDirSrc}/eExam3.cxx
        ) # add_executable
endif() # eExam3
