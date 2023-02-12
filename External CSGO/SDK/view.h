#pragma once

#include "vector.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"

struct ViewMatrix {
    ViewMatrix() noexcept
        : data() {}

    float* operator[](int index) noexcept {
        return data[index];
    }

    const float* operator[](int index) const noexcept {
        return data[index];
    }

    float data[4][4];
};

static bool w2s(const Vector& world, Vector& screen, const ViewMatrix& vm) noexcept {
    float w = vm[3][0] * world.x + vm[3][1] * world.y + vm[3][2] * world.z + vm[3][3];

    if (w < 0.001f) {
        return false;
    }

    const float x = world.x * vm[0][0] + world.y * vm[0][1] + world.z * vm[0][2] + vm[0][3];
    const float y = world.x * vm[1][0] + world.y * vm[1][1] + world.z * vm[1][2] + vm[1][3];

    w = 1.f / w;
    float nx = x * w;
    float ny = y * w;

    const ImVec2 size = ImGui::GetIO().DisplaySize;

    screen.x = (size.x * 0.5f * nx) + (nx + size.x * 0.5f);
    screen.y = -(size.y * 0.5f * ny) + (ny + size.y * 0.5f);

    return true;
}