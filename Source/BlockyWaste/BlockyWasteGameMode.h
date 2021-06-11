// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "GameFramework/GameModeBase.h"
#include "BlockyWasteGameMode.generated.h"

UCLASS(minimalapi)
class ABlockyWasteGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABlockyWasteGameMode();
  virtual void StartPlay() override; 
private:

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UHierarchicalInstancedStaticMeshComponent* _blocksMesh;

  UStaticMesh* block = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Game/Geometry/Meshes/1M_Cube.1M_Cube")));
};


