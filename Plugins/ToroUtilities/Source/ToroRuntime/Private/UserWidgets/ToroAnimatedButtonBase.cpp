// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserWidgets/ToroAnimatedButtonBase.h"
#include "Components/Button.h"

void UToroAnimatedButtonBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Button->OnClicked.AddDynamic(this, &UToroAnimatedButtonBase::OnButtonClicked);
	Button->OnHovered.AddDynamic(this, &UToroAnimatedButtonBase::OnButtonHovered);
	Button->OnUnhovered.AddDynamic(this, &UToroAnimatedButtonBase::OnButtonUnhovered);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UToroAnimatedButtonBase::OnButtonClicked()
{
	OnClicked.Broadcast();
	OnClickedBP.Broadcast();
}

void UToroAnimatedButtonBase::OnButtonHovered()
{
	PlayAnimationForward(ButtonAnim);
}

void UToroAnimatedButtonBase::OnButtonUnhovered()
{
	PlayAnimationReverse(ButtonAnim);
}
