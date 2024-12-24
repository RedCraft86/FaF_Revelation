// Copyright (C) RedCraft86. All Rights Reserved.

#include "DataGraphFactory.h"
#include "ClassViewerFilter.h"
#include "ClassViewerModule.h"
#include "Kismet2/SClassPickerDialog.h"

#include "DataGraphBase.h"

class FDataGraphAssetFilter final : public IClassViewerFilter
{
public:
	
	FDataGraphAssetFilter(){}
	
	TSet<const UClass*> AllowedChildrenOfClasses{UDataGraphBase::StaticClass()};
	virtual bool IsClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const UClass* InClass, TSharedRef<FClassViewerFilterFuncs> InFilterFuncs) override
	{
		return !InClass->HasAnyClassFlags(CLASS_Abstract | CLASS_Deprecated | CLASS_NewerVersionExists | CLASS_HideDropDown | CLASS_CompiledFromBlueprint)
			&& !InClass->GetBoolMetaDataHierarchical(TEXT("IsBlueprintBase")) // FBlueprintMetadata::MD_IsBlueprintBase
			&& InFilterFuncs->IfInChildOfClassesSet(AllowedChildrenOfClasses, InClass) != EFilterReturn::Failed;
	}

	virtual bool IsUnloadedClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const TSharedRef<const IUnloadedBlueprintData> InUnloadedClassData, TSharedRef<FClassViewerFilterFuncs> InFilterFuncs) override
	{
		return false;
	}
};

UDataGraphFactory::UDataGraphFactory()
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UDataGraphBase::StaticClass();
}

bool UDataGraphFactory::ConfigureProperties()
{
	GraphClass = nullptr;
	
	FClassViewerInitializationOptions Options;
	Options.Mode = EClassViewerMode::ClassPicker;
	Options.ClassFilters.Add(MakeShareable(new FDataGraphAssetFilter));

	UClass* ChosenClass = nullptr;
	const bool bPicked = SClassPickerDialog::PickClass(NSLOCTEXT("DataGraphEditor", "GraphFactory", "Pick Data Graph Class"),
		Options, ChosenClass, UDataGraphBase::StaticClass());

	if (bPicked) GraphClass = ChosenClass;
	return bPicked;
}

FString UDataGraphFactory::GetDefaultNewAssetName() const
{
	return TEXT("New") + (GraphClass ? GraphClass->GetName() : GetSupportedClass()->GetName());
}

UObject* UDataGraphFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	if (GraphClass)
	{
		return NewObject<UDataGraphBase>(InParent, GraphClass, Name, Flags | RF_Transactional);
	}
	
	check(Class->IsChildOf(UDataGraphBase::StaticClass()));
	return NewObject<UDataAsset>(InParent, Class, Name, Flags | RF_Transactional);
}
