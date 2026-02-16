cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(DX11AsyncRender)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INCLUDE_DIR include)
set(SRC_DIR src)
set(SHADERS_DIR shaders)

set(SOURCES DX11AsyncRender.cpp
    ${SRC_DIR}/d3d11_render_batch.cpp
    ${SRC_DIR}/d3d11_shader.cpp
    ${SRC_DIR}/d3d11_shader_store.cpp
    ${SRC_DIR}/d3d11_transform.cpp
	${SRC_DIR}/graphics_device.cpp
	${SRC_DIR}/platform_win32.cpp
    ${SRC_DIR}/render_frame.cpp
    ${SRC_DIR}/render_frame_buffer.cpp
	${SRC_DIR}/renderer.cpp
	${SRC_DIR}/win32_window.cpp
    "${CMAKE_CURRENT_SOURCE_DIR}/app.manifest"
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
	foreach(SHADER_FILE IN LISTS ${shader_files})
		get_filename_component(SHADER_FILENAME ${SHADER_FILE} NAME_WE)
        get_filename_component(SHADER_DIRECTORY ${SHADER_FILE} DIRECTORY)
        set(COMPILED_SHADER_FILE "${SHADER_DIRECTORY}/${SHADER_FILENAME}.cso")
        set(COMPILED_PDB_FILE "${SHADER_DIRECTORY}/${SHADER_FILENAME}.pdb")
        add_custom_command(
            OUTPUT "${COMPILED_SHADER_FILE}" "${COMPILED_PDB_FILE}"
            COMMAND fxc.exe /nologo /EMain /T${shader_type}_${shader_model} $<IF:$<CONFIG:DEBUG>,/Od,/O1> /Zi /Fo ${COMPILED_SHADER_FILE} /Fd ${COMPILED_PDB_FILE} ${SHADER_FILE}
            COMMENT "Compiling HLSL Shader: ${SHADER_FILE}"
            DEPENDS "${SHADER_FILE}"
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            VERBATIM
        )
        list(APPEND COMPILED_SHADER_FILES ${COMPILED_SHADER_FILE})
	endforeach(SHADER_FILE)

    add_custom_target(${shader_type}_Shaders DEPENDS ${COMPILED_SHADER_FILES})
    add_dependencies(${PROJECT_NAME} ${shader_type}_Shaders)
endfunction()

target_sources(${PROJECT_NAME} PRIVATE ${VERTEX_SHADERS} ${PIXEL_SHADERS})

compile_shaders(VERTEX_SHADERS vs 5_0)
compile_shaders(PIXEL_SHADERS ps 5_0)

add_custom_target(Shaders)

add_custom_command(TARGET Shaders POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/${SHADERS_DIR}" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/${SHADERS_DIR}"
    VERBATIM)

add_dependencies(${PROJECT_NAME} Shaders)
