file(GLOB FILES *.c *.h)
add_library(drehgeber ${FILES})
target_link_libraries(drehgeber
	 pico_stdlib
)

target_include_directories(drehgeber PUBLIC ./)
