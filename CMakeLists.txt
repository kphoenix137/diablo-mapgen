cmake_minimum_required(VERSION 3.22)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(WARNING [[In-source build detected, please eg. create a new directory and use `cmake ..`]])
endif()

project(DiabloMapGen
  VERSION "0.1.0"
  LANGUAGES C CXX)
set(PROJECT_VERSION_WITH_SUFFIX "${VERSION_PREFIX}${VERSION_SUFFIX}")

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -fno-exceptions")
elseif(MSVC)
  string(REGEX REPLACE "/EHsc" "/EHs-c-" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  string(REGEX REPLACE "/GR" "/GR-" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif()

set(CMAKE_CXX_STANDARD 20)

set(BIN_TARGET diablo-mapgen)

add_executable(${BIN_TARGET}
  Source/all.h
  Source/drlg_l1.cpp
  Source/drlg_l2.cpp
  Source/drlg_l3.cpp
  Source/drlg_l4.cpp
  Source/engine.cpp
  Source/funkMapGen.cpp
  Source/gendung.cpp
  #Source/itemdat.cpp
  #Source/items.cpp
  Source/level.cpp
  Source/lighting.cpp
  #Source/monstdat.cpp
  #Source/monster.cpp
  #Source/objdat.cpp
  #Source/objects.cpp
  Source/path.cpp
  Source/quests.cpp
  #Source/stores.cpp
  #Source/themes.cpp
  Source/trigs.cpp
  )

if(CMAKE_STRIP)
  add_custom_command(
    TARGET ${BIN_TARGET} POST_BUILD
    DEPENDS ${BIN_TARGET}
    COMMAND $<$<OR:$<CONFIG:Release>,$<CONFIG:MinSizeRel>>:${CMAKE_STRIP}>
    ARGS $<TARGET_FILE:${BIN_TARGET}>)
endif()
