// === File: src/main.cpp ===
#include <cstdlib>
#include <GLFW/glfw3.h>
#include <glad/gl.h>       // Core OpenGL functions
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "main.h"
#include "Agent.h"
#include "Performance.h"
#include "Simulation.h"
#include "Renderer.h"

int main() {
    if (!glfwInit()) return -1;
    GLFWwindow* window = glfwCreateWindow(1920, 1080, "Evolution Simulation", nullptr, nullptr);
    if (!window) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // === Initialize GLAD ===
    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    // === Setup OpenGL state ===
    glEnable(GL_PROGRAM_POINT_SIZE); // Allow setting point size in shaders
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    // === Initialize ImGui ===
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
    
    // === Initialize Renderer ===
    initRenderer();

    // Initialize agents
    AgentList agents;
    agents.reserve(2000);
    for (int i = 0; i < agents.capacity(); ++i) {
        Agent a;
        a.type = (i % 2 == 0 ? Agent::Carnivore : Agent::Vegetarian);
        a.x = (rand() / (float)RAND_MAX) * 2 - 1;
        a.y = (rand() / (float)RAND_MAX) * 2 - 1;
        a.energy = 1.0f;
        agents.push_back(a);
    }

    int carnivoreCount;
    double lastTime = getTimeSeconds();
    while (!glfwWindowShouldClose(window)) {
        double now = getTimeSeconds();
        double dt = now - lastTime;
        lastTime = now;

        // Simulation
        simulateStep(dt, agents);

        carnivoreCount = 0;
        for (const auto& a : agents) {
            if (a.type == Agent::Carnivore) ++carnivoreCount;
        }
        // Render
        glClear(GL_COLOR_BUFFER_BIT);
        renderAgents(agents);

        // UI
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Performance");
        ImGui::Text("FPS: %.1f", 1.0f / dt);
        ImGui::Text("CPU: %.1f%%", cpuUsagePercent());
        ImGui::Text("RAM: %.1f MB", ramUsageMB());
        ImGui::Text("Carnivores: %d", carnivoreCount);
        ImGui::Text("Vegetarians: %d", agents.size() - carnivoreCount);
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    cleanupRenderer();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
