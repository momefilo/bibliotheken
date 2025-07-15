file(GLOB FILES *.c *.h)
add_library(si5351 ${FILES})
target_link_libraries(si5351
	 pico_stdlib
	 hardware_i2c
)

target_include_directories(si5351 PUBLIC ./)
