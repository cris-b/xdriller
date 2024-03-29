#/*
#-----------------------------------------------------------------------------
#Filename:    CMakeLists.txt
#-----------------------------------------------------------------------------
#
#This source file is part of the
#   ___                 __    __ _ _    _ 
#  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
# //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
#/ \_// (_| | | |  __/  \  /\  /| |   <| |
#\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
#      |___/                              
#      Tutorial Framework
#      http://www.ogre3d.org/tikiwiki/
#-----------------------------------------------------------------------------
#*/

cmake_minimum_required(VERSION 3.5)
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)
cmake_policy(SET CMP0003 NEW)

# Use relative paths
# This is mostly to reduce path size for command-line limits on windows

set(OGRE_HOME "/Users/durmieu/Desktop/program/OgreSDK")
set(Boost_INCLUDE_DIR "/Users/durmieu/Desktop/program/OgreSDK/boost_1_42")

if(WIN32)
  # This seems to break Xcode projects so definitely don't enable on Apple builds
  set(CMAKE_USE_RELATIVE_PATHS true)
  set(CMAKE_SUPPRESS_REGENERATION true)
  set(CMAKE_MODULE_PATH "$ENV{OGRE_HOME}/CMake/;${CMAKE_MODULE_PATH}")
  set(OGRE_SAMPLES_INCLUDEPATH
      $ENV{OGRE_HOME}/Samples/include)
endif()

# Assign compiler for Mac OS X-based systems
# NOTE: This logic is order-dependent and must occur here because the
# configuration phase of CMake projects will fall into an infinite loop
# if compiler assignments are modified after issuing a project(...) command.
#
if (OGRE_BUILD_PLATFORM_IPHONE)
  # Force gcc <= 4.2 on iPhone
  include(CMakeForceCompiler)
  CMAKE_FORCE_C_COMPILER(gcc-4.2 GNU)
  CMAKE_FORCE_CXX_COMPILER(gcc-4.2 GNU)
elseif (APPLE AND NOT OGRE_BUILD_PLATFORM_IPHONE)
  # Force gcc <= 4.0 on Mac OS X because 4.2 is not supported prior to Mac OS X 10.5
  include(CMakeForceCompiler)
  CMAKE_FORCE_C_COMPILER(gcc-4.0 GNU)
  CMAKE_FORCE_CXX_COMPILER(gcc-4.0 GNU)
endif ()

set(OGRE_PROJECT_NAME
  "xdriller"
)

project(${OGRE_PROJECT_NAME})

# Include necessary submodules
set(OGRE_SOURCE_DIR "/Users/durmieu/Desktop/program/OgreSrc")
set(CMAKE_MODULE_PATH 
  "${OGRE_SOURCE_DIR}/CMake"
  "${OGRE_SOURCE_DIR}/CMake/Utils" 
  "${OGRE_SOURCE_DIR}/CMake/Packages"
)
set(OGRE_SAMPLES_INCLUDEPATH
  "${OGRE_SOURCE_DIR}/Samples/Common/include"
)
include(CMakeDependentOption)
include(CheckCXXCompilerFlag)

if(UNIX)
  include(GNUInstallDirs)
endif()

set(OGRE_TEMPLATES_DIR "${OGRE_SOURCE_DIR}/CMake/Templates")
 

#####################################################################
# Set up the basic build environment
#####################################################################
 
if (CMAKE_BUILD_TYPE STREQUAL "")
  # CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
  # differentiation between debug and release builds.
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()
 
if (NOT APPLE)
  # Create debug libraries with _d postfix
  set(CMAKE_DEBUG_POSTFIX "_d")
endif ()

# Set compiler specific build flags
if (CMAKE_COMPILER_IS_GNUCXX)
  check_cxx_compiler_flag(-msse OGRE_GCC_HAS_SSE)
  if (OGRE_GCC_HAS_SSE)
    add_definitions(-msse)
  endif ()
endif ()
if (MSVC)
  if (CMAKE_BUILD_TOOL STREQUAL "nmake")
    # set variable to state that we are using nmake makefiles
	set(NMAKE TRUE)
  endif ()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /fp:fast")
  # Enable intrinsics on MSVC in debug mode
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Oi")
  if (CMAKE_CL_64)
    # Visual Studio bails out on debug builds in 64bit mode unless
	# this flag is set...
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /bigobj")
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /bigobj")
  endif ()
endif ()
if (MINGW)
  add_definitions(-D_WIN32_WINNT=0x0500)
endif ()

if (CMAKE_COMPILER_IS_GNUCXX AND NOT MINGW)
  # Test for GCC visibility
  include(CheckCXXCompilerFlag)
  check_cxx_compiler_flag(-fvisibility=hidden OGRE_GCC_VISIBILITY)
  if (OGRE_GCC_VISIBILITY)
    # determine gcc version
    execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion 
      OUTPUT_VARIABLE OGRE_GCC_VERSION)
    message(STATUS "Detected g++ ${OGRE_GCC_VERSION}")
    message(STATUS "Enabling GCC visibility flags")
    set(OGRE_GCC_VISIBILITY_FLAGS "-DOGRE_GCC_VISIBILITY -fvisibility=hidden")
  
    # check if we can safely add -fvisibility-inlines-hidden
    string(TOLOWER "${CMAKE_BUILD_TYPE}" OGRE_BUILD_TYPE)
    if (OGRE_BUILD_TYPE STREQUAL "debug" AND OGRE_GCC_VERSION VERSION_LESS "4.2")
      message(STATUS "Skipping -fvisibility-inlines-hidden due to possible bug in g++ < 4.2")
    else ()
      set(OGRE_GCC_VISIBILITY_FLAGS "${OGRE_GCC_VISIBILITY_FLAGS} -fvisibility-inlines-hidden")
    endif ()
  endif (OGRE_GCC_VISIBILITY)
  
  # Fix x64 issues on Linux
  if("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64" AND NOT APPLE)
    add_definitions(-fPIC)
  endif()  
endif (CMAKE_COMPILER_IS_GNUCXX AND NOT MINGW)

# determine system endianess
#include(TestBigEndian)
#test_big_endian(OGRE_TEST_BIG_ENDIAN)
set(OGRE_TEST_BIG_ENDIAN FALSE)

# Add OgreMain include path
include_directories("${OGRE_SOURCE_DIR}/OgreMain/include")
include_directories("${OGRE_BINARY_DIR}/include")
include_directories("/Users/durmieu/Desktop/program/xdriller/include")
if (APPLE)
  if (OGRE_BUILD_PLATFORM_IPHONE)
    include_directories("${OGRE_SOURCE_DIR}/OgreMain/include/iPhone")

    # Set static early for proper dependency detection
    set(OGRE_STATIC TRUE)
  else ()
    include_directories("${OGRE_SOURCE_DIR}/OgreMain/include/OSX")
  endif ()
endif (APPLE)

include(FindGettext)

find_package(PkgConfig REQUIRED)
 
find_package(OGRE REQUIRED)
 
Find_Package ( SDL2 REQUIRED )
pkg_check_modules(SDL2MIXER REQUIRED SDL2_mixer)
pkg_check_modules(OIS REQUIRED OIS)
pkg_check_modules(TINYXML REQUIRED tinyxml2)

if(APPLE)
Find_Package ( IOKit REQUIRED )
find_library(IOKIT_LIBRARY IOKit)
find_library(OIS_LIBRARY OIS)
find_library(CF_LIBRARY CoreFoundation)
endif()
 
# Find Boost
if (NOT OGRE_BUILD_PLATFORM_IPHONE)
	if (WIN32 OR APPLE)
		set(Boost_USE_STATIC_LIBS TRUE)
	else ()
		# Statically linking boost to a dynamic Ogre build doesn't work on Linux 64bit
		set(Boost_USE_STATIC_LIBS ${OGRE_STATIC})
	endif ()
	if (MINGW)
		# this is probably a bug in CMake: the boost find module tries to look for
		# boost libraries with name libboost_*, but CMake already prefixes library
		# search names with "lib". This is the workaround.
		set(CMAKE_FIND_LIBRARY_PREFIXES ${CMAKE_FIND_LIBRARY_PREFIXES} "")
	endif ()
	set(Boost_ADDITIONAL_VERSIONS "1.42" "1.42.0" "1.41.0" "1.41" "1.40.0" "1.40" "1.39.0" "1.39" "1.38.0" "1.38" "1.37.0" "1.37" )
	# Components that need linking (NB does not include header-only components like bind)
	set(OGRE_BOOST_COMPONENTS thread date_time)
	find_package(Boost COMPONENTS ${OGRE_BOOST_COMPONENTS} QUIET)
	if (NOT Boost_FOUND)
		# Try again with the other type of libs
		set(Boost_USE_STATIC_LIBS NOT ${Boost_USE_STATIC_LIBS})
		find_package(Boost COMPONENTS ${OGRE_BOOST_COMPONENTS} QUIET)
	endif()
	find_package(Boost QUIET)
 
	# Set up referencing of Boost
	include_directories(${Boost_INCLUDE_DIR})
	add_definitions(-DBOOST_ALL_NO_LIB)
	set(OGRE_LIBRARIES ${OGRE_LIBRARIES} ${Boost_LIBRARIES})
endif()
 
set(HDRS
./include/Arrows.h
./include/BackgroundSceneManager.h
./include/Board.h
./include/Brick.h
./include/Cam.h
./include/ConfigManager.h
./include/CreditsState.h
./include/DotScene.h
./include/EditableText.h
./include/EntityMaterialInstance.h
./include/Fader.h
./include/GameManager.h
./include/GameState.h
./include/Gettext.h
./include/Globals.h
./include/HighScoreManager.h
./include/HighScoreState.h
./include/HighScoreTable.h
./include/InputManager.h
./include/IntroState.h
./include/LevelLoader.h
./include/MaterialInstance.h
./include/MenuButton.h
./include/MenuState.h
./include/MovableText.h
./include/Music.h
./include/PauseState.h
./include/PlayState.h
./include/Player.h
./include/PlayerModelSelector.h
./include/Planet.h
./include/RingSwitcher.h
./include/RingSwitcherObject.h
./include/RumbleManager.h
./include/Sound.h
./include/SoundManager.h
./include/SubEntityMaterialInstance.h
./include/SuperBrick.h
./include/TextEffector.h
./include/TextEffectorBigMessage.h
./include/TextGlobe.h
./include/Tools.h

)
 
set(SRCS
./src/Arrows.cpp
./src/BackgroundSceneManager.cpp
./src/Board.cpp
./src/Brick.cpp
./src/Cam.cpp
./src/ConfigManager.cpp
./src/CreditsState.cpp
./src/DotScene.cpp
./src/EditableText.cpp
./src/EntityMaterialInstance.cpp
./src/Fader.cpp
./src/GameManager.cpp
./src/GameState.cpp
./src/HighScoreManager.cpp
./src/HighScoreState.cpp
./src/HighScoreTable.cpp
./src/InputManager.cpp
./src/IntroState.cpp
./src/LevelLoader.cpp
./src/Main.cpp
./src/MaterialInstance.cpp
./src/MenuButton.cpp
./src/MenuState.cpp
./src/MovableText.cpp
./src/Music.cpp
./src/PauseState.cpp
./src/PlayState.cpp
./src/Player.cpp
./src/PlayerModelSelector.cpp
./src/Planet.cpp
./src/RingSwitcher.cpp
./src/RingSwitcherObject.cpp
./src/RumbleManager.cpp
./src/Sound.cpp
./src/SoundManager.cpp
./src/SubEntityMaterialInstance.cpp
./src/SuperBrick.cpp
./src/TextEffector.cpp
./src/TextEffectorBigMessage.cpp
./src/TextGlobe.cpp
./src/Tools.cpp

)
 
include_directories( ${OIS_INCLUDE_DIRS}
	${OGRE_INCLUDE_DIRS}
        ${SDL2_INCLUDE_DIRS}
        ${SDL2MIXER_INCLUDE_DIRS}
	${OGRE_SAMPLES_INCLUDEPATH}
        "./include"
)
 
add_executable(${OGRE_PROJECT_NAME} WIN32 ${HDRS} ${SRCS})
 
set_target_properties(${OGRE_PROJECT_NAME} PROPERTIES DEBUG_POSTFIX _d)
 
target_link_libraries(${OGRE_PROJECT_NAME} ${OGRE_LIBRARIES} ${CF_LIBRARY} ${IOKIT_LIBRARIES} ${OIS_LIBRARIES} ${SDL2_LIBRARIES} ${SDL2MIXER_LIBRARIES} ${TINYXML_LIBRARIES} )

install(TARGETS ${OGRE_PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})

install(DIRECTORY media DESTINATION ${CMAKE_INSTALL_DATADIR}/${OGRE_PROJECT_NAME})
install(FILES ${OGRE_PROJECT_NAME}.desktop DESTINATION ${CMAKE_INSTALL_DATADIR}/applications/)
install(FILES ${OGRE_PROJECT_NAME}.png DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/64x64/apps/)
add_subdirectory(locale)
add_subdirectory(default_config)

if(WIN32) 
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/dist/bin)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/dist/media)
endif()
 
# post-build copy for win32
if(WIN32 AND NOT MINGW)
	add_custom_command( TARGET ${OGRE_PROJECT_NAME} PRE_BUILD
		COMMAND if not exist .\\dist\\bin mkdir .\\dist\\bin )
	add_custom_command( TARGET ${OGRE_PROJECT_NAME} POST_BUILD
		COMMAND copy \"$(TargetPath)\" .\\dist\\bin )
endif(WIN32 AND NOT MINGW)
if(MINGW)
	set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/dist/bin)
endif(MINGW)

if(UNIX)
        install(TARGETS ${OGRE_PROJECT_NAME}
                RUNTIME DESTINATION bin
                CONFIGURATIONS All)
endif()
 
if(WIN32)
 
	install(TARGETS ${OGRE_PROJECT_NAME}
		RUNTIME DESTINATION bin
		CONFIGURATIONS All)
 
	install(DIRECTORY ${CMAKE_SOURCE_DIR}/dist/Media
		DESTINATION ./
		CONFIGURATIONS Release RelWithDebInfo Debug
	)
 
	install(FILES ${CMAKE_SOURCE_DIR}/dist/bin/plugins.cfg
		${CMAKE_SOURCE_DIR}/dist/bin/resources.cfg
		DESTINATION bin
		CONFIGURATIONS Release RelWithDebInfo
	)
 
	install(FILES ${CMAKE_SOURCE_DIR}/dist/bin/plugins_d.cfg
		${CMAKE_SOURCE_DIR}/dist/bin/resources_d.cfg
		DESTINATION bin
		CONFIGURATIONS Debug
	)
 
	install(FILES ${OGRE_PLUGIN_DIR_REL}/OgreMain.dll
		${OGRE_PLUGIN_DIR_REL}/RenderSystem_Direct3D9.dll
		${OGRE_PLUGIN_DIR_REL}/RenderSystem_GL.dll
		${OGRE_PLUGIN_DIR_REL}/OIS.dll
		DESTINATION bin
		CONFIGURATIONS Release RelWithDebInfo
	)
 
	install(FILES ${OGRE_PLUGIN_DIR_DBG}/OgreMain_d.dll
		${OGRE_PLUGIN_DIR_DBG}/RenderSystem_Direct3D9_d.dll
		${OGRE_PLUGIN_DIR_DBG}/RenderSystem_GL_d.dll
		${OGRE_PLUGIN_DIR_DBG}/OIS_d.dll
		DESTINATION bin
		CONFIGURATIONS Debug
	)
endif(WIN32)
