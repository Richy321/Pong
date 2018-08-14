#pragma once

UENUM(BlueprintType)
enum class EAIDifficulty : uint8
{
	Easy,
	Normal,
	Hard
};

FString static AIDifficultyEnumToString(EAIDifficulty Difficulty)
{
	switch (Difficulty)
	{
	case EAIDifficulty::Easy:
		return TEXT("Easy");
	case EAIDifficulty::Normal:
		return TEXT("Normal");
	case EAIDifficulty::Hard:
		return TEXT("Hard");
	default:
		return TEXT("Error");
	}
}