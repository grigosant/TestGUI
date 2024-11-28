// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "SettingsBaseStructs.h"

#include "SettingsComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT4_API USettingsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USettingsComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	// Получение параметра с именем
	UFUNCTION(BlueprintCallable, Category = "Parameters")
	UCustomParameterBase* GetParameterByName(FName ParameterName) const;

	//возвращает все правила в виде массива структур FSettingParameter,
	//содержащих описание все переменных строками
	UFUNCTION(BlueprintCallable, Category = "Parameters")
	void GetParametersStruct(TArray<FSettingRule>& StructsArray) const;

	//НЕ РАБОТАЕТ СЕЙЧАС
	//установить значение правила с именем ParameterName
	//обновляет только переменные
	UFUNCTION(BlueprintCallable, Category = "Parameters")
	bool SetParameterFromStruct(const FSettingRule& ParameterData, const FName& ParameterName);

	//установить для параметра один из шаблонов
	//в кастомных объектах функция ChooseOneOfVariants должна быть перегружена 
	UFUNCTION(BlueprintCallable, Category = "Parameters")
	bool SelectParameterForName(const FName& ParameterName, int VariantNumber);

	UFUNCTION(BlueprintCallable, Category = "Parameters")
	void GetAllRulesNames(TArray<FString>& Names) const;
	
public:
	// Массив параметров
	UPROPERTY( EditDefaultsOnly,BlueprintReadWrite, Instanced, Category = "Parameters")
	TArray<TObjectPtr<UCustomParameterBase>> ConfigurableParameters;
};
