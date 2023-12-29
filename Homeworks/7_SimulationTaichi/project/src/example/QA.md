





Reference: A Moving Least Squares Material Point Method with Displacement
              Discontinuity and Two-Way Rigid Body Coupling (SIGGRAPH 2018)

  By Yuanming Hu (who also wrote this 88-line version), Yu Fang, Ziheng Ge,
           Ziyin Qu, Yixin Zhu, Andre Pradhana, Chenfanfu Jiang

Build Instructions:

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

```sh
ti video 60
```

where 60 stands for 60 FPS. A file named "video.mp4" is what you want.

Q6: How is taichi.h generated?
A6: Please check out my #include <taichi> talk:
    http://taichi.graphics/wp-content/uploads/2018/11/include_taichi.pdf
    and the generation script:
    https://github.com/yuanming-hu/taichi/blob/master/misc/amalgamate.py
    You can regenerate it using `ti amal`, if you have taichi installed.

Questions go to yuanming _at_ mit.edu
                            or https://github.com/yuanming-hu/taichi_mpm/issues.

