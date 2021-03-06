// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassEntityTraitBase.h"
#include "MassObserverProcessor.h"
#include "MassProcessor.h"
#include "MassRepresentationSubsystem.h"
#include "MassSignalSubsystem.h"
#include "SmartObjectSubsystem.h"
#include "RTSAgentTrait.generated.h"

class UMassEntitySubsystem;
class URTSBuildingSubsystem;
class USmartObjectSubsystem;

UENUM()
enum EResourceType
{
	Tree,
	Rock
};

UENUM()
enum EAgentState
{
	Idle,
	ChopResource,
	CollectItem,
	BuildFloor
};

/**
 * @brief Fragment given to entities to grant resources
 */
USTRUCT()
struct MASSAITESTING_API FRTSGatherResourceFragment : public FMassFragment
{
	GENERATED_BODY();

	// Type of resource to gather
	EResourceType Resource = Tree;

	// Amount of resource to gather
	int Amount = 1;
};

/**
 * @brief Processes FRTSGatherResourceFragment to grant resources to entity in FRTSAgent
 */
UCLASS()
class MASSAITESTING_API URTSGatherResourceProcessor : public UMassProcessor
{
	GENERATED_BODY()
public:
	virtual void Execute(UMassEntitySubsystem& EntitySubsystem, FMassExecutionContext& Context) override;
	virtual void ConfigureQueries() override;
	virtual void Initialize(UObject& Owner) override;

	FMassEntityQuery EntityQuery;

	UPROPERTY()
	UMassRepresentationSubsystem* RepresentationSubsystem;
};

/**
 * @brief Base fragment for RTS Agents
 */
USTRUCT(BlueprintType)
struct MASSAITESTING_API FRTSAgentFragment : public FMassFragment
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "")
	TMap<TEnumAsByte<EResourceType>, int> Inventory;

	UPROPERTY()
	float SkinIndex = -1;

	UPROPERTY()
	bool bPunching = false;

	// todo Move these properties to another fragment when I find out how to handle state tree tasks missing fragments :(
	// Building management
	UPROPERTY()
	FSmartObjectHandle BuildingHandle;

	// Resource chopping management
	UPROPERTY()
	FSmartObjectHandle ResourceHandle;

	// Item management
	UPROPERTY()
	TArray<FMassEntityHandle> QueuedItems;
};

/**
 * @brief Base parameters for FRTSAgentTrait
 */
USTRUCT()
struct MASSAITESTING_API FRTSAgentParameters : public FMassSharedFragment
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere);
	TMap<TEnumAsByte<EResourceType>, int> DefaultRequiredResources;
};

/**
 * @brief Declares this entity as an RTS Agent
 */
UCLASS()
class MASSAITESTING_API URTSAgentTrait : public UMassEntityTraitBase
{
	GENERATED_BODY()

	virtual void BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, UWorld& World) const override;

	UPROPERTY(EditAnywhere)
	FRTSAgentParameters AgentParameters;
};

/**
 * @brief Initializes RTS Agents with base required resources
 */
UCLASS()
class MASSAITESTING_API URTSAgentInitializer : public UMassObserverProcessor
{
	GENERATED_BODY()

	URTSAgentInitializer();
	
	virtual void Execute(UMassEntitySubsystem& EntitySubsystem, FMassExecutionContext& Context) override;
	virtual void ConfigureQueries() override;
	virtual void Initialize(UObject& Owner) override;

	TObjectPtr<URTSBuildingSubsystem> RTSMovementSubsystem;
	TObjectPtr<USmartObjectSubsystem> SmartObjectSubsystem;

	FMassEntityQuery EntityQuery;
};

/**
 * @brief Setup ISMs, animation, and textures for RTS Agents
 */
UCLASS()
class URTSAnimationProcessor : public UMassProcessor
{
	GENERATED_BODY()

	URTSAnimationProcessor();

	virtual void Initialize(UObject& Owner) override;
	virtual void ConfigureQueries() override;
	virtual void Execute(UMassEntitySubsystem& EntitySubsystem, FMassExecutionContext& Context) override;

	FMassEntityQuery EntityQuery;

	TObjectPtr<UMassRepresentationSubsystem> RepresentationSubsystem;
};

/**
 * @brief Initializes Animation state
 */
UCLASS()
class MASSAITESTING_API URTSAnimationInitializer : public UMassObserverProcessor
{
	GENERATED_BODY()

	URTSAnimationInitializer();
	
	virtual void Execute(UMassEntitySubsystem& EntitySubsystem, FMassExecutionContext& Context) override;
	virtual void ConfigureQueries() override;
	virtual void Initialize(UObject& Owner) override;

	TObjectPtr<URTSBuildingSubsystem> RTSMovementSubsystem;
	TObjectPtr<USmartObjectSubsystem> SmartObjectSubsystem;
	TObjectPtr<UMassRepresentationSubsystem> RepresentationSubsystem;

	FMassEntityQuery EntityQuery;
};

// Animation state of the ISM agent

// Stores data of the ISM agent for instanced custom data
USTRUCT()
struct FAgentAnimationData : public FMassFragment
{
    GENERATED_BODY()

	float SkinIndex = -1.f;
	
    float AnimState = 0.f;
	
	float IsPunching = 0.f;
};

/**
 * @brief Basic tag to declare entity as RTS Agent
 */
USTRUCT()
struct MASSAITESTING_API FRTSAgent : public FMassTag
{
	GENERATED_BODY();
};

/**
 * @brief Tag used to request required resources from URTSAgentInitializer
 */
USTRUCT()
struct MASSAITESTING_API FRTSRequestResources : public FMassTag
{
	GENERATED_BODY()
};
