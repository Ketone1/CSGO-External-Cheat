#pragma once

struct CGlobalVars 
{
	float		realtime;                     // 0x0000
	int			simulationTicks;              // 0x0004
	float		absoluteframetime;            // 0x0008
	float		flFootYaw;                    // 0x000C
	float		curtime;                      // 0x0010
	float		frametime;                    // 0x0014
	int			maxClients;                   // 0x0018
	int			tickbaseShift;                // 0x001C
	float		interval_per_tick;            // 0x0020
	float		testicularSpacingDelta;       // 0x0024
	int			resolverMode;                 // 0x0028
	int			network_protocol;             // 0x002C
	void*		gaySexPtr;                    // 0x0030
	bool		bIsGay;                       // 0x0031
	bool		bIsVacBanned;                 // 0x0032
};