cmake_minimum_required(VERSION 3.16)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

project(basic_stepper)

# Add the stspin220 component
set(EXTRA_COMPONENT_DIRS "../")