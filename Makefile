CPP_COMPILER = g++
C_COMPILER = gcc

TEMP_DIR = temp
BUILD_DIR = build
SRC_DIR = src
INCLUDE_DIR = -Iinclude
LIB_DIR = -Llib
LINKER_FLAGS = -lglfw3 -lopengl32 -lgdi32 -limm32

FLAGS = -Wall -Werror -Wformat $(INCLUDE_DIR) $(LIB_DIR) $(LINKER_FLAGS)

ifeq ($(OS),Windows_NT)
	RM = del /s
else
	RM = rm -rf
endif

$(TEMP_DIR)/glad.o: src/extern/glad.c
	$(C_COMPILER) $(FLAGS) $^ -c -o $@

$(TEMP_DIR)/imgui_impl_glfw.o: include/imgui/backends/imgui_impl_glfw.cpp
	$(CPP_COMPILER) $(FLAGS) $^ -c -o $@

$(TEMP_DIR)/imgui_impl_opengl3.o: include/imgui/backends/imgui_impl_opengl3.cpp
	$(CPP_COMPILER) $(FLAGS) $^ -c -o $@

$(TEMP_DIR)/imgui.o: include/imgui/imgui.cpp
	$(CPP_COMPILER) $(FLAGS) $^ -c -o $@

$(TEMP_DIR)/imgui_demo.o: include/imgui/imgui_demo.cpp
	$(CPP_COMPILER) $(FLAGS) $^ -c -o $@

$(TEMP_DIR)/imgui_draw.o: include/imgui/imgui_draw.cpp
	$(CPP_COMPILER) $(FLAGS) $^ -c -o $@

$(TEMP_DIR)/imgui_widgets.o: include/imgui/imgui_widgets.cpp
	$(CPP_COMPILER) $(FLAGS) $^ -c -o $@

$(TEMP_DIR)/imgui_tables.o: include/imgui/imgui_tables.cpp
	$(CPP_COMPILER) $(FLAGS) $^ -c -o $@

$(TEMP_DIR)/Application.o: src/MartenCore/src/Application.cpp
	$(CPP_COMPILER) $(FLAGS) $^ -c -o $@

$(TEMP_DIR)/Window.o: src/MartenCore/src/Window.cpp
	$(CPP_COMPILER) $(FLAGS) $^ -c -o $@

$(TEMP_DIR)/MartenLogger.o: src/MartenUtils/src/MartenLogger.cpp
	$(CPP_COMPILER) $(FLAGS) $^ -c -o $@

$(TEMP_DIR)/main.o: src/main.cpp
	$(CPP_COMPILER) $(FLAGS) $^ -c -o $@

project: $(TEMP_DIR)/glad.o $(TEMP_DIR)/imgui_impl_glfw.o $(TEMP_DIR)/imgui_impl_opengl3.o $(TEMP_DIR)/imgui.o $(TEMP_DIR)/imgui_demo.o $(TEMP_DIR)/imgui_draw.o $(TEMP_DIR)/imgui_widgets.o $(TEMP_DIR)/imgui_tables.o $(TEMP_DIR)/Application.o $(TEMP_DIR)/Window.o $(TEMP_DIR)/MartenLogger.o $(TEMP_DIR)/main.o
	$(CPP_COMPILER) $(FLAGS) $^ -o $(BUILD_DIR)/MartenEngine

all: project

clean:
	$(RM) *.o