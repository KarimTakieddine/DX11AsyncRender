cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(DX11AsyncRender)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INCLUDE_DIR include)
set(SRC_DIR src)

set(SOURCES DX11AsyncRender.cpp
	${SRC_DIR}/graphics_device.cpp
	${SRC_DIR}/platform_win32.cpp
	${SRC_DIR}/renderer.cpp
	${SRC_DIR}/win32_window.cpp
)

add_executable(DX11AsyncRender ${SOURCES})

target_include_directories(DX11AsyncRender PRIVATE ${INCLUDE_DIR})

target_link_libraries(DX11AsyncRender D3D11 D3DCOMPILER DXGI)
