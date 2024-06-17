#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DungeonStrategy.generated.h"
class ADungeonGenerator;

enum class EDungeonType : char {
	None		= ' ',
	Wall		= 'W',
	Floor		= '#',
	Corridor	= '.',
	Door		= 'D',
	Treasure	= 'T',
	Portal		= 'P',
};

UINTERFACE(MinimalAPI, Blueprintable)
class UGenerateStrategy : public UInterface
{
	GENERATED_BODY()
};

class IGenerateStrategy
{
	GENERATED_BODY()
public:
	virtual TArray<EDungeonType> DoGenerate(ADungeonGenerator& InGenerator) = 0;
};

class ILinearStrategy : public IGenerateStrategy
{
public:
	virtual TArray<EDungeonType> DoGenerate(ADungeonGenerator& InGenerator) override;
};

class ITinyKeepStrategy : public IGenerateStrategy
{
public:
	virtual TArray<EDungeonType> DoGenerate(ADungeonGenerator& InGenerator) override;
};