cmake_minimum_required(VERSION 3.20)
# proj
project(lTermArgs
    VERSION 1.2.0
    DESCRIPTION "cli argument parsing single-header-only-library for cxx"
    ) # project
set(vProjName ${PROJECT_NAME})
set(vProjVnum ${PROJECT_VERSION})
# lang
set(vLangVnum 11)
set(CMAKE_CXX_STANDARD ${vLangVnum})
set(CMAKE_CXX_STANDARD_REQUIRED ${vLangVnum})
# file
set(vFileDirLoc ${PROJECT_SOURCE_DIR}/..)
set(vFileDirSrc ${vFileDirLoc}/src)
set(vFileDirGen ${vFileDirLoc}/gen)
# make
if(1)
add_executable(tests
    ${vFileDirSrc}/tests.cxx
    ) # add_executable
add_executable(example1
    ${vFileDirSrc}/example1.cxx
    ) # add_executable
add_executable(example2
    ${vFileDirSrc}/example2.cxx
    ) # add_executable
endif(1)
