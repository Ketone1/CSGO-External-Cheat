#pragma once

struct player_info_s
{
	__int64         unknown;            //0x0000 
	union
	{
		__int64       steamID64;          //0x0008 - SteamID64
		struct
		{
			__int32     xuid_low;
			__int32     xuid_high;
		};
	};
	char            szName[128];        //0x0010 - Player Name
	int             userId;             //0x0090 - Unique Server Identifier
	char            szSteamID[20];      //0x0094 - STEAM_X:Y:Z
	char            pad_0x00A8[0x10];   //0x00A8
	unsigned long   iSteamID;           //0x00B8 - SteamID 
	char            szFriendsName[128];
	bool            fakeplayer;
	bool            ishltv;
	unsigned int    customfiles[4];
	unsigned char   filesdownloaded;
};

char* GetPlayerName(int index)
{
	uintptr_t dwClientState = Memory::Read<uintptr_t>(engine + offsets::dwClientState);
	uintptr_t UserInfoTable = Memory::Read<uintptr_t>(dwClientState + offsets::dwClientState_PlayerInfo);
	uintptr_t items = Memory::Read<uintptr_t>(Memory::Read<uintptr_t>(UserInfoTable + 0x40) + 0xC);
	player_info_s info = Memory::Read<player_info_s>(Memory::Read<uintptr_t>(items + 0x28 + ((index) * 0x34)));

	return info.szName;
}