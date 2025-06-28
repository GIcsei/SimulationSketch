// === File: src/Renderer.cpp ===
#include "Renderer.h"
#include <glad/gl.h>       // Core OpenGL functions
#include "Shaders.h"
#include <iostream>

// GL handles
GLuint vao = 0, vbo = 0;
GLuint shaderProgram = 0;
struct Vertex {
    float x, y;
    int type;
};


static GLuint compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(shader, 512, nullptr, log);
        std::cerr << "Shader compile error: " << log << std::endl;
    }
    return shader;
}

void initRenderer() {
    // Compile shaders
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Create VAO/VBO
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
}

void renderAgents(const AgentList& agents) {
    std::vector<Vertex> vertices;
    for (const auto& a : agents) {
        vertices.push_back({ a.x, a.y, static_cast<int>(a.type) });
    }

    glUseProgram(shaderProgram);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);

    // Vertex format: vec2 position + int type
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribIPointer(1, 1, GL_INT, sizeof(Vertex), (void*)(sizeof(float) * 2));

    glDrawArrays(GL_POINTS, 0, vertices.size());

    glBindVertexArray(0);
    glUseProgram(0);
}

void cleanupRenderer() {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteProgram(shaderProgram);
}