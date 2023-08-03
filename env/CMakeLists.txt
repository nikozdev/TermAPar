cmake_minimum_required(VERSION 3.3...3.8)
include_guard(GLOBAL)
# mods
include(dCmake.dir/dHelp.dir/fRegex.cmake)
include(dCmake.dir/dHelp.dir/fOpt.cmake)
include(dCmake.dir/dHelp.dir/fPack.cmake)
# proj
project(TermAPar
  VERSION 4.1.0
  DESCRIPTION
  "single-header-only-library for cli argument parsing in cxx"
  ) # project
set(vProjName ${PROJECT_NAME})
set(vProjNameLib "l${vProjName}")
set(vProjNameExe "e${vProjName}")
set(vProjNameDir "d${vProjName}.dir")
set(vProjNameDst "p${vProjName}")
set(vProjNameDef "d${vProjName}")
set(vProjVnum ${PROJECT_VERSION})
set(vProjVnumMajor ${PROJECT_VERSION_MAJOR})
set(vProjVnumMinor ${PROJECT_VERSION_MINOR})
set(vProjVnumMicro ${PROJECT_VERSION_PATCH})
set(vProjDesc ${PROJECT_DESCRIPTION})
# lang
enable_language(CXX)
set(vLangVnum 23)
set(CMAKE_CXX_STANDARD ${vLangVnum})
set(CMAKE_CXX_STANDARD_REQUIRED ${vLangVnum})
# osys
mark_as_advanced(FORCE CMAKE_OSX_ARCHITECTURES)
mark_as_advanced(FORCE CMAKE_OSX_DEPLOYMENT_TARGET)
mark_as_advanced(FORCE CMAKE_OSX_SYSROOT)
# file
set(vFdirLocRoot "${PROJECT_SOURCE_DIR}/..")
set(vFdirSrcRoot "${vFdirLocRoot}/src")
set(vFdirSrcBase "${vFdirSrcRoot}/${vProjNameDir}")
set(vFdirGenRoot "${vFdirLocRoot}/gen")
set(vFdirEnvRoot "${vFdirLocRoot}/env")
set(vFdirDocRoot ${vFdirLocRoot}/doc)
set(vFdirDstRoot "$ENV{HOME}$ENV{UserProfile}/.local")
set(vFdirDstBins "${vFdirDstRoot}/bin")
set(vFdirDstLibs "${vFdirDstRoot}/lib")
set(vFdirDstHead "${vFdirDstRoot}/include")
set(CMAKE_PREFIX_PATH "${vFdirDstRoot}" CACHE PATH "destination prefix" FORCE)
set(CMAKE_INSTALL_PREFIX "${vFdirDstRoot}" CACHE PATH "install prefix" FORCE)
# conf
set(oProjConfPick "Work")
set(vProjConfList "Work;Play;Dist;")
set(vProjConfDesc "controlled by oProjConfPick")
fOptEnum(oProjConfPick vProjConfList "project configuration")
if(oProjConfPick STREQUAL "")
  message("project configuration is not defined")
elseif(oProjConfPick STREQUAL "Work")
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "${vProjConfDesc}" FORCE)
elseif(oProjConfPick STREQUAL "Play")
  set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "${vProjConfDesc}" FORCE)
elseif(oProjConfPick STREQUAL "Dist")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "${vProjConfDesc}" FORCE)
else()
  message("project configuration is not handled")
endif(oProjConfPick STREQUAL "")
fOptEnum(CMAKE_BUILD_TYPE CMAKE_CONFIGURATION_TYPES ${vProjConfDesc})
mark_as_advanced(FORCE CMAKE_BUILD_TYPE)
# make
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
option(oMakeExe "make project executable" 1)
option(oTestExe "test project executable" 1)
##tool
set(vMakeToolGnucc "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(vMakeToolMsvcc "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
## lib
add_library(${vProjNameLib} INTERFACE)
set(vMakeDefsLib
  "${vProjNameDef}ProjName=\"${vProjName}\"" "dProjName=\"${vProjName}\""
  "${vProjNameDef}ProjNameExe=\"${vProjNameExe}\"" "dProjNameExe=\"${vProjNameExe}\""
  "${vProjNameDef}ProjNameLib=\"${vProjNameLib}\"" "dProjNameLib=\"${vProjNameLib}\""
  "${vProjNameDef}ProjVnum=\"${vProjVnum}\"" "dProjVnum=\"${vProjVnum}\""
  "${vProjNameDef}ProjDesc=\"${vProjDesc}\"" "dProjDesc=\"${vProjDesc}\""
  "${vProjNameDef}ProjConf${oProjConfPick}" "dProjConf${oProjConfPick}"
  "${vProjNameDef}MakeExe=${oMakeExe}"
  "${vProjNameDef}TestExe=${oTestExe}"
  ) # vMakeDefsLib
set(vMakeOptsLib
  "$<${vMakeToolGnucc}:-fpermissive;-Wno-macro-redefined;-Wno-class-conversion>"
  "$<${vMakeToolMsvcc}:>"
  ) # vMakeOptsLib
### libs
set(vLibsList "")
#### format
set(oLibsPickFormat "Fmt")
set(vLibsListFormat "Std;Fmt;")
fOptEnum(oLibsPickFormat vLibsListFormat  "library pick for formatting")
if(oLibsPickFormat STREQUAL "")
  message(WARNING "library pick for format is not defined")
elseif(oLibsPickFormat STREQUAL "Fmt")
  fPackFind("fmt" "")
  target_link_libraries(${vProjNameLib} INTERFACE fmt::fmt)
  list(APPEND vLibsList "fmt;")
elseif(oLibsPickFormat STREQUAL "Std")
else()
  message(WARNING "library pick for format is not handled")
endif(oLibsPickFormat STREQUAL "")
list(APPEND vMakeDefsLib "d${vProjName}LibsPickFormat${oLibsPickFormat}")
### last
target_compile_options(${vProjNameLib} INTERFACE ${vMakeOptsLib})
target_compile_definitions(${vProjNameLib} INTERFACE ${vMakeDefsLib})
target_include_directories(${vProjNameLib} INTERFACE
  "$<BUILD_INTERFACE:${vFdirSrcRoot}>;"
  "$<INSTALL_INTERFACE:${vFdirDstHead}>;"
  ) # target_include_directories
target_sources(${vProjNameLib}
  INTERFACE FILE_SET HEADERS
  BASE_DIRS "${vFdirSrcBase}"
  FILES "${vFdirSrcBase}/fHead.hxx"
  ) # target_sources
## exe
if(oMakeExe)
  add_executable(${vProjNameExe} ${vFdirSrcBase}/fMain.cxx)
  # test
  if(oTestExe)
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
  endif(oTestExe)
  # libs
  target_link_libraries(${vProjNameExe} PRIVATE ${vProjNameLib})
endif(oMakeExe)
# pack
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${vFdirDocRoot}/license.md")
set(CPACK_PACKAGE_VERSION_MAJOR "${vProjVnumMajor}")
set(CPACK_PACKAGE_VERSION_MINOR "${vProjVnumMinor}")
set(CPACK_PACKAGE_VERSION_PATCH "${vProjVnumMicro}")
include(CPack)
set(vFdirDstProj "${vFdirDstLibs}/cmake/${vProjNameDst}")
string(APPEND vFdirDstProj "-${vProjVnumMajor}")
string(APPEND vFdirDstProj "-${vProjVnumMinor}")
string(APPEND vFdirDstProj "-${vProjVnumMicro}")
string(APPEND vFdirDstProj ".dir")
include(CMakePackageConfigHelpers)
configure_package_config_file(
  "${vFdirEnvRoot}/dCMake.dir/dPack.dir/fConfig.cmake.in"
  "${vFdirGenRoot}/${vProjNameDst}-Config.cmake"
  INSTALL_DESTINATION "${vFdirDstProj}"
  ) # package Config
write_basic_package_version_file(
  "${vFdirGenRoot}/${vProjNameDst}-ConfigVersion.cmake"
  VERSION ${vProjVnum}
  COMPATIBILITY SameMajorVersion
  ) # package ConfigVersion
install(
  TARGETS ${vProjNameLib} EXPORT ${vProjNameDst}
  FILE_SET HEADERS DESTINATION ${vFdirDstHead}/${vProjNameDir}
  ) # install target with header
install(EXPORT "${vProjNameDst}"
  FILE "${vProjNameDst}.cmake"
  DESTINATION "${vFdirDstProj}"
  NAMESPACE "${vProjNameDst}::"
  ) # install Target
install(FILES
  "${vFdirGenRoot}/${vProjNameDst}-Config.cmake"
  "${vFdirGenRoot}/${vProjNameDst}-ConfigVersion.cmake"
  DESTINATION "${vFdirDstProj}"
  ) # install ConfigVersion
export(EXPORT "${vProjNameDst}"
  FILE "${vFdirGenRoot}/${vProjNameDst}.cmake"
  NAMESPACE "${vProjNameDst}::"
  ) # export Target
