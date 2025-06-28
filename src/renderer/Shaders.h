// === File: src/Shaders.h ===
#pragma once

const char* vertexShaderSource = R"glsl(
    #version 330 core
    layout (location = 0) in vec2 aPos;
    layout (location = 1) in int aType;

    flat out int type;

    void main() {
        type = aType;
        gl_Position = vec4(aPos, 0.0, 1.0);
    }
)glsl";

const char* fragmentShaderSource = R"glsl(
    #version 330 core
    flat in int type;
    out vec4 FragColor;

    void main() {
        if (type == 1)       // Vegetarian
            FragColor = vec4(0.0, 1.0, 0.0, 1.0);
        else if (type == 0)  // Carnivore
            FragColor = vec4(1.0, 0.0, 0.0, 1.0);
        else
            FragColor = vec4(0.0, 0.0, 1.0, 1.0); // fallback color
    }
)glsl";
