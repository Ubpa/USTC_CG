//  88-Line 2D Moving Least Squares Material Point Method (MLS-MPM)
// [Explained Version by David Medina]

// Uncomment this line for image exporting functionality
#define TC_IMAGE_IO

// Note: You DO NOT have to install taichi or taichi_mpm.
// You only need [taichi.h] - see below for instructions.
#include "taichi.h"
using namespace taichi;
using Vec = Vector2;
using Mat = Matrix2;

// Window
const int window_size = 800;

// Grid resolution (cells)
const int quality = 1;
const int n_particle_x = 100 * quality;
const int n_particle_y = 8 * quality;
const int n_particle = n_particle_x * n_particle_y;
const int n_elements = (n_particle_x-1) * (n_particle_y - 1)*2;
const int n = 160 * quality;

const real dt = 1e-5_f;
const real frame_dt = 1e-4_f;
const real dx = 1.0_f / n;
const real inv_dx = 1.0_f / dx;

// Snow material properties
const auto particle_mass = 1.0_f;
const auto vol = 1.0_f;        // Particle Volume
const auto hardening = 10.0_f; // Snow hardening factor
const auto E = 1e5_f;          // Young's Modulus
const auto nu = 0.2_f;         // Poisson ratio
const bool plastic = true;

// Initial Lamé parameters
const real mu_0 = E / (2 * (1 + nu));
const real lambda_0 = E * nu / ((1+nu) * (1 - 2 * nu));

enum Material { Water, Jelly,Snow};
/***********************************(1)*****************************************/
struct Particle {
  // Position and velocity
  Vec x, v;
  // Deformation gradient
  Mat F;
  // Affine momentum from APIC
  Mat C;
  // Determinant of the deformation gradient (i.e. volume)
  real Jp;
  // Color
  int c;
  Material material;

  Particle(Vec x, int c, Vec v=Vec(0), Material m=Snow) :
    x(x),
    v(v),
    F(1),
    C(0),
    Jp(1),
    c(c),
    material(m) {}

  Particle():
    x(Vec(0)),
    v(Vec(0)),
    F(1),
    C(0),
    Jp(1),
    c(0),
    material(Snow){}
};
//std::vector<Particle> particles;

////////////////////////////////////////////////////////////////////////////////

// Vector3: [velocity_x, velocity_y, mass]
//Vector3 grid[n + 1][n + 1];


class mpm
{
private:
  // Vector3: [velocity_x, velocity_y, mass]
  Vector3 grid[n+1][n+1];
   
public:
  std::vector<Particle> particles;
  Vector3 vertices[n_elements];

public:
  mpm(){};
  ~mpm(){};

public:
 // Seed particles with position and color
 void add_object(Vec center, int c, Material material = Snow);

 void add_spring(Vec center, int c, Material material = Jelly);

 void add_circle(Vec center, real radius,int num, int c, Vec v, Material material);

 void substep();
private:
  int mesh(const int,const int);

  void P2G();
  void grid_op();
  void G2P();

};

int main() {
  GUI gui("Real-time 2D MLS-MPM", window_size, window_size);
  auto &canvas = gui.get_canvas();
  mpm m;
  //m.add_object(Vec(0.55, 0.45), 0x87CEFA, Water);
  //m.add_object(Vec(0.45, 0.65), 0xFFFAFA, Snow);
  //m.add_object(Vec(0.55, 0.85), 0xED553B, Jelly);
  //m.add_spring(Vec(0.3, 0.8), 0xFFFAFA, Jelly);
  real r = 0.03;
  Vec center(0.5, 0.4);
  int color[] = { 0xA71A3B,0xF2522E,0x5E66F2,0x5400B4,0x0061D3,0x0D0D0D,0xF3DCBE,0xCFD600,0xE76C00,0xC11321};
  for (int i = 0, k = 0; i < 4; i++){
	  for (int j = 0; j <= i; j++) {
		  m.add_circle(center + Vec(2*j-i, i*sqrt(3))*r, r-0.005, 1500, color[k], Vec(0), Jelly);
          k++;
   	  }
  }
  m.add_circle(Vec(0.5, 0.1), r - 0.005, 1500, 0xFFFAFA, Vec(0,200), Jelly);
  //m.add_circle(Vec(0.46, 0.65), 0.03, 1000, 0xF2522E, Vec(0, 0), Jelly);
  //m.add_circle(Vec(0.54, 0.65), 0.03, 1000, 0x5E66F2, Vec(0, 0), Jelly);
  //m.add_circle(Vec(0.5, 0.6), 0.03, 1000, 0x0D0D0D, Vec(0, 0), Jelly);
  int frame = 0;
  // Main Loop
  for (int step = 0;; step++) {
    // Advance simulation
    m.substep();

    // Visualize frame
    if (step % int(frame_dt / dt) == 0) {
      // Clear background
      canvas.clear(0x547C8C);
      // Box
      canvas.rect(Vec(0.04), Vec(0.96)).radius(2).color(0x4FB99F).close();

      //center collision circle
      //canvas.circle(Vec(0.5, 0.5)).radius(40).color(0x068587);
      // Particles
      for (auto& p : m.particles) {
        canvas.circle(p.x).radius(1).color(p.c);
      }

      // springs
      /*for(size_t i = 0; i < n_elements-1; i++){
          size_t j = (i+1)%n_elements;
        for (size_t k = 0; k < 3; k++) {
            size_t a = m.vertices[i][k];
            size_t b = m.vertices[j][k];
            canvas.line(m.particles[a].x, m.particles[b].x, Vector4(0x87CEFA));
        }
      }*/

      // Update image  
      gui.update();

      // Write to disk (optional)
      canvas.img.write_as_image(fmt::format("pool_ball/{:05d}.png", frame++));
    }
  }
}

// Seed particles with position and color
void mpm::add_object(Vec center, int c, Material material) {
  // Randomly sample 1000 particles in the square
  for (int i = 0; i < 1000; i++) {
    particles.push_back(Particle((Vec::rand() * 2.0f - Vec(1)) * 0.08f + center, c, Vec(0), material));
  }
}

void mpm::add_spring(Vec center, int c, Material material) {
  //particles.reserve(n_particle_x * n_particle_y);
  particles.resize(n_particle_x * n_particle_y);
  int t;
  for (int i = 0; i < n_particle_x; i++){
    for (int j = 0; j < n_particle_y; j++){
      t = mesh(i, j);
      particles[t] = Particle(Vec(i * dx, j * dx) *0.32f + center, c, Vec(0), material);
    } 
  }

}

void mpm::add_circle(Vec center, real radius, int num, int c, Vec v, Material material) {
	for (int i = 0; i < num;) {
        auto pos = (Vec::rand() * 2.0_f - Vec(1)) * radius;
        if (pos.x * pos.x + pos.y * pos.y < radius * radius) {
            particles.push_back(Particle(pos + center, c, v, material));
            i++;
        }
	}
}

void mpm::substep() {
  // Reset grid
  std::memset(grid, 0, sizeof(grid));
  P2G();
  grid_op();
  G2P();
}

int mpm::mesh(const int i,const int j) {
  return i * n_particle_y + j;
}

void mpm::P2G() {
  // P2G
  for (auto& p : particles) {
    Vector2i base_coord = (p.x * inv_dx - Vec(0.5f)).cast<int>();
    Vec fx = p.x * inv_dx - base_coord.cast<real>();
    Vec w[3] = {
        Vec(0.5) * sqr(Vec(1.5) - fx),
        Vec(0.75) - sqr(fx - Vec(1.0)),
        Vec(0.5) * sqr(fx - Vec(0.5))};
    auto e = std::exp(hardening * (1.0_f - p.Jp));
    if (p.material == Jelly)  // 果冻
      e = 0.3;
    auto mu = mu_0 * e, lambda = lambda_0 * e;
    if (p.material == Water)  // 流体
      mu = 0;
    real J = determinant(p.F);
    Mat r, s;
    polar_decomp(p.F, r, s);
    real Dinv = 4 * inv_dx * inv_dx;
    auto PF = (2 * mu * (p.F - r) * transposed(p.F) + lambda * (J - 1) * J);
    auto stress = -(dt * vol) * (Dinv * PF);
    auto affine = stress + particle_mass * p.C;

    // P2G
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        auto dpos = (Vec(i, j) - fx) * dx;
        // Translational momentum
        Vector3 mass_x_velocity(p.v * particle_mass, particle_mass);
        grid[base_coord.x + i][base_coord.y + j] += (w[i].x * w[j].y * (mass_x_velocity + Vector3(affine * dpos, 0)));
      }
    }
  }
}

void mpm::grid_op() {
  for (int i = 0; i <= n; i++) {
    for (int j = 0; j <= n; j++) {
      auto& g = grid[i][j];
      // No need for epsilon here
      if (g[2] > 0) {
        // Normalize by mass
        g /= g[2];
        // Gravity
        //g += dt * Vector3(0, -200, 0);//gravity

        //center collision circle
        //Vec dist = Vec(i, j)*dx - Vec(0.5, 0.5);
        //real dist2 = sqrt(dist.x*dist.x+dist.y*dist.y);
        //if (dist2 < 0.05) {
        //  dist = dist/dist2;
        //  real n_normal = g[0]*dist.x + g[1]*dist.y;//dot product
        //  n_normal = n_normal < 0 ? n_normal : 0;
        //  g -= Vector3(n_normal*dist.x, n_normal*dist.y, 0); 
        //}


        // boundary thickness
        real boundary = 0.05;
        // Node coordinates
        real x = (real)i / n;
        real y = (real)j / n;

        // Sticky boundary
        
        if(x < boundary){
            g[0] = std::max(0.0f, g[0]);
        }
        if(x > 1 - boundary){
            g[0] = std::min(0.0f, g[0]);
        }
        // Separate boundary
        if (y < boundary) {
          g[1] = std::max(0.0f, g[1]);
        }
        if (y > 1 - boundary) {
          g[1] = std::min(0.0f, g[1]);
        }
      }
    }
  }
}

void mpm::G2P() {
  for (auto& p : particles) {
    // element-wise floor
    Vector2i base_coord = (p.x * inv_dx - Vec(0.5f)).cast<int>();
    Vec fx = p.x * inv_dx - base_coord.cast<real>();
    Vec w[3] = {
        Vec(0.5) * sqr(Vec(1.5) - fx),
        Vec(0.75) - sqr(fx - Vec(1.0)),
        Vec(0.5) * sqr(fx - Vec(0.5))};

    p.C = Mat(0);
    p.v = Vec(0);

    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        auto dpos = (Vec(i, j) - fx);
        auto grid_v = Vec(grid[base_coord.x + i][base_coord.y + j]);
        auto weight = w[i].x * w[j].y;
        // Velocity
        p.v += weight * grid_v;
        // APIC C
        p.C += 4 * inv_dx * Mat::outer_product(weight * grid_v, dpos);
      }
    }

    // Advection
    p.x += dt * p.v;

    // MLS-MPM F-update
    auto F = (Mat(1) + dt * p.C) * p.F;

    /***********************************(3)*****************************************/
    if (p.material == Water) {  // Water
      p.F = Mat(1) * sqrt(determinant(F));
    } else if (p.material == Jelly) {  // Jelly
      p.F = F;
    } else if (p.material == Snow) {  // Snow
      Mat svd_u, sig, svd_v;
      svd(F, svd_u, sig, svd_v);
      for (int i = 0; i < 2 * int(plastic); i++)  // Snow Plasticity
        sig[i][i] = clamp(sig[i][i], 1.0_f - 2.5e-2_f, 1.0_f + 7.5e-3_f);
      real oldJ = determinant(F);
      F = svd_u * sig * transposed(svd_v);
      real Jp_new = clamp(p.Jp * oldJ / determinant(F), 0.6_f, 20.0_f);
      p.Jp = Jp_new;
      p.F = F;
    }
    ////////////////////////////////////////////////////////////////////////////////
  }
}

/* -----------------------------------------------------------------------------
** Reference: A Moving Least Squares Material Point Method with Displacement
              Discontinuity and Two-Way Rigid Body Coupling (SIGGRAPH 2018)

  By Yuanming Hu (who also wrote this 88-line version), Yu Fang, Ziheng Ge,
           Ziyin Qu, Yixin Zhu, Andre Pradhana, Chenfanfu Jiang


** Build Instructions:

Step 1: Download and unzip mls-mpm88.zip (Link: http://bit.ly/mls-mpm88)
        Now you should have "mls-mpm88.cpp" and "taichi.h".

Step 2: Compile and run

* Linux:
    g++ mls-mpm88-explained.cpp -std=c++14 -g -lX11 -lpthread -O3 -o mls-mpm
    ./mls-mpm


* Windows (MinGW):
    g++ mls-mpm88-explained.cpp -std=c++14 -lgdi32 -lpthread -O3 -o mls-mpm
    .\mls-mpm.exe


* Windows (Visual Studio 2017+):
  - Create an "Empty Project"
  - Use taichi.h as the only header, and mls-mpm88-explained.cpp as the only source
  - Change configuration to "Release" and "x64"
  - Press F5 to compile and run


* OS X:
    g++ mls-mpm88-explained.cpp -std=c++14 -framework Cocoa -lpthread -O3 -o mls-mpm
    ./mls-mpm


** FAQ:
Q1: What does "1e-4_f" mean?
A1: The same as 1e-4f, a float precision real number.

Q2: What is "real"?
A2: real = float in this file.

Q3: What are the hex numbers like 0xED553B?
A3: They are RGB color values.
    The color scheme is borrowed from
    https://color.adobe.com/Copy-of-Copy-of-Core-color-theme-11449181/

Q4: How can I get higher-quality?
A4: Change n to 320; Change dt to 1e-5; Change E to 2e4;
    Change particle per cube from 500 to 8000 (Ln 72).
    After the change the whole animation takes ~3 minutes on my computer.

Q5: How to record the animation?
A5: Uncomment Ln 2 and 85 and create a folder named "tmp".
    The frames will be saved to "tmp/XXXXX.png".

    To get a video, you can use ffmpeg. If you already have taichi installed,
    you can simply go to the "tmp" folder and execute

      ti video 60

    where 60 stands for 60 FPS. A file named "video.mp4" is what you want.

Q6: How is taichi.h generated?
A6: Please check out my #include <taichi> talk:
    http://taichi.graphics/wp-content/uploads/2018/11/include_taichi.pdf
    and the generation script:
    https://github.com/yuanming-hu/taichi/blob/master/misc/amalgamate.py
    You can regenerate it using `ti amal`, if you have taichi installed.

Questions go to yuanming _at_ mit.edu
                            or https://github.com/yuanming-hu/taichi_mpm/issues.

                                                      Last Update: March 6, 2019
                                                      Version 1.5

----------------------------------------------------------------------------- */

