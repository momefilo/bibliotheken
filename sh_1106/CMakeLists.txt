file(GLOB FILES *.c *.h)
add_library(sh_1106 ${FILES})
target_link_libraries(sh_1106
	hardware_gpio
	hardware_i2c
	pico_stdlib
)

target_include_directories(sh_1106 PUBLIC ./)
