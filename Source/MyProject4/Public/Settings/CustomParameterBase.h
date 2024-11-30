// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Settings/BaseBehaviourRule.h"
#include "CustomParameterBase.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRuleChangedState);

UCLASS(Blueprintable, EditInlineNew, config=Game)
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
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CustomParameter")
	bool FillStringVariants( );

	const TArray<FString>& GetRealVariants();
	
	const TArray<FString>& GetDisplayedVariants();

	 FString GetVariantType() const;

	//преобразование массива вариантов в массив строк
	UFUNCTION(BlueprintCallable, CustomThunk,
		meta = (ArrayParm = "Array", ArrayTypeDependentParams = "First", DefaultToSelf = "OwningObject"),
		Category = "Converting")
	void VariantsConvert(const TArray<int32> &Array, int32 &First, bool& Result, const UObject* OwningObject);

	//получить все имена правил
	UFUNCTION(BlueprintCallable, Category = "CustomParameter")
	void GetAllBehavioursNames(TArray<FName>& Names);

	UBaseBehaviourRule* GetBehaviourByName(const FName& DesiredNames);
	
	UFUNCTION(BlueprintCallable, Category = "CustomParameter")
	void ExecuteBehaviourWithName(const FName BehName);

	UFUNCTION(BlueprintCallable, Category = "CustomParameter")
	void ExecuteBehaviourWithNameOneParam(const FName BehName, UObject* ObjectToProceed);

	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "CustomParameter")
	inline int GetCurrentIndex() {return SelectedIndex;}
private:
	DECLARE_FUNCTION(execVariantsConvert)
	{
		Stack.MostRecentPropertyAddress = nullptr;
		Stack.MostRecentProperty = nullptr;

		//Array
		Stack.StepCompiledIn<FArrayProperty>(nullptr);
		const void *Array  = Stack.MostRecentPropertyAddress;
		const FArrayProperty *ArrayProperty = CastField<FArrayProperty>(Stack.MostRecentProperty);
		if (!ArrayProperty)
		{
			Stack.bArrayContextFailed = true;
			return;
		}

		Stack.MostRecentProperty = nullptr;
		Stack.StepCompiledIn<FProperty>(nullptr);
		void *Head = Stack.MostRecentPropertyAddress;
		const auto *HeadProperty = CastField<FProperty>(Stack.MostRecentProperty);
		if (!HeadProperty) {
			Stack.bArrayContextFailed = true;
			return;
		}

		//Success parameter
		Stack.StepCompiledIn<FBoolProperty>(NULL);
		void* SuccessBoolPtr = Stack.MostRecentPropertyAddress;
		if (!SuccessBoolPtr)
		{
			return;
		}
		auto SuccessBoolProp = (FBoolProperty*)(Stack.MostRecentProperty);

		P_GET_PROPERTY(FObjectProperty, OwningObject);
		
		P_FINISH

		P_NATIVE_BEGIN
			bool Result = genericVariantsConvert
				(Array, ArrayProperty, Head, HeadProperty, OwningObject);
		
			SuccessBoolProp->SetPropertyValue(SuccessBoolPtr, Result);
		P_NATIVE_END
	  }

	static bool genericVariantsConvert(
	const void *Array, const FArrayProperty *ArrayProperty,
	void *Head,        const FProperty *HeadProperty,
	UObject* OwningObject) ;
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rule", meta = (DisplayAfter="bUseDisplayName", EditCondition="bUseDisplayName", DisplayPriority = 3))
	TArray<FString> VariantsDisplayNames;

	//Набор поведений
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Instanced, Category = "Rule", meta = (DisplayPriority = 4))
	TArray<TObjectPtr<UBaseBehaviourRule>> BehaviourRules;


	UPROPERTY(BlueprintAssignable)
	FRuleChangedState OnChangeState;
	
	
private:
	//значения вариантов, переведенные в строки
	UPROPERTY(VisibleInstanceOnly)
	TArray<FString> VariantsAsStrings;

	//текущий выбранный вариант из списка
	int SelectedIndex = 0;

	//тип содержимого массива
	FString ContainerTypeText;
};


