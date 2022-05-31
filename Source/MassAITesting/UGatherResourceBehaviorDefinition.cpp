﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "UGatherResourceBehaviorDefinition.h"

#include "MassCommandBuffer.h"
#include "MassSmartObjectFragments.h"
#include "RTSMovementTrait.h"
#include "SmartObjectComponent.h"

void UUGatherResourceBehaviorDefinition::Activate(FMassCommandBuffer& CommandBuffer,
                                                  const FMassBehaviorEntityContext& EntityContext) const
{
	Super::Activate(CommandBuffer, EntityContext);

	// Spawn resource fragment with set values
	FRTSGatherResourceFragment RTSResourceFragment;
	RTSResourceFragment.Resource = ResourceType;
	RTSResourceFragment.Amount = ResourceAmount;
	CommandBuffer.PushCommand(FCommandAddFragmentInstance(EntityContext.EntityView.GetEntity(), FConstStructView::Make(RTSResourceFragment)));
	
	// Traditional way to spawn a default fragment
	//CommandBuffer.AddFragment<FRTSGatherResourceFragment>(EntityContext.EntityView.GetEntity());
}

void UUGatherResourceBehaviorDefinition::Deactivate(FMassCommandBuffer& CommandBuffer,
	const FMassBehaviorEntityContext& EntityContext) const
{
	Super::Deactivate(CommandBuffer, EntityContext);
	
	//CommandBuffer.RemoveFragment<FRTSGatherResourceFragment>(EntityContext.EntityView.GetEntity());
	
	const FMassSmartObjectUserFragment& SOUser = EntityContext.EntityView.GetFragmentData<FMassSmartObjectUserFragment>();
	EntityContext.SmartObjectSubsystem.GetSmartObjectComponent(SOUser.ClaimHandle)->GetOwner()->Destroy();
}