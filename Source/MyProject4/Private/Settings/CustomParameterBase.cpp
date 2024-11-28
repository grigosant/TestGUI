// Fill out your copyright notice in the Description page of Project Settings.


#include "Settings/CustomParameterBase.h"

bool UCustomParameterBase::ChooseOneOfVariants_Implementation(const int SelectedNumber)
{
	SelectedIndex = SelectedNumber;
	return false;
}

void UCustomParameterBase::FillStringVariants()
{
	//найти переменную с категорией 
	for (TFieldIterator<FProperty> PropIt(this->GetClass()); PropIt; ++PropIt)
	{
		FString category = PropIt->GetMetaData(FName("Category"));
		if(category.Equals("Variants"))
		{
			//пока выводим имя переменной. потом пройтись по массиву и занести в VariantsAsStrings
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("filling %s"), *PropIt->GetName()));
			// for (TFieldIterator<FProperty> TArrayProp(PropIt->GetClass()); PropIt; ++PropIt)
			//
			// const FArrayProperty* ArrayProperty= CastField<FArrayProperty>();
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
