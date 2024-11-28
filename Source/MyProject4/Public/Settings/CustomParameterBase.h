// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomParameterBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, abstract, EditInlineNew, config=Game)
class MYPROJECT4_API UCustomParameterBase : public UObject
{
	GENERATED_BODY()

public:
	FName GetParameterName() const { return ParameterName; }
		
	//Выбор и назначение одного из вариантов (с индексом SelectedNumber) как текущего
	//должна быть переопределена в классах, добавляющих варианты
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CustomParameter")
	bool ChooseOneOfVariants(const int SelectedNumber );

	//сгенерировать массив строк из выбранных параметров
	//выполняется пристарте игры
	UFUNCTION(BlueprintCallable, Category = "CustomParameter")
	void FillStringVariants( );

	const TArray<FString>& GetRealVariants();
	
	const TArray<FString>& GetDisplayedVariants();

	 FString GetVariantType() const;

public:
	// Имя правила
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rule", meta = (DisplayPriority = 0))
	FName ParameterName;

	//отображаемое имя правила
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rule", meta = (DisplayPriority = 1))
	FName VisibleName;
	
	//правило использует реальные значения или псевдонимы?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rule", meta = (DisplayPriority = 2))
	bool bUseDisplayName;
	
	//псевдонимы для вариантов
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rule", meta = (DisplayAfter="bUseDisplayName", EditCondition="bUseDisplayName"))
	TArray<FString> VariantsDisplayNames;

	
	//значения вариантов, переведенные в строки
	//скрытое свойство, пока видимо для отладки
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rule", meta = (DisplayAfter="bUseDisplayName"))
	TArray<FString> VariantsAsStrings;
private:
	//текущий выбранный вариант из списка
	int SelectedIndex = 0;

	//тип содержимого массива
	UPROPERTY()
	FString ContainerTypeText;
};


