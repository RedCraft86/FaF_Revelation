// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroCharacterBase.h"
#include "ToroNPCBase.generated.h"

UENUM(BlueprintType)
enum class EToroEnemyState : uint8
{
	None,
	Roam,
	Seen,
	Chase,
	Search
};

UCLASS(Abstract)
class TORORUNTIME_API AToroNPCBase : public AToroCharacterBase
{
	GENERATED_BODY()

public:
	
	AToroNPCBase();

	UFUNCTION(BlueprintPure, Category = Game, DisplayName = "Set NPC State")
		void SetNPCState(const EToroEnemyState InState);

	UFUNCTION(BlueprintPure, Category = Game, DisplayName = "Get NPC State")
		EToroEnemyState GetNPCState() const { return bEnemy ? EnemyState : EToroEnemyState::None; }

protected:

	UPROPERTY(EditAnywhere, Category = CharacterSettings, DisplayName = "Is Enemy")
		bool bEnemy;

	UPROPERTY(EditAnywhere, Category = CharacterSettings)
		EToroEnemyState EnemyState;

	UPROPERTY(Transient)
		TObjectPtr<class AToroPlayerBase> PlayerChar;
	
	virtual void BeginPlay() override;
};
