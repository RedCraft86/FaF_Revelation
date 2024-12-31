// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Factories/Factory.h"
#include "ToroAssetFactory.generated.h"

UCLASS(Abstract)
class TOROEDITOR_API UToroAssetFactory : public UFactory
{
	GENERATED_BODY()

public:

	UToroAssetFactory()
	{
		bCreateNew = true;
		bEditAfterNew = true;
		SupportedClass = UDataAsset::StaticClass();
	}

	virtual bool ConfigureProperties() override
	{
		DataClass = UDataAsset::StaticClass();
		return DataClass ? true : false;
	}

	virtual FString GetDefaultNewAssetName() const override
	{
		return TEXT("New") + (DataClass ? DataClass->GetName() : GetSupportedClass()->GetName());
	}

	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override
	{
		const UClass* ObjClass = DataClass ? *DataClass : Class;
		check(ObjClass->IsChildOf(SupportedClass));
		return NewObject<UDataAsset>(InParent, ObjClass, Name, Flags | RF_Transactional);
	}
	
protected:

	TSubclassOf<UDataAsset> DataClass;
};