// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActorParameterComponent.generated.h"

USTRUCT(BlueprintType)
struct FParameterData
{
	GENERATED_BODY()

	// Имя параметра (ключ)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Key;

	// Значение параметра
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Value;

	// Тип параметра
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Type;
};

USTRUCT(BlueprintType)
struct FParameterDataVariety
{
	GENERATED_BODY()

	// Имя параметра (ключ)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Key;

	//отображаемое имя (имя опции)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString VisibleName;

	// Значение опции
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Value;

	// Тип параметра
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Type;
};


USTRUCT(BlueprintType)
struct FTESTPlayer
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> Friends;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString PropertyToIgnore;
};

UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT4_API UActorParameterComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:

	UActorParameterComponent();

	//JSON
	// Установить параметры через JSON
	UFUNCTION(BlueprintCallable, Category = "Parameters")
	void SetParameters(const FString& NewParametersJSON);

	// Получить параметры в формате JSON
	UFUNCTION(BlueprintCallable, Category = "Parameters")
	FString GetParameters() const;
	
	// Установить отдельный параметр
	UFUNCTION(BlueprintCallable)
	bool SetParameter(const FString& Key, const FString& Value, const FString& Type);

	// Получить значение отдельного параметра
	UFUNCTION(BlueprintCallable, Category = "Parameters")
	FParameterData GetParameter(const FString& Key) const;





	//STRUCT
	// Получить параметры в формате JSON
	UFUNCTION(BlueprintCallable, Category = "Parameters")
	void GetParametersStruct(TArray<FParameterData>& StructsArray) const;
	
	UFUNCTION(BlueprintCallable, Category = "Parameters")
	bool SetParameterFromStruct(const FParameterData& ParameterData);
	
protected:
	// Сериализация всех свойств в JSONs
	FString SerializeToJson() const;

	// Десериализация JSON в свойства
	void DeserializeFromJson(const FString& ParametersJSON);

	// Проверка, является ли свойство структурой
	void SerializeStruct(FJsonObject& JsonObject, const FString& PropertyName, const void* StructValue, UStruct* StructType) const;


	//template <typename StructType>
	//requires TIsDerivedFrom<StructType, UScriptStruct>::Value
	//UFUNCTION(BlueprintCallable, Category = "Parameters")
	void DeserializeStruct(const FStructProperty* StructProperty, void* StructData, FString& JsonObject);


	//TEST
	UFUNCTION(BlueprintCallable, Category = "Parameters")
	FString TestSerialize();
	
	template <typename StructType>
	UFUNCTION(BlueprintCallable, Category = "Parameters")
	void DeserializeTest(const FString& JsonObject, StructType& StructTyp);
	
private:
	FString ParametersJSON;
};
