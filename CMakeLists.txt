cmake_minimum_required(VERSION 3.17)
project(php_runpy)

set(CMAKE_CXX_STANDARD 20)

find_package(PythonLibs 3.8 REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

execute_process(
        COMMAND bash "-c" "php-config --configure-options | cut -d' ' -f 8 | cut -d'=' -f 2"
        OUTPUT_VARIABLE PHP_INI_DIR)
message("PHP ini directory: ${PHP_INI_DIR}")

execute_process(
        COMMAND bash "-c" "php-config --extension-dir"
        OUTPUT_VARIABLE PHP_EXTENSION_DIR)
message("PHP extension directory: ${PHP_EXTENSION_DIR}")
execute_process (
        COMMAND php-config --include-dir
        OUTPUT_VARIABLE PHP_SOURCE
)
message("Using source directory: ${PHP_SOURCE}")


add_library(php_runpy SHARED library.cpp)

link_directories(/usr/local/lib)


target_include_directories(php_runpy PRIVATE ${PYTHON_INCLUDE_DIRS})
target_link_libraries(php_runpy  ${PYTHON_LIBRARIES} )

find_library(PHP_CPP_LIB NAMES libphpcpp.so)
target_link_libraries(php_runpy ${PHP_CPP_LIB})


if (APPLE)
    link_directories(/usr/local/lib)
    include_directories(/usr/local/include)
#    target_include_directories(php_runpy PUBLIC /usr/local/include)
    set_property(TARGET php_runpy PROPERTY PREFIX "")
    set_property(TARGET php_runpy PROPERTY OUTPUT_NAME "runpy.so")
    set_property(TARGET php_runpy PROPERTY SUFFIX "")
    #    set_property(TARGET OpenImageIO PROPERTY SOVERSION "32.1.2.0")
endif()

#add_custom_command(
#        TARGET php_runpy
#        POST_BUILD
#        COMMAND ls -lah && env)