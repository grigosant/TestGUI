// Fill out your copyright notice in the Description page of Project Settings.

//КЛАСС КАК ОБРАЗЕЦ, НЕ ИСПОЛЬЗУЕТСЯ

#include "Settings/ActorParameterComponent.h"

#include "JsonObjectConverter.h"

// Sets default values for this component's properties
UActorParameterComponent::UActorParameterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
    ParametersJSON = TEXT("{}"); // Пустой JSON
}

void UActorParameterComponent::SetParameters(const FString& NewParametersJSON)
{
	DeserializeFromJson(ParametersJSON);
}

FString UActorParameterComponent::GetParameters() const
{
	return SerializeToJson();
}

bool UActorParameterComponent::SetParameter(const FString& Key, const FString& Value, const FString& Type)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ParametersJSON);

	if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
	{
		// Создаем вложенный объект для хранения значений и типа
		TSharedPtr<FJsonObject> ParameterObject = MakeShareable(new FJsonObject);
		ParameterObject->SetStringField(TEXT("Value"), Value);
		ParameterObject->SetStringField(TEXT("Type"), Type);

		// Сохраняем вложенный объект по ключу
		JsonObject->SetObjectField(Key, ParameterObject);

		// Сериализуем обратно в JSON
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&ParametersJSON);
		return FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
	}
	return false;
}

FParameterData UActorParameterComponent::GetParameter(const FString& Key) const
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ParametersJSON);
	FParameterData Result;

	if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
	{
		if (JsonObject->HasTypedField<EJson::Object>(Key))
		{
			TSharedPtr<FJsonObject> ParameterObject = JsonObject->GetObjectField(Key);
			Result.Key = Key;
			Result.Value = ParameterObject->GetStringField(TEXT("Value"));
			Result.Type = ParameterObject->GetStringField(TEXT("Type"));
		}
	}

	return Result;
}

FString UActorParameterComponent::SerializeToJson() const
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

	for (TFieldIterator<FProperty> PropIt(GetClass()); PropIt; ++PropIt)
	{
		const FProperty* Property = *PropIt;
		const void* ValuePtr = Property->ContainerPtrToValuePtr<void>(this);

		if (const FNumericProperty* NumericProperty = CastField<FNumericProperty>(Property))
		{
			if (NumericProperty->IsInteger())
			{
				JsonObject->SetNumberField(Property->GetName(), NumericProperty->GetSignedIntPropertyValue(ValuePtr));
			}
			else
			{
				JsonObject->SetNumberField(Property->GetName(), NumericProperty->GetFloatingPointPropertyValue(ValuePtr));
			}
		}
		else if (const FStrProperty* StringProperty = CastField<FStrProperty>(Property))
		{
			JsonObject->SetStringField(Property->GetName(), StringProperty->GetPropertyValue(ValuePtr));
		}
		else if (const FStructProperty* StructProperty = CastField<FStructProperty>(Property))
		{
			SerializeStruct(*JsonObject, Property->GetName(), ValuePtr, StructProperty->Struct);
		}
	}

	FString SerializedJson;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&SerializedJson);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	return SerializedJson;
}

void UActorParameterComponent::DeserializeFromJson(const FString& ParametersJSONlocal)
{
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ParametersJSON);

	if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
	{
		for (TFieldIterator<FProperty> PropIt(GetClass()); PropIt; ++PropIt)
		{
			FProperty* Property = *PropIt;
			void* ValuePtr = Property->ContainerPtrToValuePtr<void>(this);

			if (const FNumericProperty* NumericProperty = CastField<FNumericProperty>(Property))
			{
				if (JsonObject->HasField(Property->GetName()))
				{
					if (NumericProperty->IsInteger())
					{
						NumericProperty->SetIntPropertyValue(ValuePtr, (uint64)JsonObject->GetIntegerField(Property->GetName()));
					}
					else
					{
						NumericProperty->SetFloatingPointPropertyValue(ValuePtr, JsonObject->GetNumberField(Property->GetName()));
					}
				}
			}
			else if (const FStrProperty* StringProperty = CastField<FStrProperty>(Property))
			{
				if (JsonObject->HasField(Property->GetName()))
				{
					StringProperty->SetPropertyValue(ValuePtr, JsonObject->GetStringField(Property->GetName()));
				}
			}
			else if (const FStructProperty* StructProperty = CastField<FStructProperty>(Property))
			{
				if (JsonObject->HasField(Property->GetName()))
				{
					void* StructData = StructProperty->ContainerPtrToValuePtr<void>(this);

					FString JSONPayload = ParametersJSONlocal;
				
					//FJsonObjectConverter::UStructToJsonObjectString(StructProperty->Struct, StructData, JSONPayload, 0, 0);
					//FJsonObjectConverter::JsonObjectStringToUStruct(JSONPayload, StructProperty);
					DeserializeStruct(StructProperty, StructData, JSONPayload);
				}
			}
		}
	}
}

void UActorParameterComponent::SerializeStruct(FJsonObject& JsonObject, const FString& PropertyName,
	const void* StructValue, UStruct* StructType) const
{
	FString StructJson;
	if (FJsonObjectConverter::UStructToJsonObjectString(StructType, StructValue, StructJson, 0, 0))
	{
		JsonObject.SetStringField(PropertyName, StructJson);
	}
}

FString UActorParameterComponent::TestSerialize()
{
	FTESTPlayer Player;
	Player.Name = "Frank";
	Player.Level = 3;
	Player.Friends.Add("Jeff");
	Player.PropertyToIgnore = "Gotta Go Fast";

	FString JSONPayload;
	FJsonObjectConverter::UStructToJsonObjectString(Player, JSONPayload, 0, 0);
	return JSONPayload;
}

// template <typename StructType>
// void UActorParameterComponent::DeserializeStruct(const FString& PropertyName, const TSharedPtr<FJsonObject>& JsonObject, const FString& JsonString, const StructType &StructTyp)
// {
// 	if (JsonObject->HasField(PropertyName))
// 	{
// 		FString StructJson = JsonObject->GetStringField(PropertyName);
// 		if (!FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &StructTyp, 0, 0))
// 		{
// 			UE_LOG(LogTemp, Error, TEXT("Failed to deserialize struct for property: %s"), *PropertyName);
// 		}
// 	}
// }

//requires TIsDerivedFrom<StructType, UScriptStruct>::Value
void UActorParameterComponent::DeserializeStruct(const FStructProperty* StructProperty, void* StructData, FString& JsonObject)
{
		//FString StructJson = JsonObject->GetStringField(PropertyName);
		//if (!FJsonObjectConverter::JsonObjectStringToUStruct(StructJson, &OutStruct, 0, 0))

		if(!FJsonObjectConverter::UStructToJsonObjectString(StructProperty->Struct, StructData, JsonObject, 0, 0))
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to deserialize struct for property: ..."));
		}
}

template <typename StructType>
void UActorParameterComponent::DeserializeTest(const FString& JsonObject, StructType& StructTyp)
{
	FTESTPlayer Player;
	//FJsonObjectConverter::JsonObjectStringToUStruct(JsonObject, &StructTyp, 0, 0);
}





void UActorParameterComponent::GetParametersStruct(TArray<FParameterData>& StructsArray) const
{
	StructsArray.Empty();

	FParameterData LocalParameter;
	for (TFieldIterator<FProperty> PropIt(GetClass()); PropIt; ++PropIt)
	{
		const FProperty* Property = *PropIt;

		// Получить класс, в котором объявлено свойство
		if(Property->GetOwnerClass() != this->GetClass())
			continue;

		LocalParameter.Type = Property-> GetCPPType();
		//Убрать из переменных граф
		if (LocalParameter.Type == "FPointerToUberGraphFrame")
			continue;
		
		LocalParameter.Key = Property->GetName();

		if(!Property->ExportText_InContainer(0, LocalParameter.Value, this, this, this->GetOwner(), PPF_None))
			continue;
		
		StructsArray.Add(LocalParameter);
		LocalParameter.Value = "";
	}
}

bool UActorParameterComponent::SetParameterFromStruct(const FParameterData& ParameterData)
{
	const FProperty* Property = this->GetClass()->FindPropertyByName(FName(ParameterData.Key));
	if(Property == nullptr)
		return false;

	Property->ImportText_InContainer(*(ParameterData.Value), this, this, PPF_None );
	
	return true;
}