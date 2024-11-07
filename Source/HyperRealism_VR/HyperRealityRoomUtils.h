// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HyperRealityRoomUtils.generated.h"

/**
 * 
 */
UCLASS()
class HYPERREALISM_VR_API UHyperRealityRoomUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "RoomUtils")
	static TArray<FTransform>SortWalls(const TArray<FTransform>& Walls, const FVector PlayerForward, const FVector RoomCenter);
	
	
};
