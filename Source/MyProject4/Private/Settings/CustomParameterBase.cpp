// Fill out your copyright notice in the Description page of Project Settings.


#include "Settings/CustomParameterBase.h"

bool UCustomParameterBase::ChooseOneOfVariants_Implementation(const int SelectedNumber)
{
	SelectedIndex = SelectedNumber;
	return false;
}

void UCustomParameterBase::FillStringVariants()
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
				
				return;
			}
			//пока выводим имя переменной. потом пройтись по массиву и занести в VariantsAsStrings
			
		}
			

	}
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
