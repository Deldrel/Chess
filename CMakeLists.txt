cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 14)

set(PROJECT_NAME OrdinaryLog)
project(${PROJECT_NAME})

set(LIBRARIES_FILES
        imgui/imconfig.h
        imgui/imconfig-SFML.h
        imgui/imgui.cpp
        imgui/imgui.h
        imgui/imgui-SFML.cpp
        imgui/imgui-SFML.h
        imgui/imgui-SFML_export.h
        imgui/imgui_demo.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_internal.h
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
        imgui/imstb_rectpack.h
        imgui/imstb_textedit.h
        imgui/imstb_truetype.h
        )

add_executable(OrdinaryLog main.cpp Chess.cpp Chess.h cplus.h "${LIBRARIES_FILES}")

find_package(SFML COMPONENTS system window graphics audio network REQUIRED)
target_link_libraries(${PROJECT_NAME} sfml-system sfml-window sfml-graphics sfml-audio sfml-network)

find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} glu32 opengl32)