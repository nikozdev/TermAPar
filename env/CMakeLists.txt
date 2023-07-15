cmake_minimum_required(VERSION 3.20)
# proj
project(lTermArgs
    VERSION 1.0.0
    DESCRIPTION "command line argument parser library for cxx"
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
add_library(${vProjName}
    ${vFileDirSrc}/${vProjName}.cxx
    ) # add_executable
target_compile_options(${vProjName} PRIVATE
    -Wno-format
    ) # target_compile_options
add_executable(tests
    ${vFileDirSrc}/tests.cxx
    ) # add_executable
target_link_libraries(tests
    ${vProjName}
    ) # target_link_libraries
add_executable(example1
    ${vFileDirSrc}/example1.cxx
    ) # add_executable
target_link_libraries(example1
    ${vProjName}
    ) # target_link_libraries
add_executable(example2
    ${vFileDirSrc}/example2.cxx
    ) # add_executable
target_link_libraries(example2 PRIVATE
    ${vProjName}
    ) # target_link_libraries
