/*
  Author: Lunnos
  Github: github.com/LunnosMp4
  File: Raylib.cpp
*/

#include <raylib.h>
#include <cmath>
#include <iostream>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

constexpr int windowWidth = 800;
constexpr int windowHeight = 600;
constexpr int definitionWidth = windowWidth / 8;
constexpr int definitionHeight = windowHeight / 8;
volatile float animationSpeed = 0.002f;
volatile float scale = 25.0f;
volatile int octaves = 2;
volatile float persistence = 0.2f;

static Color* pixels = new Color[definitionWidth * definitionHeight];

float lerp(float a, float b, float t) {
    return a + t * (b - a);
}

float grad(int hash, float x, float y) {
    int h = hash & 15;
    float u = h < 8 ? x : y;
    float v = h < 4 ? y : (h == 12 || h == 14 ? x : 0);
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

float perlin(float x, float y) {
    static const int permutation[] = {
        151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225,
        140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23, 190, 6, 148,
        247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32,
        57, 177, 33, 88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175,
        74, 165, 71, 134, 139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122,
        60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244, 102, 143, 54,
        65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200,
        196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52,
        217, 226, 250, 124, 123, 5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207,
        206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42, 223, 183, 170, 213, 119,
        248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9, 129,
        22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218,
        246, 97, 228, 251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81,
        51, 145, 235, 249, 14, 239, 107, 49, 192, 214, 31, 181, 199, 106, 157, 184,
        84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254, 138, 236, 205, 93, 222,
        114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180
    };

    int xi = static_cast<int>(x) & 255;
    int yi = static_cast<int>(y) & 255;

    float xf = x - static_cast<int>(x);
    float yf = y - static_cast<int>(y);

    float u = xf * xf * xf * (xf * (xf * 6 - 15) + 10);
    float v = yf * yf * yf * (yf * (yf * 6 - 15) + 10);

    int aa = permutation[permutation[xi] + yi];
    int ab = permutation[permutation[xi] + yi + 1];
    int ba = permutation[permutation[xi + 1] + yi];
    int bb = permutation[permutation[xi + 1] + yi + 1];

    float x1, x2, y1;

    x1 = lerp(grad(aa, xf, yf), grad(ba, xf - 1, yf), u);
    x2 = lerp(grad(ab, xf, yf - 1), grad(bb, xf - 1, yf - 1), u);
    y1 = lerp(x1, x2, v);

    return y1;
}

void generatePerlinTexture(Image& image, float offsetX, float offsetY, float scaleX, float scaleY, int octaves, float persistence) {
    float minNoise = 0.0f;
    float maxNoise = 1.0f;

    for (int y = 0; y < definitionHeight; ++y) {
        for (int x = 0; x < definitionWidth; ++x) {
            float amplitude = 1.0f;
            float frequency = 1.0f;
            float noise = 0.0f;

            for (int o = 0; o < octaves; ++o) {
                float nx = static_cast<float>(x) / scaleX * frequency + offsetX;
                float ny = static_cast<float>(y) / scaleY * frequency + offsetY;

                noise += perlin(nx, ny) * amplitude * 0.5f;

                amplitude *= persistence;
                frequency *= 1.5f;
            }

            if (noise < minNoise) minNoise = noise;
            if (noise > maxNoise) maxNoise = noise;

            int noiseValue = static_cast<int>((noise + 1.0f) / 2.0f * 255);
            pixels[y * definitionWidth + x] = {
                static_cast<unsigned char>(noiseValue),
                static_cast<unsigned char>(noiseValue),
                static_cast<unsigned char>(noiseValue),
                255
            };
        }
    }
    image.data = pixels;
}

void RaylibPerlinNoise() {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(windowWidth, windowHeight, "Perlin Noise");
    SetTraceLogLevel(LOG_WARNING);
    DisableCursor();

    float offsetX = 0.0f;
    float offsetY = 0.0f;
    bool isCursorEnabled = false;

    Image image = GenImageColor(definitionWidth, definitionHeight, BLACK);
    Texture2D texture = LoadTextureFromImage(image);
    UnloadImage(image);

    Camera3D camera = { 0 };
    camera.position = (Vector3){ 0.0f, 20.0f, 0.5f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Mesh terrainMesh = { 0 };
    Model terrainModel = { 0 };

    while (!WindowShouldClose()) {
        generatePerlinTexture(image, offsetX, offsetY, scale, scale, octaves, persistence);
        UpdateTexture(texture, image.data);

        if (IsKeyDown(KEY_LEFT_ALT)) {
            if (isCursorEnabled == false) {
                isCursorEnabled = true;
                EnableCursor();
            }
        } else {
            if (isCursorEnabled == true) {
                isCursorEnabled = false;
                DisableCursor();
            }
            UpdateCamera(&camera, false);
        }

        terrainMesh = GenMeshHeightmap(image, (Vector3){ 32, 16, 32 });
        terrainModel = LoadModelFromMesh(terrainMesh);
        terrainMesh = { 0 };

        Shader terrainShader = LoadShader(0, "src/shader.fs");
        int colorLocation = GetShaderLocation(terrainShader, "color");
        float colorValues[] = { 0.0f, 0.7f, 0.9f, 1.0f };
        SetShaderValue(terrainShader, colorLocation, colorValues, SHADER_UNIFORM_VEC4);
        terrainModel.materials[0].shader = terrainShader;
        terrainModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

        BeginDrawing();
            ClearBackground(RAYWHITE);

            BeginMode3D(camera);
                DrawModelEx(terrainModel, (Vector3){ -16, 0, -16 }, (Vector3){ 0.0f, 1.0f, 0.0f }, 0.0f, (Vector3){ 1.0f, 1.0f, 1.0f }, LIGHTGRAY);
            EndMode3D();

            DrawTexture(texture, windowWidth - texture.width - 20, 20, WHITE);
            scale = GuiSlider({ 55, 20, 100, 20 }, "Scale", TextFormat("%f", scale), scale, 1.0f, 100.0f);
            octaves = GuiSlider({ 55, 45, 100, 20 }, "Octaves", TextFormat("%i", octaves), octaves, 1, 6);
            persistence = GuiSlider({ 55, 70, 100, 20 }, "Decay", TextFormat("%f", persistence), persistence, 0.0f, 1.0f);
            animationSpeed = GuiSlider({ 55, 95, 100, 20 }, "Speed", TextFormat("%f", animationSpeed), animationSpeed, 0.0f, 0.01f);
        EndDrawing();

        offsetX += animationSpeed;
        offsetY += animationSpeed;

        UnloadModel(terrainModel);
    }

    UnloadMesh(terrainMesh);
    UnloadModel(terrainModel);
    UnloadImage(image);
    UnloadTexture(texture);
    EnableCursor();
    CloseWindow();
}