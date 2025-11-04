#include "../MartenGUI.hpp"
#include "../../MartenData.h"

#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

namespace MartenGUI {

    void ShowViewportWindow(bool *p_open) {
        if (!ImGui::Begin("Viewport", p_open, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar)){
            ImGui::End();
            return;
        }
        ImGui::BeginMenuBar();
        if (ImGui::BeginMenu("fff")) {
            ImGui::MenuItem("Open");
            ImGui::MenuItem("Open");
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("ddd")) {
            ImGui::MenuItem("Open");
            ImGui::MenuItem("Open");
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
        ImGui::End();
    }
    
    void ShowOutlinerWindow(bool *p_open) {
        if (!ImGui::Begin("Outliner", p_open, ImGuiWindowFlags_AlwaysAutoResize)){
            ImGui::End();
            return;
        }
        if (ImGui::TreeNode("Parent")) {
            // if (ImGui::BeginTable("3ways", 3)) {
            //     ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide);
            //     ImGui::TableSetupColumn("Size", ImGuiTableColumnFlags_WidthFixed, 1.0 * 12.0f);
            //     ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, 1.0 * 18.0f);
            //     ImGui::TableHeadersRow();
            // }
            // ImGui::TreePop();
            // need other table

            if (ImGui::TreeNodeEx("Child 1", ImGuiTreeNodeFlags_DrawLinesFull)) {
                ImGui::Button("Button for Child 1");
                ImGui::TreePop();
            }
            if (ImGui::TreeNodeEx("Child 2", ImGuiTreeNodeFlags_DrawLinesFull)) {
                ImGui::Button("Button for Child 2");
                ImGui::TreePop();
            }
            ImGui::Text("Remaining contents");
            ImGui::Text("Remaining contents");
            ImGui::TreePop();
        }
        ImGui::End();
    }

    void ShowPropertiesWindow(bool *p_open) {
        if (!ImGui::Begin("Properties", p_open, ImGuiWindowFlags_AlwaysAutoResize)){
            ImGui::End();
            return;
        }
        ImGui::End();
    }
    
    void ShowFileBrowserWindow(bool *p_open) {
        if (!ImGui::Begin("FileBrowser", p_open, ImGuiWindowFlags_AlwaysAutoResize)){
            ImGui::End();
            return;
        }
        ImGui::End();
    }

    void ShowAboutWindow(bool *p_open) {
        if (!ImGui::Begin("About Marten Engine", p_open, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoResize)){
            ImGui::End();
            return;
        }
        ImGui::Text("Version: %s : %s", ME_VERSION_PREFIX, ME_VERSION);
        ImGui::Separator();
        ImGui::Text("(c) 2025 %s", ME_AUTHOR);
        ImGui::Text("Developed by Aichuvakov Amir.");
        ImGui::Text("Marten Engine is licensed under the MIT License, see LICENSE for more information.");
        ImGui::End();
    }

    void ShowDemoWindow() {
        ImGui::ShowDemoWindow();
    }
}