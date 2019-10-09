cmake_minimum_required (VERSION 3.4)
project (withmyengine)

set(CMAKE_CXX_COMPILER_TARGET wasm32-unknown-unknown-wasm)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED)

# set(CMAKE_EXE_LINKER_FLAGS "-Wl,--no-entry -Wl,--export-dynamic -Wl,--no-demangle -Wl,-undefined" CACHE INTERNAL "" FORCE)

set(CMAKE_EXE_LINKER_FLAGS "-Wl,--export-table -Wl,--allow-undefined -Wl,--no-entry -Wl,--export-dynamic -Wl,--no-demangle" CACHE INTERNAL "" FORCE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-sign-compare -nostdlib -fno-builtin -Wall -pedantic -DENABLE_CXX_EXCEPTIONS=0 -fno-exceptions -fno-rtti -fdeclspec -fshort-wchar -v --target=wasm32-unknown-unknown-wasm -g --optimize=0")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")


add_executable(withmyengine
	main.cpp
	web_impl.cpp
)

target_link_libraries(withmyengine)

