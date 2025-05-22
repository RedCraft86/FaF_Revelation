// Copyright (C) RedCraft86. All Rights Reserved.

#include "EditorTools/StaticMeshInstancer.h"
#include "Subsystems/EditorActorSubsystem.h"
#include "Engine/StaticMeshActor.h"
#include "EditorDialogLibrary.h"
#include "Helpers/LoggingMacros.h"
#include "DataTypes/PrimitiveData.h"
#include "Actors/ISMActor.h"

void FStaticMeshInstancer::ExecuteAction()
{
	#define FindMesh(Prop) Meshes.FindByPredicate([Prop](const FInstancerMeshes& Element) -> bool \
		{ \
			return Element.MeshProp.Mesh == Prop.Mesh \
			&& Element.MeshProp.Materials == Prop.Materials \
			&& Element.MeshProp.bCastShadows == Prop.bCastShadows; \
		})
	
	struct FInstancerMeshes
	{
		FString Label = TEXT("Spawned");
		FName FolderPath = NAME_None;
		FStaticMeshData MeshProp = {};
		TArray<FTransform> Transforms = {};
	};
	
	if (UEditorActorSubsystem* Subsystem = GEditor ? GEditor->GetEditorSubsystem<UEditorActorSubsystem>() : nullptr)
	{
		const TArray<AActor*> Actors = Subsystem->GetSelectedLevelActors();
		if (Actors.IsEmpty()) return;

		if (UEditorDialogLibrary::ShowMessage(
			FText::FromString(TEXT("Are you sure you want to Convert Selected Meshes to Instanced Static Meshes?")),
			FText::FromString(TEXT("Proceeding will gather static mesh components from the selected actors and convert them into instanced static mesh actors.")),
			EAppMsgType::OkCancel, EAppReturnType::Cancel, EAppMsgCategory::Info) != EAppReturnType::Ok) return;

		const EAppReturnType::Type HISMType = UEditorDialogLibrary::ShowMessage(FText::FromString(TEXT("Use HISMs?")),
			FText::FromString(TEXT("Hierarchical Instanced Static Mesh will be used instead of Instanced Static Mesh.")),
			EAppMsgType::YesNoCancel, EAppReturnType::No, EAppMsgCategory::Info);

		if (HISMType == EAppReturnType::Cancel) return;

		const bool bUseHISM = HISMType == EAppReturnType::Yes;

		UE_LOG_MESSAGE(LogToroEditor, 2.0f, TEXT("Starting Instancing!%s"), *FString());

		// Inner scope
		{
			const FScopedTransaction Transaction(NSLOCTEXT("ToroEditor", "ConvertMeshesToInstances", "Convert Meshes to Instances"));
			FStaticMeshData MeshProp;
			TArray<FInstancerMeshes> Meshes;
			for (int i = 0; i < Actors.Num(); i++)
			{
				if (!Actors[i]) continue;
				UE_LOG_MESSAGE(LogToroEditor, 2.0f, TEXT(">	Instancing [%d/%d] %s"),
					i + 1, Actors.Num(), *Actors[i]->GetName());
				i++;
				
				TArray<UStaticMeshComponent*> MeshCompArray;
				Actors[i]->GetComponents<UStaticMeshComponent>(MeshCompArray);
				for (UStaticMeshComponent* Comp : MeshCompArray)
				{
					if (!Comp || Comp->IsEditorOnly()) continue;
					MeshProp.FromStaticMeshComponent(Comp);
					if (const UInstancedStaticMeshComponent* InstComp = Cast<UInstancedStaticMeshComponent>(Comp))
					{
						if (FInstancerMeshes* Mesh = FindMesh(MeshProp))
						{
							for (int j = 0; j < InstComp->GetNumInstances(); j++)
							{
								FTransform T;
								if (InstComp->GetInstanceTransform(j, T, true))
								{
									Mesh->Transforms.Add(T);
								}
							}
						}
						else
						{
							FInstancerMeshes NewMesh;
							NewMesh.MeshProp = MeshProp;
							for (int j = 0; j < InstComp->GetNumInstances(); j++)
							{
								FTransform T;
								if (InstComp->GetInstanceTransform(j, T, true))
								{
									NewMesh.Transforms.Add(T);
								}
							}

							NewMesh.Label = FString::Printf(TEXT("%s_%d"),
								*MeshProp.Mesh.GetAssetName(), Meshes.Num() + 1);
						
							NewMesh.FolderPath = *(Actors[i]->GetFolderPath().IsNone() ? TEXT("Instanced")
								: Actors[i]->GetFolderPath().ToString() / Actors[i]->GetActorLabel());
						
							Meshes.Add(NewMesh);
						}
					}
					else
					{
						if (FInstancerMeshes* Mesh = FindMesh(MeshProp))
						{
							Mesh->Transforms.Add(Comp->GetComponentTransform());
						}
						else
						{
							FInstancerMeshes NewMesh;
							NewMesh.MeshProp = MeshProp;
							NewMesh.Transforms.Add(Comp->GetComponentTransform());
						
							NewMesh.Label = FString::Printf(TEXT("%s_%d"),
								*MeshProp.Mesh.GetAssetName(), Meshes.Num() + 1);
							
							NewMesh.FolderPath = *(Actors[i]->GetFolderPath().IsNone() ? TEXT("Instanced")
								: Actors[i]->GetFolderPath().ToString() / Actors[i]->GetActorLabel());
						
							Meshes.Add(NewMesh);
						}
					}
				}

				Subsystem->SetActorSelectionState(Actors[i], false);
			}

			for (const FInstancerMeshes& Mesh : Meshes)
			{
				if (Mesh.Transforms.IsEmpty()) continue;
				if (Mesh.Transforms.Num() > 1)
				{
					AInstancedStaticMeshActor* ISMActor = Cast<AInstancedStaticMeshActor>(Subsystem->SpawnActorFromClass(
					   bUseHISM ? AHierarchicalInstancedStaticMeshActor::StaticClass() : AInstancedStaticMeshActor::StaticClass(),
					   Mesh.Transforms[0].GetTranslation(), Mesh.Transforms[0].GetRotation().Rotator()));

					ISMActor->SetActorLabel(Mesh.Label);
					ISMActor->SetFolderPath(Mesh.FolderPath);
					Subsystem->SetActorSelectionState(ISMActor, true);
					Mesh.MeshProp.ToStaticMeshComponent(ISMActor->GetStaticMeshComponent());

#if WITH_EDITORONLY_DATA
					ISMActor->Instances.Empty(Mesh.Transforms.Num());
					for (const FTransform& T : Mesh.Transforms)
					{
						ISMActor->Instances.Add({
							Mesh.Transforms[0].InverseTransformRotation(T.GetRotation()),
							Mesh.Transforms[0].InverseTransformPositionNoScale(T.GetTranslation()),
							T.GetScale3D()
						});
					}
#endif
					ISMActor->GetStaticMeshComponent()->ClearInstances();
					ISMActor->GetStaticMeshComponent()->AddInstances(Mesh.Transforms, false, true);
				}
				else
				{
					AStaticMeshActor* SMActor = Cast<AStaticMeshActor>(Subsystem->SpawnActorFromClass(AStaticMeshActor::StaticClass(),
						Mesh.Transforms[0].GetTranslation(), Mesh.Transforms[0].GetRotation().Rotator()));
					SMActor->SetActorScale3D(Mesh.Transforms[0].GetScale3D());
					
					SMActor->SetActorLabel(Mesh.Label);
					SMActor->SetFolderPath(Mesh.FolderPath);
					Subsystem->SetActorSelectionState(SMActor, true);
					Mesh.MeshProp.ToStaticMeshComponent(SMActor->GetStaticMeshComponent());
				}
			}
		}

		// Transaction scope
		{
			const FScopedTransaction Transaction(NSLOCTEXT("ToroEditor", "DestroySourceMeshes", "Destroy Source Meshes (Delete and Undo to fix)"));
			Subsystem->DestroyActors(Actors);
		}

		UE_LOG_MESSAGE(LogToroEditor, 2.0f, TEXT("Finished Instancing!%s"), *FString());
	}
#undef FindMesh
}
