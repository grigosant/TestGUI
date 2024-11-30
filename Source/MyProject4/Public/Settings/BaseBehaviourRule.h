#pragma once

#include "CoreMinimal.h"
#include "BaseBehaviourRule.generated.h"

UENUM(BlueprintType)
enum class EExecutionTime : uint8
{
	Never = 0,
	Now = 1, 
	OnChange = 2,
	OnEnd = 3
};

class UCustomParameterBase;

UCLASS(Blueprintable, abstract, EditInlineNew)
class MYPROJECT4_API UBaseBehaviourRule : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Behavior Functions")
	bool StartBehavior();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Behavior Functions")
	bool StartBehaviorParam(UObject* Object);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Behavior Functions")
	UCustomParameterBase* GetOwnerParameter();
	

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior Rules")
	EExecutionTime ExecutionTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior Rules")
	FName BehaviourName;
};

inline UCustomParameterBase* UBaseBehaviourRule::GetOwnerParameter()
{
	return reinterpret_cast<UCustomParameterBase*>(GetOuter());
}

inline bool UBaseBehaviourRule::StartBehaviorParam_Implementation(UObject* Object)
{
	return false;
}

inline bool UBaseBehaviourRule::StartBehavior_Implementation()
{
	return false;
}
