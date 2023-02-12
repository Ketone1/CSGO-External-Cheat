#pragma once

struct mstudiobbox_t
{
    int					bone;
    int					group;				// intersection group
    Vector				bbmin;				// bounding box
    Vector				bbmax;
    int					szhitboxnameindex;	// offset to the name of the hitbox.
    int					unused[8];
};

struct mstudiohitboxset_t
{
	int					sznameindex;
	int					numhitboxes;
	int					hitboxindex;
};

enum EHitbox : std::size_t
{
    Hitbox_Head,
    Hitbox_Neck,
    Hitbox_Pelvis,
    Hitbox_Stomach,
    Hitbox_Chest,
    Hitbox_Lower_Chest,
    Hitbox_Upper_Chest,
    Hitbox_Right_Thigh,
    Hitbox_Left_Thigh,
    Hitbox_Right_Calf,
    Hitbox_Left_Calf,
    Hitbox_Right_Foot,
    Hitbox_Left_Foot,
    Hitbox_Right_Hand,
    Hitbox_Left_Hand,
    Hitbox_Right_Upper_Arm,
    Hitbox_Right_Lower_Arm,
    Hitbox_Left_Upper_Arm,
    Hitbox_Left_Lower_Arm,
    Hitbox_Last
};