#include "includes.h"
#include "style.h"

void style_set(style* style)
{
    ImGuiStyle* imgui_style = igGetStyle();
    ImVec4_c* colors = imgui_style->Colors;

    colors[ImGuiCol_Text] = style->text_primary;
    colors[ImGuiCol_WindowBg] = style->background_primary;
    colors[ImGuiCol_Border] = style->background_tertiary;
    colors[ImGuiCol_FrameBg] = style->background_secondary;
    colors[ImGuiCol_FrameBgHovered] = style->background_tertiary;
    colors[ImGuiCol_FrameBgActive] = style->highlight_secondary;
    colors[ImGuiCol_TitleBg] = style->background_secondary;
    colors[ImGuiCol_TitleBgActive] = style->background_secondary;
    colors[ImGuiCol_TitleBgCollapsed] = style->background_secondary;
    colors[ImGuiCol_SliderGrab] = style->highlight_primary;
    colors[ImGuiCol_SliderGrabActive] = style->highlight_primary;
    colors[ImGuiCol_Header] = style->background_tertiary;
    colors[ImGuiCol_HeaderHovered] = style->highlight_primary;
    colors[ImGuiCol_HeaderActive] = style->highlight_primary;
    colors[ImGuiCol_TextSelectedBg] = style->highlight_primary;
    colors[ImGuiCol_CheckMark] = style->highlight_primary;
    colors[ImGuiCol_Button] = style->background_tertiary;
    colors[ImGuiCol_ButtonHovered] = style->highlight_secondary;
    colors[ImGuiCol_ButtonActive] = style->highlight_primary;
    colors[ImGuiCol_Separator] = style->background_tertiary;
    colors[ImGuiCol_SeparatorHovered] = style->highlight_primary;
    colors[ImGuiCol_SeparatorActive] = style->highlight_primary;
    colors[ImGuiCol_ResizeGrip] = style->highlight_primary;
    colors[ImGuiCol_ResizeGripHovered] = style->highlight_primary;
    colors[ImGuiCol_ResizeGripActive] = style->highlight_primary;
    colors[ImGuiCol_Tab] = style->background_primary;
    colors[ImGuiCol_TabHovered] = style->background_tertiary;
    colors[ImGuiCol_DockingPreview] = style->highlight_primary;
    colors[ImGuiCol_DockingEmptyBg] = style->background_secondary;
    colors[ImGuiCol_PlotHistogram] = style->highlight_secondary;
    colors[ImGuiCol_PlotHistogramHovered] = style->highlight_primary;
    colors[ImGuiCol_DragDropTarget] = style->highlight_secondary;

    imgui_style->WindowRounding = 0.0f;
    imgui_style->FrameRounding = 0.0f;
    imgui_style->GrabRounding = 0.0f;
    imgui_style->PopupRounding = 0.0f;
    imgui_style->TabRounding = 0.0f;
    imgui_style->WindowMenuButtonPosition = ImGuiDir_Right;
    imgui_style->ScrollbarSize = 10.0f;
    imgui_style->GrabMinSize = 10.0f;
    imgui_style->DockingSeparatorSize = 1.0f;
    imgui_style->SeparatorTextBorderSize = 2.0f;
    imgui_style->WindowPadding = (ImVec2){0.0f, 0.0f};
}