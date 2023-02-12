#pragma once

#include <sstream>
#include <iostream>

void DrawCenterText(const ImVec2& centerPoint, const ImColor& color, const std::string& string)
{
    ImVec2 textSize{ ImGui::CalcTextSize(string.c_str()) };
    const float offset{ textSize.x / 2.0f };

    ImGui::GetBackgroundDrawList()->AddText({ centerPoint.x - offset, centerPoint.y }, color, string.c_str());
}

void CornerRect(float x, float y, float w, float h, float _x, float _y, const ImColor& clr)
{
    ImGui::GetBackgroundDrawList()->AddLine( { x, y } , { x + (w / _x), y } , clr);
    ImGui::GetBackgroundDrawList()->AddLine( { x, y } , { x, y + (h / _y) } , clr);

    ImGui::GetBackgroundDrawList()->AddLine( { x + w, y } , { x + w - (w / _x), y } , clr);
    ImGui::GetBackgroundDrawList()->AddLine( { x + w, y } , { x + w, y + (h / _y) } , clr);

    ImGui::GetBackgroundDrawList()->AddLine( { x, y + h } , { x + (w / _x), y + h } , clr);
    ImGui::GetBackgroundDrawList()->AddLine( { x, y + h } , { x, y + h - (h / _y) } , clr);

    ImGui::GetBackgroundDrawList()->AddLine({ x + w, y + h }, { x + w - (w / _x), y + h } , clr);
    ImGui::GetBackgroundDrawList()->AddLine({ x + w, y + h }, { x + w, y + h - (h / _y) } , clr);
}

int GetClassID(uintptr_t entity)
{
    uintptr_t dwClientNetworkable = Memory::Read<uintptr_t>(entity + 0x8);
    uintptr_t dwGetClientClassFn = Memory::Read<uintptr_t>(dwClientNetworkable + 0x8);
    uintptr_t dwEntityClientClass = Memory::Read<uintptr_t>(dwGetClientClassFn + 0x1);

    return Memory::Read<int>(dwEntityClientClass + 0x14);
}

void ESP(uintptr_t entity, int index, ViewMatrix vm, int Health, int Armor, uintptr_t BoneMatrix)
{

    const auto bones = Memory::Read<uintptr_t>(entity + offsets::m_dwBoneMatrix);

    if (bones)
    {
        Vector head = GetBonePos(entity, BoneMatrix, 8);

        Vector feet = Memory::Read<Vector>(entity + offsets::m_vecOrigin);

        Vector Shead;
        Vector Sfeet;
        if (w2s(Shead + Vector{ 0, 0, 11.f }, head, vm) && w2s(feet - Vector{ 0, 0, 7.f }, Sfeet, vm)) {
            const float h = Sfeet.y - Shead.y;
            const float w = h * 0.25f;

            /*Box Esp*/
            if (var::boxOutLine && var::boxEsp)
            {
                ImGui::GetBackgroundDrawList()->AddRect({ Shead.x - w + 1, Shead.y + 1 }, { Shead.x + w - 1, Sfeet.y - 1 }, ImColor(0.f, 0.f, 0.f, 0.95f));
                ImGui::GetBackgroundDrawList()->AddRect({ Shead.x - w - 1, Shead.y - 1 }, { Shead.x + w + 1, Sfeet.y + 1 }, ImColor(0.f, 0.f, 0.f, 0.95f));
            }

            if (var::boxEsp)
                ImGui::GetBackgroundDrawList()->AddRect({ Shead.x - w, Shead.y }, { Shead.x + w, Sfeet.y }, ImColor(1.f, 1.f, 0.95f));

            if (var::boxOutLine && var::cornersEsp)
            {
                CornerRect((Shead.x - w) + 1, Shead.y + 1, (w * 2) - 2, h - 2, 3, 5, ImColor(0.f, 0.f, 0.f, 0.95f));
                CornerRect((Shead.x - w) - 1, Shead.y - 1, (w * 2) + 2, h + 2, 3, 5, ImColor(0.f, 0.f, 0.f, 0.95f));
            }

            if (var::cornersEsp)
                CornerRect( Shead.x - w, Shead.y, w * 2, h, 3, 5, ImColor(1.f, 1.f, 1.f));

            if (var::nameEsp)
                DrawCenterText({ Shead.x, Shead.y - 15 }, ImColor(1.f, 1.f, 1.f), GetPlayerName(index));

            /*Health Bar*/
            if (var::healthBarEsp)
            {
                ImGui::GetBackgroundDrawList()->AddRectFilled({ (Shead.x - w) - 12, Shead.y - 1 }, { (Shead.x - w) - 8, (Sfeet.y) + 1 }, ImColor(0.f, 0.f, 0.f));
                ImGui::GetBackgroundDrawList()->AddRectFilled({ (Shead.x - w) - 11, Sfeet.y }, { Shead.x - w - 9, (Sfeet.y - (Health * h) / 100) }, GetHealthColor(Health, 100));
            }

            if (var::armorBarEsp)
            {
                ImGui::GetBackgroundDrawList()->AddRectFilled({ Shead.x - w - 1, Sfeet.y + 11 }, { Shead.x + w + 1, Sfeet.y + 7 }, ImColor(0.f, 0.f, 0.f));
                ImGui::GetBackgroundDrawList()->AddRectFilled({ Shead.x - w, Sfeet.y + 10 }, { (Shead.x + (Armor * w) / 100), Sfeet.y + 8 }, ImColor(0.f, 0.5f, 1.f));
            }

            if (var::activeWeaponEsp)
            {
                int ActiveWeapon = Memory::Read<int>(entity + offsets::m_hActiveWeapon) & 0xFFF;
                ActiveWeapon = Memory::Read<int>(client + offsets::dwEntityList + (ActiveWeapon - 1) * 0x10);
                int ActiveWeaponID = Memory::Read<int>(ActiveWeapon + offsets::m_iItemDefinitionIndex);

                if (ActiveWeaponID > 64 || ActiveWeaponID < 1)
                    ActiveWeaponID = 42;

                const auto weapon_name = strcmp(weapon_names[ActiveWeaponID], "none") ? weapon_names[ActiveWeaponID] : "KNIFE";

                DrawCenterText({ Shead.x, Sfeet.y + 12 }, ImColor(1.f, 1.f, 1.f), weapon_name);
            }
        }
    }

}

void SkeletonESP(uintptr_t entity, uintptr_t BoneMatrix, ViewMatrix vm)
{

    uintptr_t dwStudioHdr = Memory::Read<uintptr_t>(entity + offsets::m_pStudioHdr);
    uintptr_t dwStudioHdr2 = Memory::Read<uintptr_t>(dwStudioHdr);

    int HitboxSetIndex = Memory::Read<int>(dwStudioHdr2 + 0xB0);

    int HitBoxNameIndex = Memory::Read<int>(dwStudioHdr2 + 0xC);

    /*ReadProcessMemory(Memory::processHandle, reinterpret_cast<void*>(dwStudioHdr2 + 0xC), &buffer, sizeof(buffer), nullptr);*/

    uintptr_t dwHitBoxSet = dwStudioHdr2 + HitboxSetIndex;

    int HitboxIndex = Memory::Read<int>(dwHitBoxSet + 0x8);

    for (int i = 1; i < 20; i++)
    {
        int HitBoxNum = i;

        if (HitBoxNum == 1 || HitBoxNum == 2)
            continue;

        uintptr_t dwHitBox = dwHitBoxSet + HitboxIndex + (i * sizeof(mstudiobbox_t));

        mstudiobbox_t HitBox = Memory::Read<mstudiobbox_t>(dwHitBox);

        if (HitBoxNum != Hitbox_Right_Upper_Arm || HitBoxNum != Hitbox_Left_Upper_Arm)
        {
            Vector Bone = GetBonePos(entity, BoneMatrix, HitBox.bone);

            Vector SBone;

            Vector NextBone = GetBonePos(entity, BoneMatrix, HitBox.bone + 1);

            Vector SNextBone;
            if (w2s(Bone, SBone, vm) && w2s(NextBone, SNextBone, vm))
            {
                ImGui::GetBackgroundDrawList()->AddLine({ SBone.x, SBone.y }, { SNextBone.x, SNextBone.y }, ImColor(1.f, 1.f, 1.f), 1);
            }
        }

        if (HitBoxNum == Hitbox_Right_Upper_Arm || HitBoxNum == Hitbox_Left_Upper_Arm)
        {
            Vector Neck = GetBonePos(entity, BoneMatrix, 7);
            Vector SNeck;

            Vector Chest = GetBonePos(entity, BoneMatrix, 6);
            Vector SChest;

            Vector ShoulderBone = GetBonePos(entity, BoneMatrix, HitBox.bone);
            Vector SShoulderBone;

            if (w2s(Chest, SChest, vm) && w2s(ShoulderBone, SShoulderBone, vm), w2s(Neck, SNeck, vm))
            {
                ImGui::GetBackgroundDrawList()->AddLine({ SChest.x + ((SNeck.x - SChest.x) / 2), SChest.y + ((SNeck.y - SChest.y) / 2) }, { SShoulderBone.x, SShoulderBone.y }, ImColor(1.f, 1.f, 1.f), 1);
            }
        }

        if (HitBoxNum == Hitbox_Right_Thigh || HitBoxNum == Hitbox_Left_Thigh)
        {
            Vector Waist = GetBonePos(entity, BoneMatrix, 0);
            Vector SWaist;

            Vector HipBone = GetBonePos(entity, BoneMatrix, HitBox.bone);
            Vector SHipBone;

            if (w2s(Waist, SWaist, vm) && w2s(HipBone, SHipBone, vm))
            {
                ImGui::GetBackgroundDrawList()->AddLine({ SWaist.x, SWaist.y }, { SHipBone.x, SHipBone.y }, ImColor(1.f, 1.f, 1.f), 1);
            }
        }

        Vector LowerSpine = GetBonePos(entity, BoneMatrix, 3);
        Vector SLowerSpine;

        Vector Waist = GetBonePos(entity, BoneMatrix, 0);
        Vector SWaist;

        if (w2s(LowerSpine, SLowerSpine, vm) && w2s(Waist, SWaist, vm))
        {
            ImGui::GetBackgroundDrawList()->AddLine({ SLowerSpine.x, SLowerSpine.y }, { SWaist.x, SWaist.y }, ImColor(1.f, 1.f, 1.f), 1);
        }
    }
}

void EntityEsp(uintptr_t entity, ViewMatrix vm, CGlobalVars vars)
{
    int classId = GetClassID(entity);

    auto Origin = Memory::Read<Vector>(entity + offsets::m_vecOrigin);

    Vector pos;

    if (w2s(Origin, pos, vm))
    {

        if (classId == CPlantedC4)
        {
            if (Memory::Read<bool>(client + offsets::m_bBombPlanted))
                return;

            float bombTime = Memory::Read<float>(entity + offsets::m_flC4Blow);

            float timer = bombTime - vars.curtime;
            if (0.f > timer || timer > 60.f)
                return;

            char tmp[32];
            sprintf_s(tmp, "Bomb\n%.2f", timer);

            DrawCenterText({ pos.x, pos.y }, ImColor(1.f, 0.f, 0.f), tmp);
            
        }

        if (classId == CInferno)
        {
            float spawnTime = Memory::Read<float>(entity + offsets::m_flSimulationTime);

            float timer = ((spawnTime + 7.03125f) - vars.curtime) / 7.03125f;

            /*char tmp[32];
            sprintf_s(tmp, "Inferno\n%.2f", timer);

            DrawCenterText({ pos.x, pos.y }, ImColor(1.f, 0.f, 0.f), tmp);*/

            DrawCenterText({ pos.x, pos.y + 15 }, ImColor(1.f, 0.5f, 0.f), "Inferno");
            ImGui::GetBackgroundDrawList()->AddRectFilled({ pos.x - 50 - 1, pos.y + 11 }, { pos.x + 50 + 1, pos.y + 7 }, ImColor(0.f, 0.f, 0.f));
            ImGui::GetBackgroundDrawList()->AddRectFilled({ pos.x - 50, pos.y + 10 }, { (pos.x - 50) + (timer * 100), pos.y + 8 }, ImColor(1.f, 0.5f, 0.f));
        }

        if (classId == CBaseCSGrenadeProjectile)
        {

            DrawCenterText({ pos.x, pos.y + 10 }, ImColor(0.5f, 0.5f, 0.5f), "HE Grenade");
        }

        if (classId == CSmokeGrenadeProjectile)
        {
            float spawnTime = Memory::Read<float>(entity + offsets::m_flSimulationTime);

            float timer = ((spawnTime + 17.5f) - vars.curtime) / 17.5f;

            DrawCenterText({ pos.x, pos.y - 10 }, ImColor(0.5f, 0.5f, 0.5f), "Smoke");
            ImGui::GetBackgroundDrawList()->AddRectFilled({ pos.x - 50 - 1, pos.y + 11 }, { pos.x + 50 + 1, pos.y + 7 }, ImColor(0.f, 0.f, 0.f));
            ImGui::GetBackgroundDrawList()->AddRectFilled({ pos.x - 50, pos.y + 10 }, { (pos.x - 50) + (timer * 100), pos.y + 8 }, ImColor(0.5f, 0.5f, 0.5f));

        }
    }
}