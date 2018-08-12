#pragma once

#include "SidesEnum.h"
#include "GameScore.generated.h"

USTRUCT(BlueprintType)
struct PONG_API FGameScore
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	int ScoreLeft;
	UPROPERTY(BlueprintReadOnly)
	int ScoreRight;
};