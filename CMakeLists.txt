cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(DX11AsyncRender)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INCLUDE_DIR include)
set(SRC_DIR src)
set(SHADERS_DIR shaders)

set(SOURCES DX11AsyncRender.cpp
	${SRC_DIR}/graphics_device.cpp
	${SRC_DIR}/platform_win32.cpp
	${SRC_DIR}/renderer.cpp
	${SRC_DIR}/win32_window.cpp
)

add_executable(DX11AsyncRender ${SOURCES})

target_include_directories(DX11AsyncRender PRIVATE ${INCLUDE_DIR})

target_link_libraries(DX11AsyncRender D3D11 D3DCOMPILER DXGI)

file(GLOB_RECURSE VERTEX_SHADERS 
    CONFIGURE_DEPENDS 
    "${SHADERS_DIR}/*vertex*.hlsl"
)

file(GLOB_RECURSE PIXEL_SHADERS 
    CONFIGURE_DEPENDS 
    "${SHADERS_DIR}/*pixel*.hlsl"
)

message(STATUS "Vertex shader dependencies: ${VERTEX_SHADERS}")
message(STATUS "Pixel shader dependencies: ${PIXEL_SHADERS}")

function(compile_shaders shader_files shader_type shader_model)
	foreach(SHADER_FILE ${shader_files})
		get_filename_component(SHADER_FILENAME ${SHADER_FILE} NAME_WE)
        get_filename_component(SHADER_DIRECTORY ${SHADER_FILE} DIRECTORY)
        list(APPEND CSO_SHADER_FILES ${CMAKE_BINARY_DIR}/${SHADER_FILENAME}.cso)
        add_custom_command(TARGET ${PROJECT_NAME} PRE_LINK
                            COMMAND fxc.exe /nologo /EMain /T${shader_type}_${shader_model} $<IF:$<CONFIG:DEBUG>,/Od,/O1> /Zi /Fo ${SHADER_DIRECTORY}/${SHADER_FILENAME}.cso /Fd ${SHADER_DIRECTORY}/${SHADER_FILENAME}.pdb ${SHADER_FILE}
                            COMMENT "HLSL ${SHADER_FILE}"
                            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                            VERBATIM)
	endforeach(SHADER_FILE)
endfunction()

compile_shaders(${VERTEX_SHADERS} vs 5_1)
compile_shaders(${PIXEL_SHADERS} ps 5_1)

add_custom_target(Shaders DEPENDS ${SHADERS})

add_custom_command(TARGET Shaders POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/${SHADERS_DIR}" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/${SHADERS_DIR}"
    VERBATIM)

add_dependencies(${PROJECT_NAME} Shaders)
