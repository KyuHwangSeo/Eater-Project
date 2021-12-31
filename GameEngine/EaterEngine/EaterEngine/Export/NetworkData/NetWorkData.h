#pragma once
struct Vec3
{
	float x;
	float y;
	float z;
};
struct PlayerData
{
	short PLAYERTYPE;
	bool isPlayer;

	Vec3 Position;
	Vec3 Direction;
	float Speed;
};




