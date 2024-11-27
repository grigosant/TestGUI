// Fill out your copyright notice in the Description page of Project Settings.


#include "TestEditorUtilityWidget.h"
#include "imgui.h"
#include "Serialization/JsonTypes.h"

UTestEditorUtilityWidget::UTestEditorUtilityWidget()
{
	bIsShowingWindow = false;
    LinesNumber = LinesNumberOld = 1;
	CellsInLineNumber = CellsInLineNumberOld = 1;
	
	//инициализация многомерного массива
	RoadContainer.Init(TempExample, LinesNumber);
}

UTestEditorUtilityWidget::~UTestEditorUtilityWidget()
{
	StopEditorTimer();
}


void UTestEditorUtilityWidget::StartEditorTimer()
{
	bIsShowingWindow = true;
}

void UTestEditorUtilityWidget::StopEditorTimer()
{
	bIsShowingWindow = false;
}

void UTestEditorUtilityWidget::Tick(float DeltaTime)
{
	if(!bIsShowingWindow)
		return;

	const ImGui::FScopedContext ScopedContext;
	if (ScopedContext)
	{
		// Your ImGui code goes here!
	    
	    ImGui::Checkbox("Show demo?",&isShowingDemo);
		if(isShowingDemo)
		{
			ImGui::ShowDemoWindow();
		}
		else
		{
			ShowDragNDropWindow();
		}
	}
}

ETickableTickType UTestEditorUtilityWidget::GetTickableTickType() const
{
	return ETickableTickType::Conditional;
}
TStatId UTestEditorUtilityWidget::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(FAssetTest, STATGROUP_Tickables);
}
bool UTestEditorUtilityWidget::IsTickableWhenPaused() const
{
	return true;
}
bool UTestEditorUtilityWidget::IsTickableInEditor() const
{
	return true;
}
bool UTestEditorUtilityWidget::IsTickable() const
{
	return true;
}
void UTestEditorUtilityWidget::Run()
{
	Super::Run();
}

void UTestEditorUtilityWidget::ShowDragNDropWindow()
{
	ImGui::InputInt("Lines number", &LinesNumber);
	ImGui::InputInt("Cells number", &CellsInLineNumber);

	//>=1
	if(CellsInLineNumber <= 1)
		CellsInLineNumber = 1;
	if(LinesNumber <= 1)
		LinesNumber = 1;

	
	if(LinesNumber != LinesNumberOld)
	{
		//изменение числа линий
		if(LinesNumber > LinesNumberOld)
		{
			TArray<int> TmpFillArray;
			TmpFillArray.Init(0, CellsInLineNumber);
			for(int n = 0; n < LinesNumber - LinesNumberOld; n++)
			{
				RoadContainer.Push(TmpFillArray);
			}
		}
		else
		{
			RoadContainer.SetNum(LinesNumber);
		}
		LinesNumberOld = LinesNumber;
	}

	if(CellsInLineNumber != CellsInLineNumberOld)
	{
		//изменение числа клеток в линии
		TempExample.SetNum(CellsInLineNumber);
		for (int n = 0; n < RoadContainer.Num(); n++)
		{
			RoadContainer[n].SetNum(CellsInLineNumber);
		}
		CellsInLineNumberOld = CellsInLineNumber;
	}
	
	ImGui::NewLine();
	for (auto RoadContaierInside : RoadContainer)
	{
		for (int n = 0; n < RoadContaierInside.Num(); n++)
		{
			ImGui::PushID(n);
			ImGui::SameLine();
			ImGui::Button( "0", ImVec2(60, 60));
			ImGui::PopID();
		}
		ImGui::NewLine();
	}


	//ImgUI EXAMPLE of drag'n'drop
	//IMGUI_DEMO_MARKER("Widgets/Drag and drop");
    if (ImGui::TreeNode("Drag and Drop"))
    {
        //IMGUI_DEMO_MARKER("Widgets/Drag and drop/Copy-swap items");
        if (ImGui::TreeNode("Drag and drop to copy/swap items"))
        {
            enum Mode
            {
                Mode_Copy,
                Mode_Move,
                Mode_Swap
            };
            static int mode = 0;
            if (ImGui::RadioButton("Copy", mode == Mode_Copy)) { mode = Mode_Copy; } ImGui::SameLine();
            if (ImGui::RadioButton("Move", mode == Mode_Move)) { mode = Mode_Move; } ImGui::SameLine();
            if (ImGui::RadioButton("Swap", mode == Mode_Swap)) { mode = Mode_Swap; }
            static const char* names[9] =
            {
                "Bobby", "Beatrice", "Betty",
                "Brianna", "Barry", "Bernard",
                "Bibi", "Blaine", "Bryn"
            };
            for (int n = 0; n < IM_ARRAYSIZE(names); n++)
            {
                ImGui::PushID(n);
                if ((n % 3) != 0)
                    ImGui::SameLine();
                ImGui::Button(names[n], ImVec2(60, 60));

                // Our buttons are both drag sources and drag targets here!
                if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
                {
                    // Set payload to carry the index of our item (could be anything)
                    ImGui::SetDragDropPayload("DND_DEMO_CELL", &n, sizeof(int));

                    // Display preview (could be anything, e.g. when dragging an image we could decide to display
                    // the filename and a small preview of the image, etc.)
                    if (mode == Mode_Copy) { ImGui::Text("Copy %s", names[n]); }
                    if (mode == Mode_Move) { ImGui::Text("Move %s", names[n]); }
                    if (mode == Mode_Swap) { ImGui::Text("Swap %s", names[n]); }
                    ImGui::EndDragDropSource();
                }
                if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DEMO_CELL"))
                    {
                        IM_ASSERT(payload->DataSize == sizeof(int));
                        int payload_n = *(const int*)payload->Data;
                        if (mode == Mode_Copy)
                        {
                            names[n] = names[payload_n];
                        }
                        if (mode == Mode_Move)
                        {
                            names[n] = names[payload_n];
                            names[payload_n] = "";
                        }
                        if (mode == Mode_Swap)
                        {
                            const char* tmp = names[n];
                            names[n] = names[payload_n];
                            names[payload_n] = tmp;
                        }
                    }
                    ImGui::EndDragDropTarget();
                }
                ImGui::PopID();
            }
            ImGui::TreePop();
        }

        ImGui::TreePop();
    }
}