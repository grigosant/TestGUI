#pragma once

#include "CoreMinimal.h"
#include "SettingsBaseStructs.generated.h"


USTRUCT(BlueprintType)
struct FSettingParameterData
{
	GENERATED_BODY()

	// Имя переменной (ключ)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Key;
	
	// Возможные варианты в виде строк
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> VariableValues;

	// Тип переменной
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Type;
};

USTRUCT(BlueprintType)
struct FSettingParameter
{
	GENERATED_BODY()

	// Имя параметра (ключ)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString RuleName;

	// Имя параметра (ключ)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString VisibleRuleName;

	// все переменные в виде массива структур
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSettingParameterData> RuleValues;
};


USTRUCT(BlueprintType, Blueprintable)
struct FULevelSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UWorld> Level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Texture;
};
