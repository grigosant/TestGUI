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

UCLASS(Blueprintable, abstract, EditInlineNew)
class MYPROJECT4_API UBaseBehaviourRule : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Behavior Functions")
	bool StartBehavior();
	

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior Rules")
	EExecutionTime ExecutionTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior Rules")
	FName BehaviourName;
};

inline bool UBaseBehaviourRule::StartBehavior_Implementation()
{
	return false;
}
