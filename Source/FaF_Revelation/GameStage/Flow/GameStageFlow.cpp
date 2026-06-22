// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#include "GameStageFlow.h"
#include "GameStageNode.h"
#include "GameStageRequirement.h"
#include "Nodes/Graph/FlowNode_Start.h"

UGameStageFlow::UGameStageFlow()
{
#if WITH_EDITORONLY_DATA
	AllowedNodeClasses = {
		UFlowNode_Start::StaticClass(),
		UGameStageNode::StaticClass(),
		UGameStageRequirement::StaticClass()
	};
#endif
}
