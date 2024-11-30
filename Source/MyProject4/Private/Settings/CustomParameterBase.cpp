// Fill out your copyright notice in the Description page of Project Settings.


#include "Settings/CustomParameterBase.h"

bool UCustomParameterBase::ChooseOneOfVariants_Implementation(const int SelectedNumber)
{
	SelectedIndex = SelectedNumber;
	OnChangeState.Broadcast();
	return false;
}

bool UCustomParameterBase::FillStringVariants_Implementation()
{
	//имя неизвестно, поэтому просто найти не получится, надо перебирать
	// FArrayProperty* ArrayProperty = FindFProperty<FArrayProperty>(this->GetClass(), )
	
	for (TFieldIterator<FProperty> PropIt(this->GetClass()); PropIt; ++PropIt)
	{
		FString category = PropIt->GetMetaData(FName("Category"));
		//сохраняем только массивы из категории
		if(category.Equals("Variants"))
		{
			FArrayProperty* ArrayProperty= CastField<FArrayProperty>(*PropIt);
			if(ArrayProperty)
			{
				FString PropertyType = ArrayProperty->GetCPPType(&ContainerTypeText,  0);
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("filling %s,  %s "), *PropIt->GetName(), *ContainerTypeText));

				FString VariableAsString;
				ArrayProperty->ExportText_InContainer(0, VariableAsString, (const void *)this, this,this , PPF_None);
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("filling %s"), *VariableAsString));

				//VariantsAsStrings.Empty();
				//попытка пройтись по всем элементам
				auto ArrayValue = ArrayProperty->GetPropertyValuePtr(ArrayProperty->ContainerPtrToValuePtr<void>(this));
				// for(auto InnerArrayElement : ArrayValue)
				// {
				// 	VariantsAsStrings.Add("param");
				// }
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%s contain %d"), *PropIt->GetName(), ArrayValue->Num()));
				
				return true;
			}
			//пока выводим имя переменной. потом пройтись по массиву и занести в VariantsAsStrings
			
		}
			
		
	}
	return false;
}

const TArray<FString>& UCustomParameterBase::GetRealVariants()
{
	return VariantsAsStrings;
}

const TArray<FString>& UCustomParameterBase::GetDisplayedVariants()
{
	return VariantsDisplayNames;
}

FString UCustomParameterBase::GetVariantType() const
{
	return ContainerTypeText;
}

void UCustomParameterBase::GetAllBehavioursNames(TArray<FName>& Names)
{
	for (auto SingleBehaviour : BehaviourRules)
	{
		Names.Add(SingleBehaviour->BehaviourName);
	}
}

UBaseBehaviourRule* UCustomParameterBase::GetBehaviourByName(const FName& DesiredNames)
{
	for (auto SingleBehaviour : BehaviourRules)
	{
		if(SingleBehaviour->BehaviourName == DesiredNames)
			return SingleBehaviour;
	}
	return nullptr;
}

void UCustomParameterBase::ExecuteBehaviourWithName(const FName BehName)
{
	GetBehaviourByName(BehName)->StartBehavior();		
}

void UCustomParameterBase::ExecuteBehaviourWithNameOneParam(const FName BehName, UObject* ObjectToProceed)
{
	GetBehaviourByName(BehName)->StartBehaviorParam(ObjectToProceed);
}

bool UCustomParameterBase::genericVariantsConvert(const void* Array, const FArrayProperty* ArrayProperty, void* Head,
                                                  const FProperty* HeadProperty, UObject* OwningObject)
{
	FScriptArrayHelper ArrayHelper(ArrayProperty, Array);
	if (ArrayHelper.Num() == 0)
		return false;
	else
	{
		//void*	DiffArr = ArrayProperty->ContainerPtrToValuePtrForDefaults<void>()
		FProperty * InnerProp = ArrayProperty->Inner;
		HeadProperty->InitializeValue(Head);
		HeadProperty->CopyCompleteValue(Head, ArrayHelper.GetElementPtr(0));

		UCustomParameterBase* ParameterBase = static_cast<UCustomParameterBase*>(OwningObject);
		ParameterBase->VariantsAsStrings.Empty();
		ParameterBase->VariantsAsStrings.SetNum(ArrayHelper.Num());
		for(int32 DynamicArrayIndex = 0; DynamicArrayIndex < ArrayHelper.Num(); DynamicArrayIndex++)
		{
			FString variantAsString;
			//ArrayProperty->ExportTextItem_InContainer(variantAsString, HeadProperty, nullptr, nullptr, 0, nullptr);

			//const void *Value = ArrayProperty->ContainerPtrToValuePtr<void>(Array, i);
			//ArrayProperty->ExportTextItem_InContainer(variantAsString, Value, nullptr, nullptr, 0);

			uint8* SourceData = ArrayHelper.GetRawPtr(DynamicArrayIndex);
			InnerProp->ExportTextItem(variantAsString, SourceData, nullptr, nullptr, 0);
			
			ParameterBase->VariantsAsStrings[DynamicArrayIndex] = variantAsString;
		}
		return true;
	}
}
