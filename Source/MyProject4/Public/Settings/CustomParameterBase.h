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

public:
	// Имя параметра
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter")
	FName ParameterName;

	//отображаемое имя
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter")
	FName VisibleName;
};


