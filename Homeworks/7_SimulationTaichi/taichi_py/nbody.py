# Welcome to Cursor


# 1. Try generating with command K on a new line. Ask for a pytorch script of a feedforward neural network
# 2. Then, select the outputted code and hit chat. Ask if there's a bug. Ask how to improve.
# 3. Try selecting some code and hitting edit. Ask the bot to add residual layers.
# 4. To try out cursor on your own projects, go to the file menu (top left) and open a folder.

import taichi as ti
import numpy as np
ti.init(arch=ti.gpu)

# Constants
dt = 1e-4
dx = 1 / 64
inv_dx = 1 / dx
half_inv_dx = 0.5 * inv_dx
particle_mass = 1
particle_radius = dx * 0.5
particle_volume = (4 / 3) * np.pi * particle_radius ** 3
rho0 = 1000
pressure_constant = 1e4
viscosity_constant = 0.1
gravity = ti.Vector([0, -9.8])
boundary_width = 3
max_num_particles = 5000

# Variables
x = ti.Vector(2, dt=ti.f32, shape=max_num_particles)
v = ti.Vector(2, dt=ti.f32, shape=max_num_particles)
C = ti.Matrix(2, 2, dt=ti.f32, shape=max_num_particles)
J = ti.var(dt=ti.f32, shape=max_num_particles)
grid_v = ti.Vector(2, dt=ti.f32, shape=(64, 64))
grid_m = ti.var(dt=ti.f32, shape=(64, 64))
num_particles = ti.var(dt=ti.i32, shape=())
num_cells = ti.Vector(2, dt=ti.i32, shape=())

# GUI
gui = ti.GUI("SPH", (512, 512), background_color=0x112F41)
particle_color = 0x068587

# Functions


@ti.func
def poly6_value(s, h):
    if 0 < s < h:
        return 315 / (64 * np.pi * h ** 9) * (h ** 2 - s ** 2) ** 3
    else:
        return 0


@ti.func
def spiky_gradient(r, h):
    if 0 < r < h:
        return -45 / (np.pi * h ** 6) * (h - r) ** 2 / r
    else:
        return 0


@ti.func
def compute_scorr(C):
    trace = C[0, 0] + C[1, 1]
    # Continue from here
    S = ti.Matrix([[C[0, 0], C[0, 1]], [C[1, 0], C[1, 1]]])
    # Debugging: Print the trace and S matrix values
    print("Trace:", trace)
    print("S matrix:", S)
    gui.circles(x.to_numpy()[:num_particles], color=particle_color, radius=1.5)
    gui.show()
