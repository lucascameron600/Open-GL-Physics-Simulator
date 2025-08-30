CXX = g++
CXXFLAGS = -g -pg -Wall -I../include -I../libs/imgui -I../libs/imgui/backends
LDFLAGS = -pg -lglfw -ldl -lGL -lX11 -lXxf86vm -lXcursor -lXinerama

IMGUI_SRC = \
    ../libs/imgui/imgui.cpp \
    ../libs/imgui/imgui_draw.cpp \
    ../libs/imgui/imgui_tables.cpp \
    ../libs/imgui/imgui_widgets.cpp \
    ../libs/imgui/backends/imgui_impl_glfw.cpp \
    ../libs/imgui/backends/imgui_impl_opengl3.cpp

SRC = main.cpp camera.cpp sphere.cpp render.cpp parseobj.cpp engine.cpp gl.c $(IMGUI_SRC)

TARGET = app

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)
