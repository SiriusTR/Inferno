#pragma once

#include "WindowBase.h"
#include "Editor/Editor.Object.h"

namespace Inferno::Editor {
    class StatusBar : public WindowBase {
    public:
        StatusBar() : WindowBase("Status", nullptr, ToolbarFlags) {
            DefaultWidth = 0;
            DefaultHeight = 0;
        }
        ImVec2 Position;
        float Width = 0;
        float Height = 40 * Shell::DpiScale;

    protected:
        void BeforeUpdate() override {
            float height = ImGui::GetTextLineHeight() + 16 * Shell::DpiScale;
            ImGui::SetNextWindowPos({ Position.x, Position.y });
            ImGui::SetNextWindowSize({ Width, height });
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 1.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        }

        void AfterUpdate() override {
            ImGui::PopStyleVar(2);
        }

        void OnUpdate() override {
            auto& level = Game::Level;

            if (!ImGui::BeginTable("statusbar", 5, ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_BordersInnerV, {}, Width))
                return;

            ImGui::TableSetupColumn("status", 0, Width - (130 + 150 + 250 + 170) * Shell::DpiScale);
            ImGui::TableSetupColumn("c1", 0, 130 * Shell::DpiScale);
            ImGui::TableSetupColumn("c2", 0, 150 * Shell::DpiScale);
            ImGui::TableSetupColumn("c3", 0, 250 * Shell::DpiScale);
            ImGui::TableSetupColumn("c4", 0, 170 * Shell::DpiScale);
            //ImGui::TableSetupColumn("c5", 0, 200 * Shell::DpiScale);
            
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text(Editor::StatusText.c_str());

            if (level.SegmentExists(Editor::Selection.Segment)) {
                ImGui::TableNextColumn();
                auto [seg, side] = level.GetSegmentAndSide(Selection.Tag());

                switch (Settings::SelectionMode) {
                    case SelectionMode::Point:
                    case SelectionMode::Edge:
                        ImGui::Text("Marked: %i", Editor::Marked.Points.size());
                        break;

                    default:
                    case SelectionMode::Face:
                        ImGui::Text("Marked: %i", Editor::Marked.Faces.size());
                        break;

                    case SelectionMode::Segment:
                        ImGui::Text("Marked: %i", Editor::Marked.Segments.size());
                        break;

                    case SelectionMode::Object:
                        ImGui::Text("Marked: %i", Editor::Marked.Objects.size());
                        break;
                }

                ImGui::TableNextColumn();
                ImGui::Text("T1: %i T2: %i", side.TMap, side.TMap2);

                ImGui::TableNextColumn();
                auto vertIndex = seg.GetVertexIndex(Selection.Side, Selection.Point);
                auto& vert = level.Vertices[vertIndex];
                ImGui::Text("Point %i: %.1f, %.1f, %.1f", vertIndex, vert.x, vert.y, vert.z);
            }
            else {
                ImGui::TableNextColumn();
                ImGui::TableNextColumn();
                ImGui::TableNextColumn();
            }

            ImGui::TableNextColumn();
            ImGui::Text("Seg: %i:%i:%i",
                        Editor::Selection.Segment,
                        Editor::Selection.Side,
                        Editor::Selection.Point);

            ImGui::EndTable();

            Height = ImGui::GetWindowHeight();
        }
    };
}