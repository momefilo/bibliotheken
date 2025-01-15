cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)
project(bibliotheken_project C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()
add_executable(bibliotheken bibliotheken.c )

# Bibliotheksverzeichnisse
add_subdirectory(./buzzer_sound)
add_subdirectory(./momefilo_flash)

target_link_libraries(bibliotheken
	buzzer_sound
	momefilo_flash
	pico_stdlib
	pico_rand)
pico_add_extra_outputs(bibliotheken)
