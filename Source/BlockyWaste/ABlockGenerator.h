#pragma once
#include "BlockyWasteCharacter.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Engine/StaticMesh.h"

#include "ABlockGenerator.generated.h"

UCLASS()
class ABlockGenerator : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABlockGenerator();

	// Called after the C++ constructor and after the properties have been initialized.
	virtual void PostInitializeComponents() override;

	// Called when the actor has begun playing in the level
	virtual void BeginPlay() override;

	//virtual void Tick(float DeltaSeconds) override;



	//The player as a refference
	UPROPERTY(VisibleAnywhere) 
    ABlockyWasteCharacter* PlayerCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UHierarchicalInstancedStaticMeshComponent* _blocksMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMesh* block = nullptr;

};