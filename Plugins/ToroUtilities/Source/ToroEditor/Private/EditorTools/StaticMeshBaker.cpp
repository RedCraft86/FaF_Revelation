// Copyright (C) RedCraft86. All Rights Reserved.

#include "EditorTools/StaticMeshBaker.h"
#include "Subsystems/EditorActorSubsystem.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Helpers/LoggingMacros.h"
#include "EditorDialogLibrary.h"

void FStaticMeshBaker::ExecuteAction()
{
	if (UEditorActorSubsystem* Subsystem = GEditor ? GEditor->GetEditorSubsystem<UEditorActorSubsystem>() : nullptr)
	{
		const TArray<AActor*> Actors = Subsystem->GetSelectedLevelActors();
		if (Actors.IsEmpty()) return;

		if (UEditorDialogLibrary::ShowMessage(FText::FromString(TEXT("Are you sure you want to Bake Selected Meshes?")),
			FText::FromString(TEXT("Proceeding will gather static mesh components from the selected actors and bake them into individual static mesh actors.")),
			EAppMsgType::OkCancel, EAppReturnType::Cancel, EAppMsgCategory::Info) != EAppReturnType::Ok)
		{
			return;
		}
		
		const EAppReturnType::Type SrcRetType = UEditorDialogLibrary::ShowMessage(FText::FromString(TEXT("Do you want to destroy source actors?")),
			FText::FromString(TEXT("By clicking 'Yes' source actors will be destroyed, otherwise, they will be kept.\nDefault: Yes")),
			EAppMsgType::YesNoCancel, EAppReturnType::Yes, EAppMsgCategory::Info);

		if (SrcRetType == EAppReturnType::Cancel) return;
		const bool bDestroySourceActors = SrcRetType == EAppReturnType::Yes;

		UE_LOG_MESSAGE(LogToroEditor, 2.0f, TEXT("Starting Bake!%s"), *FString());

		// Transaction scope
		{
			const FScopedTransaction Transaction(NSLOCTEXT("ToroEditor", "BakeStaticMeshes", "Bake Static Meshes"));
			for (int i = 0; i < Actors.Num(); i++)
			{
				if (!Actors[i]) continue;
				UE_LOG_MESSAGE(LogToroEditor, 2.0f, TEXT(">	Baking [%d/%d] %s"),
					i + 1, Actors.Num(), *Actors[i]->GetName());

				TArray<UStaticMeshComponent*> MeshCompArray;
				Actors[i]->GetComponents<UStaticMeshComponent>(MeshCompArray);
				if (MeshCompArray.IsEmpty()) continue;

				int32 Idx = 0;
				for (const UStaticMeshComponent* Comp : MeshCompArray)
				{
					if (!Comp || Comp->IsEditorOnly() || !Comp->GetStaticMesh()) continue;
					FStaticMeshData MeshProp; MeshProp.FromStaticMeshComponent(Comp);
					if (!MeshProp.IsValidData()) return;

					AStaticMeshActor* NewActor = nullptr;
					if (const UInstancedStaticMeshComponent* InstComp = Cast<UInstancedStaticMeshComponent>(Comp))
					{
						for (int j = 0; j < InstComp->GetNumInstances(); j++)
						{
							if (FTransform T; InstComp->GetInstanceTransform(j, T, true))
							{
								NewActor = SpawnActor(Subsystem, MeshProp, T, FString::Printf(TEXT("%s_Inst_%d"), *MeshProp.Mesh->GetName(), j + 1),
								(Actors[i]->GetFolderPath().IsNone() ? TEXT("Baked") : Actors[i]->GetFolderPath()).ToString() / Actors[i]->GetActorLabel());
							}
						}
					}
					else
					{
						const FTransform Transform = Comp->GetComponentTransform();
						NewActor = SpawnActor(Subsystem, MeshProp, Transform, FString::Printf(TEXT("%s_%d"), *MeshProp.Mesh->GetName(), Idx + 1),
							(Actors[i]->GetFolderPath().IsNone() ? TEXT("Baked") : Actors[i]->GetFolderPath()).ToString() / Actors[i]->GetActorLabel());
					}

					Idx++;
					if (NewActor) Subsystem->SetActorSelectionState(NewActor, true);
				}

				Subsystem->SetActorSelectionState(Actors[i], false);
			}
		}

		// Transaction scope
		{
			const FScopedTransaction Transaction(NSLOCTEXT("ToroEditor", "DestroySourceMeshes", "Destroy Source Meshes (Delete and Undo to fix)"));
			if (bDestroySourceActors)
			{
				Subsystem->DestroyActors(Actors);
			}
		}

		UE_LOG_MESSAGE(LogToroEditor, 2.0f, TEXT("Bake Finished!%s"), *FString());
	}
}

AStaticMeshActor* FStaticMeshBaker::SpawnActor(UEditorActorSubsystem* Subsystem, const FStaticMeshData& MeshProperties, const FTransform& Transform, const FString& Label,const FString& Folder)
{
	if (!Subsystem || !MeshProperties.IsValidData()) return nullptr;
	if (AStaticMeshActor* NewActor = Cast<AStaticMeshActor>(Subsystem->SpawnActorFromClass(AStaticMeshActor::StaticClass(),
		Transform.GetTranslation(), Transform.GetRotation().Rotator())))
	{
		NewActor->SetActorScale3D(Transform.GetScale3D());
		MeshProperties.ToStaticMeshComponent(NewActor->GetStaticMeshComponent());
		Subsystem->SetActorSelectionState(NewActor, true);
		if (!Folder.IsEmpty()) NewActor->SetFolderPath(*Folder);
		if (!Label.IsEmpty()) NewActor->SetActorLabel(Label);

		return NewActor;
	}

	return nullptr;
}
