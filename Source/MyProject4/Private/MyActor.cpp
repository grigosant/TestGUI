// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"

// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.SetTickFunctionEnable(true);
	PrimaryActorTick.bStartWithTickEnabled = true;

}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();

	const ImGui::FScopedContext ScopedContext;
	if (ScopedContext)
	{
		// Your ImGui code goes here!
		ImGui::ShowDemoWindow();
	}
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	const FString Label = GetActorLabel();
	
	const ImGui::FScopedContext ScopedContext;
	if (ScopedContext)
	{
		ImGui::Begin(TCHAR_TO_ANSI(*Label), nullptr, ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::SliderFloat("Scale", &Size, 0, 5);
		ImGui::End();
		if(Size != SizeOld)
		{
			SizeOld = Size;
			SetActorScale3D(FVector(SizeOld));
		}

	}
}

