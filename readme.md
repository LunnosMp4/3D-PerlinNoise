# 3D Perlin Noise

This program generates a Perlin noise terrain using the Raylib library. It allows you to interactively adjust various parameters to customize the generated terrain.

![Demo Gif](https://github.com/LunnosMp4/3D-PerlinNoise/blob/master/res/perlin_noise_video.gif)

# Installation
To run the program, you need to have the Raylib library installed. Follow the instructions below to install Raylib and compile the program:

- Download the Raylib library from the official repository: https://github.com/raysan5/raylib
- Extract the downloaded Raylib archive to a directory of your choice.
- Navigate to the extracted Raylib directory and follow the installation instructions provided in the README file.
- Once Raylib is installed, clone this repository to your local machine and build the program using the following commands:

```
git clone https://github.com/LunnosMp4/3D-PerlinNoise.git
cd 3D-PerlinNoise
make
```

# Usage
After building the program, you can run it by executing the generated executable file.

```
./perlin_noise
```
The program will open a window displaying the generated Perlin noise terrain. You can use the following controls:

- Move the mouse to rotate the camera.
- Hold the **left Alt key** and move the mouse to enable cursor mode for adjusting parameters. Use the sliders to modify the following parameters:
    - **Scale**: Controls the scale of the Perlin noise.
    - **Octaves**: Specifies the number of octaves used in the Perlin noise generation.
    - **Decay**: Adjusts the persistence of the Perlin noise.
    - **Speed**: Controls the animation speed of the terrain.
    - **Size**: Specifies the size of the terrain.
    - Use the **color picker** to change the color of the terrain.
- Press the close button or the Escape key to exit the program.


**Note:** This program uses the Raylib library, which provides a simple and easy-to-use interface for 2D and 3D graphics programming. For more information about Raylib and its features, refer to the official documentation: https://www.raylib.com/

# Code Explanation
The program generates a Perlin noise texture, convert it to a heightmap and renders it as a 3D terrain using Raylib. Here are the main functions and their purpose:

- `lerp`: Performs linear interpolation between two values.
- `grad`: Computes the gradient for a given hash value and coordinates.
- `perlin`: Computes Perlin noise for the specified coordinates.
- `generatePerlinTexture`: Generates a Perlin noise texture using the specified parameters.
- `RaylibPerlinNoise`: Main function that initializes Raylib, sets up the camera and window, and enters the main rendering loop.


The program uses various Raylib functions to create and manipulate the window, camera, textures, and models. It also utilizes **[Raygui](https://github.com/raysan5/raygui)**, a simple immediate-mode GUI library for Raylib, to create sliders and color picker controls.

Feel free to explore and modify the code to create different types of procedural terrains or experiment with other features provided by Raylib.
