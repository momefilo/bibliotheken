file(GLOB FILES *.c *.h)
add_library(buzzer_sound ${FILES})
target_link_libraries(buzzer_sound
	hardware_pwm
	hardware_gpio
	hardware_clocks
	pico_stdlib
)

target_include_directories(buzzer_sound PUBLIC ./)
