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

void USettingsComponent::GetParametersStruct(TArray<FSettingParameter>& StructsArray) const
{
	StructsArray.Empty();

	for (auto SingleParameter : ConfigurableParameters)
	{
		FSettingParameter LocalParameter;
		const UCustomParameterBase *tmpParameter = SingleParameter;

		LocalParameter.RuleName = SingleParameter->ParameterName.ToString();
		for (TFieldIterator<FProperty> PropIt(tmpParameter->GetClass()); PropIt; ++PropIt)
		{
			FSettingParameterData LocalParameterData;
			const FProperty* Property = *PropIt;
			
			LocalParameterData.Type = Property-> GetCPPType();
			//Убрать из переменных граф
			if (LocalParameterData.Type == "FPointerToUberGraphFrame")
				continue;
			//переменные из массива должны помещаться по одной
			/*else if (LocalParameterData.Type == "TArray")
			{
				CastField<FArrayProperty>(Property);
				decltype(Property->)
			}*/
			else
			{
				LocalParameterData.Key = Property->GetName();
				//
				FString VariableAsString;
				if(!Property->ExportText_InContainer(0, VariableAsString, (const void *)tmpParameter, this,this->GetOwner() , PPF_None))
					continue;
				LocalParameterData.VariableValues = (VariableAsString);
			}
			
			
			
			LocalParameter.RuleValues.Add(LocalParameterData);
		}
		StructsArray.Add(LocalParameter);
	}
}

bool USettingsComponent::SetParameterFromStruct(const FSettingParameter& ParameterData, const FName& ParameterName)
{
	UCustomParameterBase* changingParameter = GetParameterByName(ParameterName);
	if(changingParameter == nullptr)
		return false;

	for (auto SingleRule : ParameterData.RuleValues)
	{
		const FProperty* Property = changingParameter->GetClass()->FindPropertyByName(FName(SingleRule.Key));
		if(Property == nullptr)
			continue;

		Property->ImportText_InContainer(*(SingleRule.VariableValues), changingParameter, changingParameter, PPF_None );
	}
	
	
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
