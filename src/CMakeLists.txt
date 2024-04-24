# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)
include(CMakePrintHelpers)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.5.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.5.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

project(GP2040-RE LANGUAGES C CXX ASM)
add_compile_options(-Wall
        -Wtype-limits
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        )

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()
add_compile_options(-fdata-sections -ffunction-sections)
add_link_options(-Wl,--gc-sections)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  # Activate some compiler / linker options to aid us with diagnosing stack space issues in Debug builds
  add_compile_options(-fstack-usage -Wstack-usage=500)
  add_compile_definitions(PICO_USE_STACK_GUARDS=1)
endif()

# We want a larger stack of 4kb per core instead of the default 2kb
add_compile_definitions(PICO_STACK_SIZE=0x1000)
add_executable(${PROJECT_NAME}
main.cpp
controllers/NESController.cpp
controllers/SNESController.cpp
controllers/N64.cpp
)
add_subdirectory(../lib/no-OS-FatFS-SD-SPI-RPi-Pico/FatFs_SPI build)
pico_set_program_name(${PROJECT_NAME} "GP2040-RE")
pico_set_program_version(${PROJECT_NAME} "0.1")

pico_enable_stdio_usb(${PROJECT_NAME} 0)
pico_enable_stdio_uart(${PROJECT_NAME} 1)

# Add the standard library to the build
target_link_libraries(${PROJECT_NAME}
	pico_i2c_slave
	hardware_i2c
	pico_multicore
	pico_stdlib
  FatFs_SPI
)
target_compile_definitions(${PROJECT_NAME} PUBLIC
  PICO_XOSC_STARTUP_DELAY_MULTIPLIER=64
)
# Add the standard include files to the build
target_include_directories(${PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
)

pico_add_extra_outputs(${PROJECT_NAME})

