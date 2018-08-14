#pragma once

UENUM(BlueprintType)
enum class EGameType : uint8
{
	OneVsOne
};

UENUM(BlueprintType)
enum class EMultiplayerGameType : uint8
{
	SinglePlayer,
	Multiplayer
};