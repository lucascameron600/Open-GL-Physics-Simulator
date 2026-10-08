



# OpenGL Verlet Physics Simulation

A real-time 3D physics sandbox written in C++ and OpenGL 3.3. Spheres fall under gravity, bounce off the floor and walls, and collide with each other. Motion uses Verlet integration on a fixed time step. Rendering uses a small pipeline (GLFW window, GLSL shaders, GLM matrices), with a Dear ImGui panel for live controls.
![gif](https://github.com/user-attachments/assets/f1c2c377-bd6c-4937-b32c-8994856aee0c)

## Features

- **Verlet integration** with a fixed 16 ms physics step, decoupled from the frame rate by an accumulator
- **Sphere-to-sphere collisions** resolved by position correction, checking every pair each step
- **Floor and wall collisions** inside a 100 x 50 x 100 unit box
- **OBJ loader** for the sphere mesh, a 320-triangle icosphere exported from Blender
- **GLSL 3.30 shaders** with model, view and projection matrices built with GLM
- **Keyboard camera** for moving through the scene
- **Dear ImGui panel** with sliders for camera speed, field of view and spawn count, plus live sphere count and frame rate

## Build and run

Linux with X11. The commands below are for Ubuntu or Debian.

```bash
sudo apt-get install build-essential libglfw3-dev libglm-dev libgl1-mesa-dev \
    libx11-dev libxxf86vm-dev libxcursor-dev libxinerama-dev

git clone https://github.com/lucascameron600/Open-GL-Physics-Simulator.git
cd Open-GL-Physics-Simulator/src
make
./app
```

Run `./app` from the `src` directory, because the sphere mesh is loaded by a relative path.

## Controls

| Input | Action |
| --- | --- |
| `W` / `S` | Move forward / back |
| `A` / `D` | Move left / right |
| `Space` / `Left Shift` | Move up / down |
| `B` | Spawn spheres (the "Add Balls" slider sets how many) |
| ImGui panel | Camera speed, field of view, spawn count |

## How it works

### Integration

Each sphere stores its current and previous position. One physics step computes

```
next = current + (current - previous) + acceleration * dt * dt
```

Velocity is never stored. It is implied by the difference between the two positions, so a collision can be resolved by moving a position directly.

The main loop adds each frame's duration to an accumulator and runs physics steps of exactly 16 ms until the accumulator is used up. The simulation behaves the same at any frame rate.

### Collisions

- **Sphere to sphere:** for every pair, if the distance between centers is less than the sum of the radii, each sphere is pushed back half of the overlap along the line between them.
- **Floor and walls:** a sphere that leaves the box is placed back on the boundary and its velocity along that axis is reversed.

### Rendering

- `parseobj.cpp` reads the vertex and triangle records of the OBJ file and expands them into a flat vertex array.
- One vertex buffer holds the sphere mesh and the floor.
- The vertex shader applies `projection * view * model`; the fragment shader writes a single color.
- Each sphere is drawn with its own model matrix, one draw call per sphere.

## Project layout

```
include/        project headers, plus the glad and KHR OpenGL loader headers
src/
  main.cpp      window loop, ImGui panel, input
  engine.cpp    Verlet step, collisions, fixed-time-step loop
  render.cpp    shaders, vertex buffers, draw calls
  camera.cpp    view and projection matrices, keyboard movement
  parseobj.cpp  OBJ loader
  sphere2.obj   icosphere mesh
  Makefile
libs/imgui/     Dear ImGui (third-party)
```


## Roadmap

- Instanced rendering, so all spheres are drawn in one call
- Broad-phase collision detection to replace the all-pairs check

## Background

I built this to learn OpenGL and real-time simulation without a game engine: the graphics pipeline, the model-view-projection math, and numerical integration. It started as an OpenGL exercise and became a physics project after I watched a Verlet simulation video by Pezza's Work.

## Screenshots

<img width="400" alt="Screenshot of the simulation" src="https://github.com/user-attachments/assets/c2d606ba-1810-4070-80ef-1370cdacfcd6" /> <img width="400" alt="Screenshot of the simulation" src="https://github.com/user-attachments/assets/9b21b9e6-339b-44a5-a218-6eaa820ec4e1" /> <img width="400" alt="Screenshot of the simulation" src="https://github.com/user-attachments/assets/05e27793-141e-4434-8547-cb7ef0807589" /> <img width="400" alt="Screenshot of the simulation" src="https://github.com/user-attachments/assets/dbf2a4ae-39de-4a62-a311-c80029e8977a" />
![Simulation running](https://github.com/user-attachments/assets/e7f99bc2-9cfe-49c9-95c2-74c7e9a7ca61)

## Credits

Third-party code: [Dear ImGui](https://github.com/ocornut/imgui), [GLFW](https://www.glfw.org/), [GLM](https://github.com/g-truc/glm) and [glad](https://github.com/Dav1dde/glad).

<!-- TODO: state what you adapted from each source below and what you wrote yourself. -->

Learning resources:

- [Verlet integration and 2D cloth physics, Pikuma](https://pikuma.com/blog/verlet-integration-2d-cloth-physics-simulation)
- [VerletSFML-Multithread, johnBuffer](https://github.com/johnBuffer/VerletSFML-Multithread)
- [black_hole, kavan010](https://github.com/kavan010/black_hole)
- Videos: [one](https://www.youtube.com/watch?v=lS_qeBy3aQI), [two](https://www.youtube.com/watch?v=9IULfQH7E90&t=545s), [three](https://www.youtube.com/watch?v=8-B6ryuBkCM)

## Author

Lucas Cameron
