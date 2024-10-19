// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateEnums.generated.h"

UENUM()
enum class EBoardState : uint8
{
	NONE,
	WHITE,
	BLACK
};

UENUM()
enum class EDisplayState : uint8
{
	INTRO,
	PLAYING,
	GAMEOVER
};

UENUM()
enum class EPlayState : uint8
{
	STARTING,
	THINKING,
	DECISION,
	WAIT,
	TURNING,
	END
};

USTRUCT()
struct FPlayerStatus
{
	GENERATED_BODY()
public:
	UPROPERTY()
	int64 Score;

	UPROPERTY()
	bool CanSetUp;

	UPROPERTY()
	bool Turn;
};

USTRUCT()
struct FNumberTexture
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere)
	class UObject* TextureZero;

	UPROPERTY(EditAnywhere)
	class UObject* TextureOne;

	UPROPERTY(EditAnywhere)
	class UObject* TextureTwo;

	UPROPERTY(EditAnywhere)
	class UObject* TextureThree;

	UPROPERTY(EditAnywhere)
	class UObject* TextureFour;

	UPROPERTY(EditAnywhere)
	class UObject* TextureFive;

	UPROPERTY(EditAnywhere)
	class UObject* TextureSix;

	UPROPERTY(EditAnywhere)
	class UObject* TextureSeven;

	UPROPERTY(EditAnywhere)
	class UObject* TextureEight;

	UPROPERTY(EditAnywhere)
	class UObject* TextureNine;
};

USTRUCT()
struct FBoradRowArray
{
	GENERATED_BODY()
public:
	TArray<class UBoardPart*> RowArray;
};

USTRUCT()
struct FPos
{
	GENERATED_BODY()
public:
	FPos(int64 NewX, int64 NewY) { PosX = NewX; PosY = NewY; }
	FPos() { PosX = 0; PosY = 0; }
	bool operator==(const FPos& other) const {
		return PosX == other.PosX && PosY == other.PosY;
	}
	FPos operator+(FPos& other)  {
		return (FPos(PosX + other.PosX, PosY + other.PosY));
	}

public:
	int64 PosX;
	int64 PosY;
};

#if UE_BUILD_DEBUG
uint32 GetTypeHash(const FPos& Thing);
#else // optimize by inlining in shipping and development builds
FORCEINLINE uint32 GetTypeHash(const FPos& Thing)
{
	uint32 Hash = FCrc::MemCrc32(&Thing, sizeof(FPos));
	return Hash;
}
#endif