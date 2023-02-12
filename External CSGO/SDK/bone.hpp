#pragma once

Vector GetBonePos(uintptr_t entity, uintptr_t BoneMatrix, int index)
{

	return Vector{ Memory::Read<float>(BoneMatrix + 0x30 * 8 + 0x0C), Memory::Read<float>(BoneMatrix + 0x30 * 8 + 0x1C), Memory::Read<float>(BoneMatrix + 0x30 * 8 + 0x2C) };
}