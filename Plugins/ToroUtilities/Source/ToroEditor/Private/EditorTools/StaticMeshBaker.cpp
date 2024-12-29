// Copyright (C) RedCraft86. All Rights Reserved.

#include "EditorTools/StaticMeshBaker.h"
#include "Subsystems/EditorActorSubsystem.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Engine/StaticMeshActor.h"
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

		// Transaction scope
		{
			const FScopedTransaction Transaction(NSLOCTEXT("ToroEditor", "BakeStaticMeshes", "Bake Static Meshes"));
			for (AActor* Actor : Actors)
			{
				if (!Actor) continue;
				TArray<UStaticMeshComponent*> MeshCompArray;
				Actor->GetComponents<UStaticMeshComponent>(MeshCompArray);
				if (MeshCompArray.IsEmpty()) continue;

				int32 Idx = 0;
				for (const UStaticMeshComponent* Comp : MeshCompArray)
				{
					if (!Comp || Comp->IsEditorOnly() || !Comp->GetStaticMesh()) continue;
					FStaticMeshProperties MeshProp = UPrimitiveDataLibrary::GetStaticMeshProperties(Comp);
					if (!MeshProp.IsValidData()) return;

					AStaticMeshActor* NewActor = nullptr;
					if (const UInstancedStaticMeshComponent* InstComp = Cast<UInstancedStaticMeshComponent>(Comp))
					{
						for (int i = 0; i < InstComp->GetNumInstances(); i++)
						{
							if (FTransform T; InstComp->GetInstanceTransform(i, T, true))
							{
								NewActor = SpawnActor(Subsystem, MeshProp, T, FString::Printf(TEXT("%s_Inst_%d"), *MeshProp.Mesh->GetName(), i + 1),
								(Actor->GetFolderPath().IsNone() ? TEXT("Baked") : Actor->GetFolderPath()).ToString() / Actor->GetActorLabel());
							}
						}
					}
					else
					{
						const FTransform Transform = Comp->GetComponentTransform();
						NewActor = SpawnActor(Subsystem, MeshProp, Transform, FString::Printf(TEXT("%s_%d"), *MeshProp.Mesh->GetName(), Idx + 1),
							(Actor->GetFolderPath().IsNone() ? TEXT("Baked") : Actor->GetFolderPath()).ToString() / Actor->GetActorLabel());
					}

					Idx++;
					if (NewActor) Subsystem->SetActorSelectionState(NewActor, true);
				}

				Subsystem->SetActorSelectionState(Actor, false);
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
	}
}

AStaticMeshActor* FStaticMeshBaker::SpawnActor(UEditorActorSubsystem* Subsystem, const FStaticMeshProperties& MeshProperties, const FTransform& Transform, const FString& Label,const FString& Folder)
{
	if (!Subsystem || !MeshProperties.IsValidData()) return nullptr;
	if (AStaticMeshActor* NewActor = Cast<AStaticMeshActor>(Subsystem->SpawnActorFromClass(AStaticMeshActor::StaticClass(),
		Transform.GetTranslation(), Transform.GetRotation().Rotator())))
	{
		NewActor->SetActorScale3D(Transform.GetScale3D());
		UPrimitiveDataLibrary::SetStaticMeshProperties(NewActor->GetStaticMeshComponent(), MeshProperties);
		Subsystem->SetActorSelectionState(NewActor, true);
		if (!Folder.IsEmpty()) NewActor->SetFolderPath(*Folder);
		if (!Label.IsEmpty()) NewActor->SetActorLabel(Label);

		return NewActor;
	}

	return nullptr;
}
