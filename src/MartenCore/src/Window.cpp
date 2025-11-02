#include "../Window.hpp"
#include "../../MartenUtils/MartenLogger.h"

#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

namespace MartenEngine {

    static bool s_glfw_initialized = false;

    Window::Window(std::string title, const unsigned int width, const unsigned int height)
        : m_data{std::move(title), width, height} {
        int resultCode = Initialize();

        if (resultCode == 0) {
            MartenLogger::SECS("[Window::Window] Window created successfully: " + std::string(m_data.title));

            // Setup ImGui context
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGui_ImplOpenGL3_Init();
            ImGui_ImplGlfw_InitForOpenGL(m_pWindow, true);

        } else {
            MartenLogger::CRIT("[Window::Window] Window creation failed with code: " + std::to_string(resultCode));
            windowState = false;
            Shutdown();
        }
    }

    Window::~Window() {
        Shutdown();
    }

    int Window::Initialize() {
        MartenLogger::INFO("[Window::Initialize] Creating window: " + std::string(m_data.title) + " (" + std::to_string(m_data.width) + "x" + std::to_string(m_data.height) + ")");
        
        // Initialize GLFW only once
        if (!s_glfw_initialized) {
            if (!glfwInit()) {
                MartenLogger::CRIT("[Window::Initialize] GLFW initialization failed");
                return -1;
            }
            s_glfw_initialized = true;
        }
        MartenLogger::SECS("[Window::Initialize] GLFW initialization");

        // Set GLFW window hints
        m_pWindow = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), nullptr, nullptr);
        if (!m_pWindow) {
            MartenLogger::CRIT("[Window::Initialize] Failed creating GLFW window -> " + std::string(m_data.title));
            glfwTerminate();
            return -2;
        }
        MartenLogger::SECS("[Window::Initialize] GLFW window created -> " + std::string(m_data.title));

        glfwMakeContextCurrent(m_pWindow);

        // Initialize GLAD
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            MartenLogger::CRIT("[Window::Initialize] GLAD initialization failed");
            return -3;
        }
        MartenLogger::SECS("[Window::Initialize] GLAD initialization");

        glfwSetWindowUserPointer(m_pWindow, &m_data);

        glfwSetWindowSizeCallback(m_pWindow, [](GLFWwindow* pWindow, int width, int height) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
			data.width = width;
			data.height = height;

			EventWindowResize event(width, height);
			data.eventCallbackFn(event);
		});

		glfwSetCursorPosCallback(m_pWindow, [](GLFWwindow* pWindow, double x, double y) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));

			EventMouseMoved event(x, y);
			data.eventCallbackFn(event);
		});

		glfwSetWindowCloseCallback(m_pWindow, [](GLFWwindow* pWindow) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));

			EventWindowClose event;
			data.eventCallbackFn(event);
		});

        glfwSetFramebufferSizeCallback(m_pWindow, [](GLFWwindow* window, int width, int height) {
            glViewport(0, 0, width, height);
        });

        return 0;
    }

    void Window::Shutdown() {
        glfwDestroyWindow(m_pWindow);
        glfwTerminate();
    }

    void Window::on_update() {
        glClearColor(m_background_color[0], m_background_color[1], m_background_color[2], m_background_color[3]);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize.x = static_cast<float>(get_width());
		io.DisplaySize.y = static_cast<float>(get_height());
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::ShowDemoWindow(); // DEMO окно

        ImGui::BeginMainMenuBar();
        if (ImGui::BeginMenu("Edit")) {
            ImGui::MenuItem("Item 1");
            ImGui::MenuItem("Item 2");
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View")) {
            ImGui::MenuItem("Item 1");
            ImGui::MenuItem("Item 2");
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("About")) {
            if (ImGui::MenuItem("Item 1")) {
                MartenLogger::INFO("About Item 1 clicked");
                ImGui::OpenPopup("Delete?");
                //ImVec2 center = ImGui::GetMainViewport()->GetCenter();
                //ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
                if (ImGui::BeginPopupModal("Delete?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
                {
                    ImGui::Text("All those beautiful files will be deleted.\nThis operation cannot be undone!");
                    ImGui::Separator();

                    //static int unused_i = 0;
                    //ImGui::Combo("Combo", &unused_i, "Delete\0Delete harder\0");

                    static bool dont_ask_me_next_time = false;
                    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
                    ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
                    ImGui::PopStyleVar();

                    if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
                    ImGui::SetItemDefaultFocus();
                    ImGui::SameLine();
                    if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
                    ImGui::EndPopup();
                }
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();

		ImGui::Begin("Background Color Window");
		ImGui::ColorEdit4("Background Color", m_background_color);
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapInterval(0); // V-Sync
        glfwSwapBuffers(m_pWindow); // Refresh window
        glfwPollEvents();
    }

}