#pragma once

UENUM(BlueprintType)
enum class EDirection : uint8
{
	Forward, Right, Back, Left, Up, Down
};

UENUM(BlueprintType)
enum class EBlock : uint8
{
	Null, 
	Air, 
	Stone, 
	Dirt, 
	Water,
	Grass,
	Oak,
	OakLeaves,
	Coal,
	Iron,
	Lapis,
	Gold,
	Diamond,
	Redstone,
	Brown_Concrete,
	Purple_Concrete,
	Gray_Concrete,
	Cyan_Concrete,
	Green_Concrete,
	Lime_Concrete,
	Orange_Concrete,
	Magenta_Concrete,
	Light_Blue_Concrete,
	Pink_Concrete,
	Blue_Concrete,
	Light_Gray_Concrete,
	White_Concrete,
	Black_Concrete,
	Yellow_Concrete,
	Red_Concrete,
};

UENUM(BlueprintType)
enum class EGenerationType : uint8
{
	GT_3D UMETA(DisplayName = "3D"),
	GT_2D UMETA(DisplayName = "2D"),
};