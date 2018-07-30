#pragma once

UENUM(BlueprintType)
enum class EGameType : uint8
{
	OneVsOne,
	TwoVsTwo
};

UENUM(BlueprintType)
enum class EMultiplayerGameType : uint8
{
	SinglePlayer,
	LocalMultiplayer,
	OnlineMultiplayer
};