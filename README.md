# 2d-particles
Simple particle simulator using a compute shader. Written in C++ with OpenGL

# Installation
In this project I have used three static libraries: glew, glfw and glm (OpenGL mathematics). If you are on linux, make sure these are installed in /usr/lib directory (or you have to change the makefile). On windows, if you use Visual Studio and are new to setting up libraries, I would recommend following this: https://www.youtube.com/watch?v=OR4fNpBjmq8 

# What does it do?
When running the application, you should get a square window opening, with red particles following your mouse. To toggle gravity, press the spacebar. If you have a powerful GPU, try building with a higher number of particles. My Lenovo Yoga can handle thousands, way more than if the program used the CPU for computing particle movement!
