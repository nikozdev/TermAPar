cmake_minimum_required(VERSION 3.20)
# proj
project(lTermArgs
    VERSION 1.2.0
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
