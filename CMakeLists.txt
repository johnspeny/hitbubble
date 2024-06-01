#/****************************************************************************
# Copyright (c) 2013-2014 cocos2d-x.org
# Copyright (c) 2019-present Axmol Engine contributors (see AUTHORS.md).
#
# https://axmolengine.github.io/
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
# ****************************************************************************/

cmake_minimum_required(VERSION 3.10)

set(APP_NAME HitBubble)

project(${APP_NAME})

if(XCODE)
    set(CMAKE_XCODE_GENERATE_TOP_LEVEL_PROJECT_ONLY TRUE)
endif()

# config quick starter batch script run.bat for windows
if(WIN32)
    file(RELATIVE_PATH CMAKE_BUILD_RELATIVE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" "${PROJECT_BINARY_DIR}")
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/run.bat.in" "${CMAKE_CURRENT_SOURCE_DIR}/run.bat" @ONLY)
endif()

set(_is_axmol_embed FALSE)
if (IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/axmol")
    set(_AX_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/axmol")
    set(_is_axmol_embed TRUE)
    message(STATUS "Building isolated project: ${APP_NAME}")
else()
    set(_AX_ROOT "$ENV{AX_ROOT}")
    if(NOT (_AX_ROOT STREQUAL ""))
        file(TO_CMAKE_PATH ${_AX_ROOT} _AX_ROOT)
        message(STATUS "Using system env var _AX_ROOT=${_AX_ROOT}")
    else()
        message(FATAL_ERROR "Please run setup.ps1 add system env var 'AX_ROOT' to specific the engine root")
    endif()
endif()

set(CMAKE_MODULE_PATH ${_AX_ROOT}/cmake/Modules/)

include(AXBuildSet)

set(_AX_USE_PREBUILT FALSE)

if(NOT _is_axmol_embed)
    if ((WIN32 OR LINUX) AND DEFINED AX_PREBUILT_DIR AND IS_DIRECTORY ${_AX_ROOT}/${AX_PREBUILT_DIR})
        set(_AX_USE_PREBUILT TRUE)
    endif()
endif()

if (NOT _AX_USE_PREBUILT)
    add_subdirectory(${_AX_ROOT}/core ${ENGINE_BINARY_PATH}/axmol/core)
endif()

# The common cross-platforms source files and header files
list(APPEND GAME_SOURCE
        Source/AppDelegate.cpp
        Source/DynamicEntity.cpp
        Source/StaticEntity.cpp
        Source/Audio.cpp
        Source/GameUtils.cpp

        # helpers
        Source/helpers/box2d/b2BodyBuilder.cpp
        Source/helpers/box2d/b2DebugRenderer.cpp
        Source/helpers/box2d/GLESDebugDraw.cpp
        Source/helpers/box2d/QueryCallback.cpp
        Source/helpers/box2d/PhysicsShapeCache.cpp

        # particles
        source/particlez/DustParticle.cpp
        source/particlez/FireWorksParticle.cpp
        source/particlez/SmokeyParticle.cpp

        # entity
        Source/entity/Wall.cpp
        Source/entity/Board.cpp
        Source/entity/LinkedChain.cpp

        # actor
        Source/actor/character/Character.cpp
        Source/actor/character/Head.cpp
        Source/actor/character/Torso.cpp
        Source/actor/character/Hair.cpp
        Source/actor/Ball.cpp
        Source/actor/Coin.cpp
        Source/actor/item/Item.cpp
        Source/actor/item/Fruit.cpp

        # scene
        Source/scene/WorldContactListener.cpp
        Source/scene/GameScene.cpp
        Source/scene/SplashScene.cpp
        Source/scene/SceneManager.cpp

        # ui
        source/ui/hud/Hud.cpp
        source/ui/hud/Heart.cpp
        source/ui/hud/ProgressUI.cpp
        source/ui/hud/TopContainer.cpp
        source/ui/ToolBar.cpp
        source/ui/BottomNavigationBar.cpp

        # layers
        source/layers/MainMenuLayer.cpp
        source/layers/ReviveGameLayer.cpp
        source/layers/GameOverLayer.cpp
        source/layers/VictoryGameLayer.cpp

        # data
        source/season/Season.cpp
        )

list(APPEND GAME_HEADER
        Source/AppDelegate.h
        Source/MyEnums.h
        Source/Constants.h
        Source/DynamicEntity.h
        Source/StaticEntity.h
        Source/Audio.h
        Source/GameUtils.h

        # helpers
        Source/helpers/box2d/b2BodyBuilder.h
        Source/helpers/box2d/b2DebugRenderer.h
        Source/helpers/box2d/GLESDebugDraw.h
        Source/helpers/box2d/QueryCallback.h
        Source/helpers/box2d/PhysicsShapeCache.h
        Source/helpers/Logger.h
        Source/helpers/string_manipulation/base64.h
        Source/helpers/string_manipulation/zlibString.h
        Source/helpers/short_types.h

        # particles
        source/particlez/DustParticle.h
        source/particlez/FireWorksParticle.h
        source/particlez/SmokeyParticle.h

        # entity
        Source/entity/Wall.h
        Source/entity/Board.h
        Source/entity/LinkedChain.h

        #actor
        Source/actor/character/Character.h
        Source/actor/character/Head.h
        Source/actor/character/Torso.h
        Source/actor/character/Hair.h
        Source/actor/Ball.h
        Source/actor/Coin.h
        Source/actor/item/Item.h
        Source/actor/item/Fruit.h


        # scene
        Source/scene/CollisionEvent.h
        Source/scene/WorldContactListener.h
        Source/scene/GameScene.h
        Source/scene/SplashScene.h
        Source/scene/SceneManager.h

        # ui
        source/ui/hud/Hud.h
        source/ui/hud/Heart.h
        source/ui/hud/ProgressUI.h
        source/ui/hud/TopContainer.h
        source/ui/ToolBar.h
        source/ui/BottomNavigationBar.h

        # layers
        source/layers/MainMenuLayer.h
        source/layers/ReviveGameLayer.h
        source/layers/GameOverLayer.h
        source/layers/VictoryGameLayer.h

        # data
        source/season/Season.h
)


set(GAME_INC_DIRS
  "${CMAKE_CURRENT_SOURCE_DIR}/Source"
)

set(content_folder
    "${CMAKE_CURRENT_SOURCE_DIR}/Content"
    )

if(APPLE)
    ax_mark_multi_resources(common_content_files RES_TO "Resources" FOLDERS ${content_folder})
elseif(WINDOWS)
    ax_mark_multi_resources(common_content_files RES_TO "Content" FOLDERS ${content_folder})
endif()

if(ANDROID)
    # the APP_NAME should match on AndroidManifest.xml
    list(APPEND GAME_SOURCE
         proj.android/app/jni/main.cpp
         )
elseif(LINUX)
    list(APPEND GAME_SOURCE
         proj.linux/main.cpp
         )
    list(APPEND GAME_SOURCE ${common_content_files})
elseif(WASM)
    list(APPEND GAME_SOURCE
         proj.wasm/main.cpp
         )
    list(APPEND GAME_SOURCE ${common_content_files})
elseif(WINDOWS)
    if(NOT WINRT)
        list(APPEND GAME_HEADER
            proj.win32/main.h
            proj.win32/resource.h
            )
        list(APPEND GAME_SOURCE
            proj.win32/main.cpp
            proj.win32/game.rc
            )
    else()
        ax_setup_winrt_sources()
    endif()
    list(APPEND GAME_SOURCE ${common_content_files})
elseif(APPLE)
    if(IOS)
        list(APPEND GAME_HEADER
             proj.ios_mac/ios/AppController.h
             proj.ios_mac/ios/RootViewController.h
             )

        if (TVOS)
            set(APP_UI_RES
                proj.ios_mac/ios/targets/tvos/LaunchScreenBackground.png
                proj.ios_mac/ios/targets/tvos/LaunchScreen.storyboard
                proj.ios_mac/ios/targets/tvos/Images.xcassets
                )
        else()
            set(APP_UI_RES
                proj.ios_mac/ios/targets/ios/LaunchScreenBackground.png
                proj.ios_mac/ios/targets/ios/LaunchScreen.storyboard
                proj.ios_mac/ios/targets/ios/Images.xcassets
                )
        endif()

        list(APPEND GAME_SOURCE
             proj.ios_mac/ios/main.m
             proj.ios_mac/ios/AppController.mm
             proj.ios_mac/ios/RootViewController.mm
             proj.ios_mac/ios/Prefix.pch
             ${APP_UI_RES}
             )
    elseif(MACOSX)
        set(APP_UI_RES
            proj.ios_mac/mac/Icon.icns
            )
        list(APPEND GAME_SOURCE
             proj.ios_mac/mac/main.cpp
             proj.ios_mac/mac/Prefix.pch
             ${APP_UI_RES}
             )
    endif()
    list(APPEND GAME_SOURCE ${common_content_files})
endif()

# mark app complie info and libs info
set(APP_SOURCES
    ${GAME_HEADER}
    ${GAME_SOURCE}
    )
if(NOT ANDROID)
    add_executable(${APP_NAME} ${APP_SOURCES})
else()
    add_library(${APP_NAME} SHARED ${APP_SOURCES})
    # whole archive for jni
    target_link_libraries(${APP_NAME} -Wl,--whole-archive cpp_android_spec -Wl,--no-whole-archive)
    config_android_shared_libs("org.axmol.lib" "${CMAKE_CURRENT_SOURCE_DIR}/proj.android/app/src")
endif()

# The optional thirdparties(not dependent by engine)
if (AX_WITH_YAML_CPP)
    list(APPEND GAME_INC_DIRS "${_AX_ROOT}/3rdparty/yaml-cpp/include")
endif()

target_include_directories(${APP_NAME} PRIVATE ${GAME_INC_DIRS})


# mark app resources, resource will be copy auto after mark
ax_setup_app_config(${APP_NAME})
if (NOT _AX_USE_PREBUILT)
    target_link_libraries(${APP_NAME} ${_AX_CORE_LIB})
endif()

if(APPLE)
    set_target_properties(${APP_NAME} PROPERTIES RESOURCE "${APP_UI_RES}")
    set_xcode_property(${APP_NAME} INSTALL_PATH "\$(LOCAL_APPS_DIR)")
    set_xcode_property(${APP_NAME} PRODUCT_BUNDLE_IDENTIFIER "org.axmol.hitbubble")

    if(MACOSX)
        set_target_properties(${APP_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/proj.ios_mac/mac/Info.plist")
    elseif(TVOS)
        set_target_properties(${APP_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/proj.ios_mac/ios/targets/tvos/Info.plist")
        set_xcode_property(${APP_NAME} ASSETCATALOG_COMPILER_APPICON_NAME "Brand Assets")
    elseif(IOS)
        set_target_properties(${APP_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/proj.ios_mac/ios/targets/ios/Info.plist")
        set_xcode_property(${APP_NAME} ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon")
        set_xcode_property(${APP_NAME} TARGETED_DEVICE_FAMILY "1,2")
    endif()

    # For code-signing, set the DEVELOPMENT_TEAM:
    #set_xcode_property(${APP_NAME} DEVELOPMENT_TEAM "GRLXXXX2K9")
elseif(WINDOWS)
    if(NOT _AX_USE_PREBUILT)
        ax_sync_target_dlls(${APP_NAME})
    endif()
endif()

if ((NOT APPLE) AND (NOT WINRT))
    ax_get_resource_path(APP_RES_DIR ${APP_NAME})
    ax_sync_target_res(${APP_NAME} LINK_TO ${APP_RES_DIR} FOLDERS ${content_folder} SYM_LINK 1)

    if((WINDOWS AND (NOT (CMAKE_GENERATOR MATCHES "Ninja"))))
        set_property(TARGET ${APP_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${content_folder}")
        if(NOT DEFINED BUILD_ENGINE_DONE)
            set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${APP_NAME})
        endif()
    endif()
elseif(WINRT)
    if(NOT DEFINED BUILD_ENGINE_DONE)
        set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${APP_NAME})
    endif()
endif()

if (_AX_USE_PREBUILT) # support windows and linux
    use_ax_compile_define(${APP_NAME})

    include(AXLinkHelpers)
    ax_link_cxx_prebuilt(${APP_NAME} ${_AX_ROOT} ${AX_PREBUILT_DIR})
endif()

ax_setup_app_props(${APP_NAME})
