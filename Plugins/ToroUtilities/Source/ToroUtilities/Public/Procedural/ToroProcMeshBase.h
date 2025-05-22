// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroMeshGenBase.h"
#include "ProceduralMeshComponent.h"
#include "ToroProcMeshBase.generated.h"

UCLASS(Abstract)
class TOROUTILITIES_API AToroProcMeshBase : public AToroMeshGenBase
{
	GENERATED_BODY()

public:

	AToroProcMeshBase();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UProceduralMeshComponent> ProceduralMesh;
	
#if WITH_EDITORONLY_DATA
protected:
	TSet<int32> EditorSections = {1};
#endif
#if WITH_EDITOR
	UFUNCTION(CallInEditor, Category = Tools, DisplayName = "Create Asset")
		virtual void CreateStaticMesh();
#endif
};