// Fill out your copyright notice in the Description page of Project Settings.


#include "Settings/SettingsComponent.h"
#include "Settings/CustomParameterBase.h"

// Sets default values for this component's properties
USettingsComponent::USettingsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void USettingsComponent::BeginPlay()
{
	Super::BeginPlay();
	
	for (auto SingleParameter : ConfigurableParameters)
	{
		SingleParameter->FillStringVariants();
	}
}

// Called every frame
void USettingsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


UCustomParameterBase* USettingsComponent::GetParameterByName(FName ParameterName) const
{
	for (auto SingleParameter : ConfigurableParameters)
	{
		if (SingleParameter->ParameterName == ParameterName)
		{
			return SingleParameter;
		}
	}
	return nullptr;
}

class UCustomParameterBase* USettingsComponent::GetParameterByClassAndName(
	TSubclassOf<UCustomParameterBase> ParameterClass, FName ParameterName) const
{
	const bool shouldChecName = !(ParameterName.IsNone()); 
	for (auto SingleParameter : ConfigurableParameters)
	{
		if (SingleParameter->IsA(ParameterClass) )
		{
			if(!shouldChecName)
				return SingleParameter;
			else if(SingleParameter->ParameterName == ParameterName)
				return SingleParameter;
		}
	}
	return nullptr;
}

void USettingsComponent::GetParametersStruct(TArray<FSettingRule>& StructsArray) const
{
	StructsArray.Empty();

	for (auto SingleParameter : ConfigurableParameters)
	{
		FSettingRule LocalRuleParameter;
		const UCustomParameterBase *tmpParameter = SingleParameter;

		//занести имя и видимое имя правила
		LocalRuleParameter.RuleName = SingleParameter->ParameterName.ToString();
		LocalRuleParameter.VisibleRuleName = SingleParameter->VisibleName.ToString();
		LocalRuleParameter.bUseDisplayName = SingleParameter->bUseDisplayName;
		

		FSettingParameterData* LocalVariableOfRule = &LocalRuleParameter.RuleVariants;
		
		const FProperty* Property = SingleParameter->GetClass()->FindPropertyByName("VariantsAsStrings");
		if(Property == nullptr)
			return;
		LocalVariableOfRule->Key = Property->GetName();
		LocalVariableOfRule->Type = SingleParameter->GetVariantType();

		TArray<FString> RealVariantsArray = SingleParameter->GetRealVariants();
		TArray<FString> DisplayVariantsArray = SingleParameter->GetDisplayedVariants();
		for(int i = 0; i < RealVariantsArray.Num(); i++)
		{
			FSettingStringValue SettingStringValue;
			SettingStringValue.RealValue = RealVariantsArray[i];
			SettingStringValue.DisplayValue = i < DisplayVariantsArray.Num() ? DisplayVariantsArray[i] : FString("");
			LocalVariableOfRule->VariableValues.Add(SettingStringValue);
		}
		
		
		/*for (TFieldIterator<FProperty> PropIt(tmpParameter->GetClass()); PropIt; ++PropIt)
		{
			FSettingParameterData LocalVariableOfRule;
			const FProperty* Property = *PropIt;

			
			LocalVariableOfRule.Type = Property-> GetCPPType();
			//Убрать из переменных граф
			if (LocalVariableOfRule.Type == "FPointerToUberGraphFrame")
				continue;
			
			LocalVariableOfRule.Key = Property->GetName();
			
			//имя и отображаемое имя уже учтены
			if(LocalVariableOfRule.Key == "ParameterName" || LocalVariableOfRule.Key == "VisibleName")
				continue;
			
			//варианты массива должны помещаться по одному
			else if (LocalVariableOfRule.Key == "VariantsAsStrings")
			{
				
				LocalVariableOfRule.VariableValues = SingleParameter->GetStringVariants();
			}

			//остальные переменные - пока просто добавлять
			else
			{
				FString VariableAsString;
				if(!Property->ExportText_InContainer(0, VariableAsString, (const void *)tmpParameter, this,this->GetOwner() , PPF_None))
					continue;
				LocalVariableOfRule.VariableValues.Add(VariableAsString);
			}
			
			LocalRuleParameter.RuleValues.Add(LocalVariableOfRule);
		}*/
		StructsArray.Add(LocalRuleParameter);
	}
}

bool USettingsComponent::SetParameterFromStruct(const FSettingRule& ParameterData, const FName& ParameterName)
{
	UCustomParameterBase* changingParameter = GetParameterByName(ParameterName);
	if(changingParameter == nullptr)
		return false;

	FSettingParameterData SingleRule = ParameterData.RuleVariants;
	const FProperty* Property = changingParameter->GetClass()->FindPropertyByName(FName(SingleRule.Key));
	if(Property == nullptr)
		return false;

	for (auto SingleString : SingleRule.VariableValues)
	{
		
	}
		
		//Property->ImportText_InContainer((SingleRule.VariableValues), changingParameter, changingParameter, PPF_None )
	
	return true;
}

bool USettingsComponent::SelectParameterForName(const FName& ParameterName, int VariantNumber)
{
	UCustomParameterBase* changingParameter = GetParameterByName(ParameterName);
	if(changingParameter == nullptr)
		return false;

	return changingParameter->ChooseOneOfVariants(VariantNumber);
}

void USettingsComponent::GetAllRulesNames(TArray<FString>& Names) const
{
	for (auto SingleParameter : ConfigurableParameters)
	{
		Names.Add(SingleParameter->ParameterName.ToString());
	}
}

UCustomParameterBase* USettingsComponent::FindRuleWithBehaviour(const FName& BehaviourName)
{
	for (auto SingleParameter : ConfigurableParameters)
	{
		TArray<FName> ParameterBehaviourNames;
		SingleParameter->GetAllBehavioursNames(ParameterBehaviourNames);
		if(ParameterBehaviourNames.Contains(BehaviourName))
			return SingleParameter;
	}
	return nullptr;
}
