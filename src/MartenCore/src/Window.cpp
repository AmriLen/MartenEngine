#include "../Window.hpp"
#include "../MartenGUI.hpp"
#include "../../MartenUtils/MartenLogger.hpp"
#include "../ShaderProgram.hpp"

#include <memory>
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

namespace MartenEngine {

    static bool s_glfw_initialized = false;


    GLfloat points[] = {
		0.0f,  0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
	   -0.5f, -0.5f, 0.0f
	};

	GLfloat colors[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	const char* vertex_shader =
		"#version 460\n"
		"layout(location = 0) in vec3 vertex_position;"
		"layout(location = 1) in vec3 vertex_color;"
		"out vec3 color;"
		"void main() {"
		"	color = vertex_color;"
		"	gl_Position = vec4(vertex_position, 1.0);"
		"}";

	const char* fragment_shader = 
		"#version 460\n"
		"in vec3 color;"
		"out vec4 frag_color;"
		"void main() {"
		"	frag_color = vec4(color, 1.0);"
		"}";

	std::unique_ptr<ShaderProgram> p_shader_program;
	GLuint vao;


    Window::Window(std::string title, const unsigned int width, const unsigned int height)
        : m_data{std::move(title), width, height} {
        int resultCode = Initialize();

        if (resultCode == 0) {
            MartenLogger::SECS("[Window::Window] Window created successfully: " + std::string(m_data.title));

            // Setup ImGui context
            if (IMGUI_CHECKVERSION()) {
                MartenLogger::SECS("[Window::Window] IMGUI " + std::string(IMGUI_VERSION));
                ImGui::CreateContext();

                std::ifstream iff("assets/fonts/Montserrat/Montserrat-Regular.ttf");

                if (!iff) {
                    MartenLogger::WARN("[Window::Window] Failed to load font: assets/fonts/Montserrat/Montserrat-Regular.ttf");
                } else {
                    ImGui::GetIO().Fonts->AddFontFromFileTTF("assets/fonts/Montserrat/Montserrat-Regular.ttf", 16.f);
                    MartenLogger::SECS("[Window::Window] Font loaded successfully: " + std::string(ImGui::GetIO().Fonts->Fonts.back()->GetDebugName()));
                }
                
                if (ImGui_ImplOpenGL3_Init()) {
                    MartenLogger::SECS("[Window::Window] ImGui OpenGL3 initialized successfully");
                    ImGui_ImplGlfw_InitForOpenGL(m_pWindow, true);
                } else {
                    MartenLogger::CRIT("[Window::Window] ImGui OpenGL3 initialization failed");
                    windowState = false;
                    Shutdown();
                }

                ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
                ImGui::GetIO().ConfigDockingAlwaysTabBar = true;
                //ImGui::GetIO().IniFilename = "guiconfig.ini";
                //ImGui::LoadIniSettingsFromMemory("guiconfig.ini");

            } else {
                MartenLogger::CRIT("[Window::Window] ImGui version check failed");
                windowState = false;
                Shutdown();
            }

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
        MartenLogger::SECS("[Window::Initialize] GLFW " + std::string(glfwGetVersionString()));

        // Set GLFW window hints
        m_pWindow = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), nullptr, nullptr);
        if (!m_pWindow) {
            MartenLogger::CRIT("[Window::Initialize] Failed creating GLFW Window -> " + std::string(m_data.title));
            glfwTerminate();
            return -2;
        }
        MartenLogger::SECS("[Window::Initialize] GLFW Window created -> " + std::string(m_data.title));

        glfwMakeContextCurrent(m_pWindow);

        // Initialize GLAD
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            MartenLogger::CRIT("[Window::Initialize] GLAD initialization failed");
            return -3;
        }
        MartenLogger::SECS("[Window::Initialize] GLAD " + std::string((const char*)glGetString(GL_VERSION)));

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

        p_shader_program = std::make_unique<ShaderProgram>(vertex_shader, fragment_shader);
        if (!p_shader_program->isCompiled()) {
            return false;
        }

        GLuint points_vbo = 0;
        glGenBuffers(1, &points_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

        GLuint colors_vbo = 0;
        glGenBuffers(1, &colors_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, nullptr);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, nullptr);

        return 0;
    }

    void Window::Shutdown() {
        if (windowState != false) {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
        }
        glfwDestroyWindow(m_pWindow);
        glfwTerminate();
    }

    void Window::on_update() {
        ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize.x = static_cast<float>(get_width());
		io.DisplaySize.y = static_cast<float>(get_height());
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();

        glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        p_shader_program->bind();
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
		ImGui::NewFrame();
        ImGui::DockSpaceOverViewport();

        ImGui::BeginMainMenuBar();
        if (ImGui::BeginMenu("File")) {
            ImGui::MenuItem("Open");
            ImGui::Separator();
            ImGui::MenuItem("Save");
            ImGui::MenuItem("Save as ...");
            ImGui::Separator();
            ImGui::MenuItem("Exit");
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            ImGui::MenuItem("Undo");
            ImGui::MenuItem("Redo");
            ImGui::Separator();
            ImGui::MenuItem("Project Settings");
            ImGui::MenuItem("Preferences");
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Window")) {
            ImGui::MenuItem("Viewport", NULL, &show_viewport_window);
            ImGui::MenuItem("Outliner", NULL, &show_outliner_window);
            ImGui::MenuItem("Properties", NULL, &show_properties_window);
            ImGui::MenuItem("File Browser", NULL, &show_fileBrowser_window);
            ImGui::MenuItem("Demo", NULL, &show_demo_window);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Build")) {
            ImGui::MenuItem("Platform");
            ImGui::MenuItem("test");
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Info")) {
            ImGui::MenuItem("About Marten Engine", NULL, &show_about_window);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();

        if (show_viewport_window) { MartenGUI::ShowViewportWindow(&show_viewport_window); }
        if (show_outliner_window) { MartenGUI::ShowOutlinerWindow(&show_outliner_window); }
        if (show_properties_window) { MartenGUI::ShowPropertiesWindow(&show_properties_window); }
        if (show_fileBrowser_window) { MartenGUI::ShowFileBrowserWindow(&show_fileBrowser_window); }
        if (show_demo_window) { MartenGUI::ShowDemoWindow();}

        if (show_about_window) { MartenGUI::ShowAboutWindow(&show_about_window); }

        
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapInterval(1); // V-Sync
        glfwSwapBuffers(m_pWindow); // Refresh window
        glfwPollEvents();
    }
}