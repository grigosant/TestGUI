// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "TickableEditorObject.h"
#include "TestEditorUtilityWidget.generated.h"

UCLASS()
class MYPROJECT4_API UTestEditorUtilityWidget : public UEditorUtilityWidget,
												public FTickableEditorObject
{
	GENERATED_BODY()
public:
	UTestEditorUtilityWidget();
	~UTestEditorUtilityWidget() override;

	void Run();
	void ShowDragNDropWindow();

	UFUNCTION(CallInEditor, BlueprintCallable)
	void StartEditorTimer();

	UFUNCTION(CallInEditor, BlueprintCallable)
	void StopEditorTimer();

	bool bIsShowingWindow;

	// To make sure it doesn't get garbage collected.
	UPROPERTY()
	TObjectPtr<UObject> CogWindowManagerRef = nullptr;

	///FTickableEditorObject
	virtual void Tick(float DeltaTime) override;
	virtual ETickableTickType GetTickableTickType() const override;
	virtual TStatId GetStatId() const override;
	virtual bool IsTickableWhenPaused() const;
	virtual bool IsTickableInEditor() const;
	virtual bool IsTickable() const override;
	
	//road data
	bool isShowingDemo = false;
	
	int32_t LinesNumber, LinesNumberOld;
	int32_t CellsInLineNumber, CellsInLineNumberOld;
	
	TArray<TArray<int>> RoadContainer;
	TArray<int> TempExample;
};
