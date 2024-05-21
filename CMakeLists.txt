cmake_minimum_required(VERSION 2.8)
project(main)

# Include PkgConfig module
find_package(PkgConfig REQUIRED)
find_package(CURL REQUIRED)

# Find libnotify using pkg-config
pkg_check_modules(LIBNOTIFY REQUIRED libnotify)

# Specify the executable
add_executable(main test.cpp)

# Include the directories for libnotify
include_directories(${LIBNOTIFY_INCLUDE_DIRS})

# Link libnotify to your target
target_link_libraries(main ${LIBNOTIFY_LIBRARIES})
target_link_libraries(main CURL::libcurl)
