#pragma once

void InitStyle()
{
	ImGui::StyleColorsDark();

	auto& Style = ImGui::GetStyle();

}

void DrawMenu()
{
	ImGui::SetNextWindowSize(ImVec2(600.000f, 400.000f), ImGuiCond_Once);
	ImGui::Begin("Ketamine", NULL, 34);

	ImVec2 P1, P2;
	ImDrawList* pDrawList;
	const auto& CurrentWindowPos = ImGui::GetWindowPos();
	const auto& pWindowDrawList = ImGui::GetWindowDrawList();
	const auto& pBackgroundDrawList = ImGui::GetBackgroundDrawList();
	const auto& pForegroundDrawList = ImGui::GetForegroundDrawList();

	P1 = ImVec2(10.000f, 60.000f);
	P1.x += CurrentWindowPos.x;
	P1.y += CurrentWindowPos.y;
	P2 = ImVec2(590.000f, 60.000f);
	P2.x += CurrentWindowPos.x;
	P2.y += CurrentWindowPos.y;
	pDrawList = pWindowDrawList;
	pDrawList->AddLine(P1, P2, ImColor(1.000f, 1.000f, 1.000f, 1.000f), 1.000f);

	ImGui::SameLine();
	ImGui::Button("Aim", ImVec2(184.000f, 0.000f));

	ImGui::SameLine();
	ImGui::Button("Visuals", ImVec2(184.000f, 0.000f));

	ImGui::SameLine();
	ImGui::Button("Misc", ImVec2(184.000f, 0.000f));

	ImGui::SetCursorPos(ImVec2(8.000f, 70.000f));
	ImGui::Checkbox("Boxes", &var::boxEsp);

	ImGui::SetCursorPos(ImVec2(8.000f, 98.000f));
	ImGui::Checkbox("Corner Box", &var::cornersEsp);

	ImGui::SetCursorPos(ImVec2(8.000f, 126.000f));
	ImGui::Checkbox("Health Bar", &var::healthBarEsp);

	ImGui::SetCursorPos(ImVec2(8.000f, 154.000f));
	ImGui::Checkbox("Armor Bar", &var::armorBarEsp);

	ImGui::SetCursorPos(ImVec2(8.000f, 182.000f));
	ImGui::Checkbox("Names", &var::nameEsp);

	ImGui::SetCursorPos(ImVec2(8.000f, 210.000f));
	ImGui::Checkbox("Active Weapon", &var::activeWeaponEsp);

	ImGui::SetCursorPos(ImVec2(8.000f, 238.000f));
	ImGui::Checkbox("Entity ESP", &var::entityEsp);

	ImGui::SetCursorPos(ImVec2(8.000f, 266.000f));
	ImGui::Checkbox("Skeleton", &var::skeletonEsp);

	ImGui::SetCursorPos(ImVec2(8.000f, 294.000f));
	ImGui::Text("Skeleton and Entity Esp can cause delay");

	ImGui::SetCursorPos(ImVec2(235.000f, 70.000f));
	ImGui::Checkbox("Recoil Crosshair", &var::recoilCrossHair);

	ImGui::SetCursorPos(ImVec2(235.000f, 98.000f));
	ImGui::Checkbox("Spectator List", &var::spectatorList);

	ImGui::SetCursorPos(ImVec2(235.000f, 126.000f));
	ImGui::Text("Kinda Fucked");

	ImGui::SetCursorPos(ImVec2(435.000f, 70.000f));
	ImGui::Checkbox("Box Outline", &var::boxOutLine);

	ImGui::SetCursorPos(ImVec2(435.000f, 98.000f));
	ImGui::Checkbox("Ignore Team", &var::ignoreTeam);

	ImGui::End();
}
