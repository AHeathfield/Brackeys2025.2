# MinGW-w64 cross-compilation toolchain for Windows
set(CMAKE_SYSTEM_NAME Windows)

# Specify the cross-compiler
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)

# Target environment root (adjust if needed)
set(CMAKE_FIND_ROOT_PATH /usr/bin/x86_64-w64-mingw32)

# Search for programs in the host environment (Linux)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# Search for libraries and headers in the target environment (Windows)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Enable big object files (fixes HarfBuzz compilation)
set(CMAKE_CXX_FLAGS "-Wa,-mbig-obj")
