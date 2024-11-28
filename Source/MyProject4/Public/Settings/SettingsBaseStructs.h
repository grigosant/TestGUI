#pragma once

#include "CoreMinimal.h"
#include "SettingsBaseStructs.generated.h"

USTRUCT(BlueprintType)
struct FSettingStringValue
{
	GENERATED_BODY()

	// Имя переменной (ключ)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString RealValue;
	
	// Возможные варианты в виде строк
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DisplayValue;
};

USTRUCT(BlueprintType)
struct FSettingParameterData
{
	GENERATED_BODY()

	// Имя переменной (ключ)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Key;
	
	// Возможные варианты в виде строк
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSettingStringValue> VariableValues;

	// Тип переменной
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Type;
};

USTRUCT(BlueprintType)
struct FSettingRule
{
	GENERATED_BODY()

	// Имя параметра (ключ)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString RuleName;

    // Имя параметра (ключ)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString VisibleRuleName;

	//правило использует реальные значения или псевдонимы?
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseDisplayName;

	// все переменные в виде массива структур
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSettingParameterData RuleVariants;
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
