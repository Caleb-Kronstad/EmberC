#include "includes.h"
#include "style.h"

void setimguistyle(
    ImVec4_c text_primary, ImVec4_c background_primary,
    ImVec4_c background_secondary, ImVec4_c background_tertiary,
    ImVec4_c highlight_primary, ImVec4_c highlight_secondary)
{
    ImGuiStyle* style = igGetStyle();
    ImVec4_c* colors = style->Colors;

    colors[ImGuiCol_Text] = text_primary;
    colors[ImGuiCol_WindowBg] = background_primary;
    colors[ImGuiCol_Border] = background_tertiary;
    colors[ImGuiCol_FrameBg] = background_secondary;
    colors[ImGuiCol_FrameBgHovered] = background_tertiary;
    colors[ImGuiCol_FrameBgActive] = highlight_secondary;
    colors[ImGuiCol_TitleBg] = background_secondary;
    colors[ImGuiCol_TitleBgActive] = background_secondary;
    colors[ImGuiCol_TitleBgCollapsed] = background_secondary;
    colors[ImGuiCol_SliderGrab] = highlight_primary;
    colors[ImGuiCol_SliderGrabActive] = highlight_primary;
    colors[ImGuiCol_Header] = background_tertiary;
    colors[ImGuiCol_HeaderHovered] = highlight_primary;
    colors[ImGuiCol_HeaderActive] = highlight_primary;
    colors[ImGuiCol_TextSelectedBg] = highlight_primary;
    colors[ImGuiCol_CheckMark] = highlight_primary;
    colors[ImGuiCol_Button] = background_tertiary;
    colors[ImGuiCol_ButtonHovered] = highlight_secondary;
    colors[ImGuiCol_ButtonActive] = highlight_primary;
    colors[ImGuiCol_Separator] = background_tertiary;
    colors[ImGuiCol_SeparatorHovered] = highlight_primary;
    colors[ImGuiCol_SeparatorActive] = highlight_primary;
    colors[ImGuiCol_ResizeGrip] = highlight_primary;
    colors[ImGuiCol_ResizeGripHovered] = highlight_primary;
    colors[ImGuiCol_ResizeGripActive] = highlight_primary;
    colors[ImGuiCol_Tab] = background_primary;
    colors[ImGuiCol_TabHovered] = background_tertiary;
    colors[ImGuiCol_DockingPreview] = highlight_primary;
    colors[ImGuiCol_DockingEmptyBg] = background_secondary;
    colors[ImGuiCol_PlotHistogram] = highlight_secondary;
    colors[ImGuiCol_PlotHistogramHovered] = highlight_primary;
    colors[ImGuiCol_DragDropTarget] = highlight_secondary;

    style->WindowRounding = 0.0f;
    style->FrameRounding = 0.0f;
    style->GrabRounding = 0.0f;
    style->PopupRounding = 0.0f;
    style->TabRounding = 0.0f;
    style->WindowMenuButtonPosition = ImGuiDir_Right;
    style->ScrollbarSize = 10.0f;
    style->GrabMinSize = 10.0f;
    style->DockingSeparatorSize = 1.0f;
    style->SeparatorTextBorderSize = 2.0f;
    style->WindowPadding = (ImVec2){0.0f, 0.0f};
}