#include "offsets.h"
#include <math.h>
#include "memory.hpp"
#include "math.h"
#include <vector>

DWORD client = Memory::GetModule(Memory::GetProcID(L"csgo.exe"), L"client.dll");
DWORD engine = Memory::GetModule(Memory::GetProcID(L"csgo.exe"), L"engine.dll");

#include "SDK/m_fFlags.h"
#include "SDK/view.h"
#include "SDK/CWeapon.h"
#include "SDK/Class.h"
#include "SDK/mstudiobbox_t.hpp"
#include "SDK/bone.hpp"
#include "SDK/PlayerInfo.hpp"
#include "SDK/dwGlobalVars.hpp"

#include "var.h"

#include "math.hpp"

#include "Hacks/ESP.hpp"
#include "Hacks/Aimbot.hpp"
#include "Hacks/AutoBhop.hpp"

#include <dwmapi.h>
#include <d3d11.h>
#include "menu.hpp"

bool isActive = false;
int ttab;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACK WINPROC(HWND window, UINT message, WPARAM w_param, LPARAM l_param) {
    if (ImGui_ImplWin32_WndProcHandler(window, message, w_param, l_param)) {
        return 1L;
    }

    return DefWindowProc(window, message, w_param, l_param);
}

INT APIENTRY WinMain(HINSTANCE instance, HINSTANCE, PSTR, INT cmd_show) {
    /*if (!AllocConsole()) {
        return FALSE;
    }

    FILE* f{ nullptr };
    freopen_s(&f, "CONIN$", "r", stdin);
    freopen_s(&f, "CONOUT$", "w", stdout);
    freopen_s(&f, "CONOUT$", "w", stderr);*/

    std::string random_str = Memory::RandomString(100);

    std::wstring stemp = std::wstring(random_str.begin(), random_str.end());
    LPCWSTR sw = stemp.c_str();

    WNDCLASSEXW wc;
    wc.cbSize = sizeof(WNDCLASSEXW),
    wc.style = CS_HREDRAW | CS_VREDRAW,
    wc.lpfnWndProc = WINPROC,
    wc.cbClsExtra = 0,
    wc.cbWndExtra = 0,
    wc.hInstance = instance,
    wc.hIcon = nullptr,
    wc.hCursor = nullptr,
    wc.hbrBackground = nullptr,
    wc.lpszMenuName = nullptr,
    wc.lpszClassName = sw,
    wc.hIconSm = nullptr;

    RegisterClassExW(&wc);

    const HWND window = CreateWindowExW( WS_EX_TRANSPARENT | WS_EX_LAYERED, wc.lpszClassName, sw, WS_POPUP, 0, 0, 1920, 1080, nullptr, nullptr, wc.hInstance, nullptr);

    if (!window)
    {
        UnregisterClassW(wc.lpszClassName, wc.hInstance); MessageBoxA(NULL, "Failed: Error 1.00\nPlease contact developer.", "Error!", MB_ICONERROR);
    }

    if (!SetLayeredWindowAttributes(window, RGB(0, 0, 0), BYTE(255), LWA_ALPHA))
    {
        DestroyWindow(window); UnregisterClassW(wc.lpszClassName, wc.hInstance); MessageBoxA(NULL, "Failed: Error 1.01\nPlease contact developer.", "Error!", MB_ICONERROR);
    }

    {
        RECT client_area{};
        if (!GetClientRect(window, &client_area))
        {
            DestroyWindow(window); UnregisterClassW(wc.lpszClassName, wc.hInstance); MessageBoxA(NULL, "Failed: Error 1.02\nPlease contact developer.", "Error!", MB_ICONERROR);
        }

        RECT window_area{};
        if (!GetWindowRect(window, &window_area))
        {
            DestroyWindow(window); UnregisterClassW(wc.lpszClassName, wc.hInstance); MessageBoxA(NULL, "Failed: Error 1.03\nPlease contact developer.", "Error!", MB_ICONERROR);
        }

        POINT diff{};
        if (!ClientToScreen(window, &diff))
        {
            DestroyWindow(window); UnregisterClassW(wc.lpszClassName, wc.hInstance); MessageBoxA(NULL, "Failed: Error 1.04\nPlease contact developer.", "Error!", MB_ICONERROR);
        }

        const MARGINS margins{
            window_area.left + (diff.x - window_area.left),
            window_area.top + (diff.y - window_area.top),
            client_area.right,
            client_area.bottom
        };

        if (FAILED(DwmExtendFrameIntoClientArea(window, &margins)))
        {
            DestroyWindow(window); UnregisterClassW(wc.lpszClassName, wc.hInstance); MessageBoxA(NULL, "Failed: Error 1.05\nPlease contact developer.", "Error!", MB_ICONERROR);
        }
    }

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));

    sd.BufferDesc.Width = 0U;
    sd.BufferDesc.Height = 0U;
    sd.BufferDesc.RefreshRate.Numerator = 60U;
    sd.BufferDesc.RefreshRate.Denominator = 1U;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.SampleDesc.Count = 1U;
    sd.SampleDesc.Quality = 0U;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = 2U;
    sd.OutputWindow = window;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    constexpr D3D_FEATURE_LEVEL lvls[2]{
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_0
    };

    D3D_FEATURE_LEVEL lvl{};

    ID3D11Device* device = nullptr;
    ID3D11DeviceContext* context = nullptr;
    IDXGISwapChain* sc = nullptr;
    ID3D11RenderTargetView* rtv = nullptr;

    if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0U, lvls, 2U, D3D11_SDK_VERSION, &sd, &sc, &device, &lvl, &context)))
    {
        DestroyWindow(window); UnregisterClassW(wc.lpszClassName, wc.hInstance); MessageBoxA(NULL, "Failed: Error 1.06\nPlease contact developer.", "Error!", MB_ICONERROR);
    }

    HWND Target = FindWindow(L"Valve001", L"Counter-Strike: Global Offensive - Direct3D 9");

    if (!Target)
    {
        DestroyWindow(window); UnregisterClassW(wc.lpszClassName, wc.hInstance); MessageBoxA(NULL, "Failed: Error 1.07, failed to find CSGO\nPlease contact developer.", "Error!", MB_ICONERROR);
    }

    RECT rect;

    GetWindowRect(Target, &rect);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    SetWindowLong(window, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT);

    SetLayeredWindowAttributes(window, RGB(0, 0, 0), 255, LWA_ALPHA);

    ID3D11Texture2D* back_buffer = nullptr;

    if (FAILED(sc->GetBuffer(0U, IID_PPV_ARGS(&back_buffer))))
    {
        MessageBoxA(NULL, "Failed: Error 1.08\nPlease contact developer.", "Error!", MB_ICONERROR);
    }

    if (FAILED(device->CreateRenderTargetView(back_buffer, nullptr, &rtv)))
    {
        MessageBoxA(NULL, "Failed: Error 1.09, failed to find CSGO\nPlease contact developer.", "Error!", MB_ICONERROR);
    }

    if (back_buffer) {
        back_buffer->Release();
    }

    ShowWindow(window, cmd_show);
    UpdateWindow(window);

    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    InitStyle();

    ImGui_ImplWin32_Init(window);
    ImGui_ImplDX11_Init(device, context);

    bool running = true;
    while (running) {
        MSG message;
        while (PeekMessage(&message, nullptr, 0U, 0U, PM_REMOVE)) {
            TranslateMessage(&message);
            DispatchMessage(&message);

            if (message.message == WM_QUIT) {
                running = false;
            }
        }

        if (!running) {
            break;
        }


        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        SetWindowPos(Target, window, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

        if (GetAsyncKeyState(VK_INSERT) & 1)
        {
            isActive = !isActive;

            if (isActive)
            {
                SetWindowLong(window, GWL_EXSTYLE, WS_EX_TRANSPARENT);
                SetWindowLong(Target, GWL_EXSTYLE, WS_EX_TRANSPARENT | WS_EX_LAYERED);
                SetActiveWindow(window);
            }
            else
            {
                SetWindowLong(window, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT);
                SetWindowLong(Target, GWL_EXSTYLE, WS_EX_TRANSPARENT);
                SetActiveWindow(Target);
            }
        }

        if (GetAsyncKeyState(VK_END) & 1)
        {
            running = false;
        }

        if (isActive)
        {
            DrawMenu();
        }

        ViewMatrix vm = Memory::Read<ViewMatrix>(client + offsets::dwViewMatrix);
        uintptr_t LocalPlayer = Memory::Read<uintptr_t>(client + offsets::dwLocalPlayer);
        /*byte m_fFlags = Memory::Read<byte>(LocalPlayer + offsets::m_fFlags);*/

        CGlobalVars GlobalVars = Memory::Read<CGlobalVars>(engine + offsets::dwGlobalVars);

        Vector aimPunch = Memory::Read<Vector>(LocalPlayer + offsets::m_aimPunchAngle);
        int shotsFired = Memory::Read<int>(LocalPlayer + offsets::m_iShotsFired);

        aimPunch.x = aimPunch.x * 12; aimPunch.y = aimPunch.y * 12;
        float x = width / 2 - aimPunch.y;
        float y = height / 2 + aimPunch.x;

        int localTeamNum = Memory::Read<int>(LocalPlayer + offsets::m_iTeamNum);
        int ObserverTarget = Memory::Read<int>(LocalPlayer + offsets::m_hObserverTarget) & 0xFFF;

        if (var::recoilCrossHair)
        {
            ImGui::GetBackgroundDrawList()->AddCircle({ x, y }, 5, ImColor(1.f, 1.f, 1.f), 0, 1);
        }

        if (var::spectatorList)
        {
            ImGui::Begin("Spectators", &isActive, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMouseInputs);
            {
                for (int i = 0; i < 32; i++)
                {
                    const auto entity = Memory::Read<uintptr_t>(client + offsets::dwEntityList + i * 0x10);

                    int LocalIndex = Memory::Read<int>(LocalPlayer + 0x64);

                    int ObserverTarget = Memory::Read<int>(entity + offsets::m_hObserverTarget) & 0xFFF;

                    if (ObserverTarget == LocalIndex)
                        ImGui::Text(GetPlayerName(i));
                }
            }
        }

        if (LocalPlayer)
            if (var::entityEsp)
            {
                for (int i = 0; i < 1048; i++)
                {
                    const auto entity = Memory::Read<uintptr_t>(client + offsets::dwEntityList + i * 0x10);

                    if (!entity)
                        continue;

                    if (Memory::Read<bool>(entity + offsets::m_bDormant))
                        continue;

                    EntityEsp(entity, vm, GlobalVars);

                    if (Memory::Read<int>(entity + offsets::m_lifeState) != 0)
                        continue;

                    if (GetClassID(entity) != CCSPlayer)
                        continue;

                    if (entity == LocalPlayer)
                        continue;

                    int entTeamNum = Memory::Read<int>(entity + offsets::m_iTeamNum);
                    int Health = Memory::Read<int>(entity + offsets::m_iHealth);
                    int Armor = Memory::Read<int>(entity + offsets::m_ArmorValue);

                    if (var::ignoreTeam && entTeamNum == localTeamNum)
                        continue;

                    int Entityindex = Memory::Read<int>(entity + 0x64);

                    if (ObserverTarget == Entityindex)
                        continue;

                    uintptr_t BoneMatrix = Memory::Read<uintptr_t>(entity + offsets::m_dwBoneMatrix);

                    ESP(entity, i, vm, Health, Armor, BoneMatrix);

                    if (var::skeletonEsp)
                    {
                        SkeletonESP(entity, BoneMatrix, vm);
                    }
                }
            }
            else
            {
                for (int i = 0; i < GlobalVars.maxClients; i++)
                {
                    const auto entity = Memory::Read<uintptr_t>(client + offsets::dwEntityList + i * 0x10);

                    if (!entity)
                        continue;

                    if (Memory::Read<bool>(entity + offsets::m_bDormant))
                        continue;

                    if (Memory::Read<int>(entity + offsets::m_lifeState) != 0)
                        continue;

                    if (GetClassID(entity) != CCSPlayer)
                        continue;

                    if (entity == LocalPlayer)
                        continue;

                    int entTeamNum = Memory::Read<int>(entity + offsets::m_iTeamNum);
                    int Health = Memory::Read<int>(entity + offsets::m_iHealth);
                    int Armor = Memory::Read<int>(entity + offsets::m_ArmorValue);

                    if (var::ignoreTeam && entTeamNum == localTeamNum)
                        continue;

                    int Entityindex = Memory::Read<int>(entity + 0x64);

                    if (ObserverTarget == Entityindex)
                        continue;

                    uintptr_t BoneMatrix = Memory::Read<uintptr_t>(entity + offsets::m_dwBoneMatrix);

                    ESP(entity, i, vm, Health, Armor, BoneMatrix);

                    if (var::skeletonEsp)
                    {
                        SkeletonESP(entity, BoneMatrix, vm);
                    }
                }
            }

        ImGui::Render();

        constexpr float color[]{ 0.f, 0.f, 0.f, 0.f };
        context->OMSetRenderTargets(1U, &rtv, nullptr);
        context->ClearRenderTargetView(rtv, color);

        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        sc->Present(0U, 0U);
    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();

    ImGui::DestroyContext();

    sc->Release();
    context->Release();
    device->Release();
    rtv->Release();

    DestroyWindow(window);
    UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}
