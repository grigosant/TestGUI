// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EArrayHeadResult : uint8 {
  Valid,
  Invalid
};

UCLASS()
class MYPROJECT4_API UMyBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	UFUNCTION(BlueprintCallable, CustomThunk,
    meta = (DisplayName = "Head", CompactNodeTitle = "HEAD", 
            ArrayParm = "Array", ArrayTypeDependentParams = "Head", ExpandEnumAsExecs="Result"),
    Category = "Dream|Utilities")
  static void Array_Head(const TArray<int32> &Array, int32 &Head, EArrayHeadResult &Result);

private:
  DECLARE_FUNCTION(execArray_Head) {
    Stack.MostRecentProperty = nullptr;
    Stack.StepCompiledIn<FArrayProperty>(nullptr);
    const void *Array = Stack.MostRecentPropertyAddress;
    const auto *ArrayProperty = CastField<FArrayProperty>(Stack.MostRecentProperty);
    if (!ArrayProperty) {
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

    Stack.MostRecentProperty = nullptr;
    Stack.StepCompiledIn<FEnumProperty>(nullptr);
    void *Result = Stack.MostRecentPropertyAddress;
    const auto ResultProperty = CastField<FEnumProperty>(Stack.MostRecentProperty);
    if (!ResultProperty) {
      Stack.bArrayContextFailed = true;
      return;
    }


    P_FINISH

    P_NATIVE_BEGIN
      genericArray_Head(
        Array, ArrayProperty,
        Head, HeadProperty,
        Result, ResultProperty);
    P_NATIVE_END
  }

  static void genericArray_Head(
    const void *Array, const FArrayProperty *ArrayProperty,
    void *Head, const FProperty *HeadProperty,
    void *Result, const FEnumProperty *ResultProperty
    ) {
    if (Array && Head && Result) {
      FScriptArrayHelper ArrayHelper(ArrayProperty, Array);
      if (ArrayHelper.Num() == 0) {
        ResultProperty->
          GetUnderlyingProperty()->
          SetIntPropertyValue(Result, static_cast<uint64>(EArrayHeadResult::Invalid));
      } else {
        ResultProperty->
          GetUnderlyingProperty()->
          SetIntPropertyValue(Result, static_cast<uint64>(EArrayHeadResult::Valid));
        HeadProperty->InitializeValue(Head);
        HeadProperty->CopyCompleteValue(Head, ArrayHelper.GetElementPtr(0));
      }
    }
  }

};