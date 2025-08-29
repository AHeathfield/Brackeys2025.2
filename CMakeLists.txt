cmake_minimum_required(VERSION 3.16)
project(Brackeys2025_2)

# Set C++ standard to 17 (or higher) for all targets
set(CMAKE_CXX_STANDARD 17)          # Use C++17
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # Enforce the standard (no fallback)
set(CMAKE_CXX_EXTENSIONS OFF)        # Disable compiler-specific extensions (e.g., GNU extensions)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_CXX_EXTENSIONS OFF)        # Disable compiler-specific extensions (e.g., GNU extensions)


# It makes sure the SDL libs and game works with MacOS versions at least 11.0
if (APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0" CACHE STRING "Minimum macOS version" FORCE)
endif()


# Use vendored libs
set(SDLTTF_VENDORED ON)

# This assumes the SDL source is available in vendored/SDL
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

# This assumes the SDL_image source is available in vendored/SDL_image
add_subdirectory(vendored/SDL_image EXCLUDE_FROM_ALL)

# This assumes the SDL_ttf source is available in vendored/SDL_ttf
add_subdirectory(vendored/SDL_ttf EXCLUDE_FROM_ALL)

# This assumes the SDL_mixer source is available in vendored/SDL_mixer
add_subdirectory(vendored/SDL_mixer EXCLUDE_FROM_ALL)


# This creates a var called sources which will contain all the cpp files we are compiling
set(SRCS 
    game.cpp
    src/Core/Utility.cpp
    src/Core/Camera.cpp
    src/Components/Buttons/StateChangerButton.cpp
    src/Components/Colliders/PlayerCollider.cpp
    src/Components/Colliders/TileCollider.cpp
    src/Components/Colliders/BiscuitCollider.cpp
    src/Systems/RenderSystem.cpp
    src/Systems/PhysicsSystem.cpp
    src/Systems/CollisionSystem.cpp
    src/Systems/ScoreSystem.cpp
    src/Systems/AnimationSystem.cpp
    src/Systems/AudioSystem.cpp
    src/Systems/MoveSystem.cpp
    # src/States/State.cpp
    src/States/TitleState.cpp
    src/States/PlayState.cpp
    # src/Biomes/StartingBiome.cpp
    # src/Biomes/ForestBiome.cpp
    # src/Biomes/CityBiome.cpp
    # src/Biomes/DesertBiome.cpp
)

# Creating the game Executables

# For apple, the game.app is actually a folder, in that folder it's common to have:
# A Frameworks folder which contains libs
# Resources folder which contains assets
# MacOS folder which contains actual exe
if (APPLE)
    # Set minimum macOS version when compiling
    # set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0" CACHE STRING "Minimum macOS version" FORCE)

    # Create macOS .app bundle
    add_executable(game MACOSX_BUNDLE ${SRCS})

    # Tell macOS where to look for dynamic libs in the bundle
    set_target_properties(game PROPERTIES
        INSTALL_RPATH "@executable_path/../Frameworks"
        BUILD_WITH_INSTALL_RPATH TRUE
        MACOSX_RPATH ON
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/Info.plist"  # optional custom plist
    )

    # Copy SDL dylibs into the bundle Frameworks folder
    add_custom_command(TARGET game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
            "$<TARGET_BUNDLE_CONTENT_DIR:game>/Frameworks"
        COMMAND ${CMAKE_COMMAND} -E copy
            "$<TARGET_FILE:SDL3::SDL3>"
            "$<TARGET_BUNDLE_CONTENT_DIR:game>/Frameworks/"
        COMMAND ${CMAKE_COMMAND} -E copy
            "$<TARGET_FILE:SDL3_image::SDL3_image>"
            "$<TARGET_BUNDLE_CONTENT_DIR:game>/Frameworks/"
        COMMAND ${CMAKE_COMMAND} -E copy
            "$<TARGET_FILE:SDL3_ttf::SDL3_ttf>"
            "$<TARGET_BUNDLE_CONTENT_DIR:game>/Frameworks/"
        COMMAND ${CMAKE_COMMAND} -E copy
            "$<TARGET_FILE:SDL3_mixer::SDL3_mixer>"
            "$<TARGET_BUNDLE_CONTENT_DIR:game>/Frameworks/"
        COMMENT "Copying SDL libraries to Frameworks"
    )

    # Create version symlinks
    add_custom_command(TARGET game POST_BUILD
        COMMAND cd "$<TARGET_BUNDLE_CONTENT_DIR:game>/Frameworks" &&
                ln -sf libSDL3_mixer.0.0.0.dylib libSDL3_mixer.0.dylib &&
                ln -sf libSDL3_mixer.0.dylib libSDL3_mixer.dylib &&
                ln -sf libSDL3_ttf.0.3.0.dylib libSDL3_ttf.0.dylib &&
                ln -sf libSDL3_ttf.0.dylib libSDL3_ttf.dylib &&
                ln -sf libSDL3_image.0.3.0.dylib libSDL3_image.0.dylib &&
                ln -sf libSDL3_image.0.dylib libSDL3_image.dylib &&
                ln -sf libSDL3.0.dylib libSDL3.dylib
    )

    # Fix SDL install names to use @rpath inside the bundle
    add_custom_command(TARGET game POST_BUILD
        COMMAND install_name_tool -id "@rpath/$<TARGET_FILE_NAME:SDL3::SDL3>"
            "$<TARGET_BUNDLE_CONTENT_DIR:game>/Frameworks/$<TARGET_FILE_NAME:SDL3::SDL3>"
        COMMAND install_name_tool -id "@rpath/$<TARGET_FILE_NAME:SDL3_image::SDL3_image>"
            "$<TARGET_BUNDLE_CONTENT_DIR:game>/Frameworks/$<TARGET_FILE_NAME:SDL3_image::SDL3_image>"
        COMMAND install_name_tool -id "@rpath/$<TARGET_FILE_NAME:SDL3_ttf::SDL3_ttf>"
            "$<TARGET_BUNDLE_CONTENT_DIR:game>/Frameworks/$<TARGET_FILE_NAME:SDL3_ttf::SDL3_ttf>"
        COMMAND install_name_tool -id "@rpath/$<TARGET_FILE_NAME:SDL3_mixer::SDL3_mixer>"
            "$<TARGET_BUNDLE_CONTENT_DIR:game>/Frameworks/$<TARGET_FILE_NAME:SDL3_mixer::SDL3_mixer>"
        COMMENT "Setting SDL install names for @rpath"
    )

    # Fix the game binary's references to SDL libs
    add_custom_command(TARGET game POST_BUILD
        COMMAND install_name_tool -change "$<TARGET_FILE:SDL3::SDL3>"
            "@rpath/$<TARGET_FILE_NAME:SDL3::SDL3>" "$<TARGET_BUNDLE_CONTENT_DIR:game>/MacOS/game"
        COMMAND install_name_tool -change "$<TARGET_FILE:SDL3_image::SDL3_image>"
            "@rpath/$<TARGET_FILE_NAME:SDL3_image::SDL3_image>" "$<TARGET_BUNDLE_CONTENT_DIR:game>/MacOS/game"
        COMMAND install_name_tool -change "$<TARGET_FILE:SDL3_ttf::SDL3_ttf>"
            "@rpath/$<TARGET_FILE_NAME:SDL3_ttf::SDL3_ttf>" "$<TARGET_BUNDLE_CONTENT_DIR:game>/MacOS/game"
        COMMAND install_name_tool -change "$<TARGET_FILE:SDL3_mixer::SDL3_mixer>"
            "@rpath/$<TARGET_FILE_NAME:SDL3_mixer::SDL3_mixer>" "$<TARGET_BUNDLE_CONTENT_DIR:game>/MacOS/game"
        COMMENT "Fixing library paths in game binary"
    )

    # Copy assets into Contents/Resources (Finder-safe)
    add_custom_command(TARGET game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
            "$<TARGET_BUNDLE_CONTENT_DIR:game>/Resources/src/Assets"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/src/Assets"
            "$<TARGET_BUNDLE_CONTENT_DIR:game>/Resources/src/Assets"
        COMMENT "Copying game assets to Resources"
    )
    
else()
    # WIN32 is needed for windows, it is ignored for linux
    add_executable(game WIN32 ${SRCS})

    # Setting paths
    set_target_properties(game PROPERTIES
        BUILD_RPATH "$ORIGIN/lib"
        INSTALL_RPATH "$ORIGIN/lib"
    )

    # Linux-only: copy .so files into libs folder
    if (UNIX AND NOT APPLE)
        add_custom_command(TARGET game POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:game>/libs"
            COMMAND ${CMAKE_COMMAND} -E copy
                "$<TARGET_FILE:SDL3::SDL3>"
                "$<TARGET_FILE_DIR:game>/libs/"
            COMMAND ${CMAKE_COMMAND} -E copy
                "$<TARGET_FILE:SDL3_image::SDL3_image>"
                "$<TARGET_FILE_DIR:game>/libs/"
            COMMAND ${CMAKE_COMMAND} -E copy
                "$<TARGET_FILE:SDL3_ttf::SDL3_ttf>"
                "$<TARGET_FILE_DIR:game>/libs/"
            COMMAND ${CMAKE_COMMAND} -E copy
                "$<TARGET_FILE:SDL3_mixer::SDL3_mixer>"
                "$<TARGET_FILE_DIR:game>/libs/"
            COMMENT "Copying SDL shared libraries to libs folder for Linux"
        )

            # Create version symlinks
        add_custom_command(TARGET game POST_BUILD
            COMMAND cd "$<TARGET_FILE_DIR:game>/libs" &&
                    ln -sf libSDL3_mixer.so.0.0.0 libSDL3_mixer.so.0 &&
                    ln -sf libSDL3_mixer.so.0 libSDL3_mixer.so &&
                    ln -sf libSDL3_ttf.so.0.3.0 libSDL3_ttf.so.0 &&
                    ln -sf libSDL3_ttf.so.0 libSDL3_ttf.so &&
                    ln -sf libSDL3_image.so.0.3.0 libSDL3_image.so.0 &&
                    ln -sf libSDL3_image.so.0 libSDL3_image.so &&
                    ln -sf libSDL3.so.0 libSDL3.so
        )
    endif()


    # Copy assets next to the built executable
    add_custom_command(TARGET game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
            "$<TARGET_FILE_DIR:game>/src/Assets"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/src/Assets"
            "$<TARGET_FILE_DIR:game>/src/Assets"
        # COMMAND ${CMAKE_COMMAND} -E copy
        #     "${CMAKE_SOURCE_DIR}/src/Scores.txt"
        #     "$<TARGET_FILE_DIR:game>/src/"
        COMMENT "Copying game assets for Windows/Linux"
    )

endif()



# Link to the actual SDL3 library.
target_link_libraries(game PRIVATE SDL3_mixer::SDL3_mixer SDL3_ttf::SDL3_ttf SDL3_image::SDL3_image SDL3::SDL3)
