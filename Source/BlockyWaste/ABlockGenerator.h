#pragma once

#ifdef __INTELLISENSE__

#define OVERRIDE_PLATFORM_HEADER_NAME ./
//#define PRAGMA_DISABLE_DEPRECATION_WARNINGS if 0
//#define PRAGMA_ENABLE_DEPRECATION_WARNINGS endif
#else
#endif


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "World/math/Vector3.h"
#include "world/map/Chunk.h"
#include "world/map/MapManager.h"

//#ifdef __INTELLISENSE__
//#define GENERATED_BODY(...)
//#define UCLASS(...)
//#else
#include "ABlockGenerator.generated.h"
//#endif

class ABlockyWasteCharacter;
class AStaticMeshActor;
class USTaticMesh;
class UInstancedStaticMeshComponent;
class UInstancedStaticMeshWithReuse;
class UMaterial;


template <typename NumType, unsigned char Size>
uint32 GetTypeHash(const World::Vector<NumType, Size>& InVec) { return InVec.GetTypeHash(); }

//struct MapLoadInfo
//{
//	// list of actors relevant to this chunk
//	std::vector<AACtor*> actors;
//	// coordinates of the chunk
//	Vector3LL chunkCoordinates;
//};

struct BlockInstInfo
{
  uint32 id;
  int32 instId;
};

struct ChunkInstInfo
{
	TArray<BlockInstInfo> blocks;
	World::Chunk::Coords chunkCoord;
};

UCLASS()
class ABlockGenerator : public AActor
{
	GENERATED_BODY()
public:
	//The player as a reference point where to place blocks and push them to the engine
  UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ABlockyWasteCharacter* PlayerCharacter = nullptr;

	// Render distance in metres, any data further away may be unloaded
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Render distance [m]")
		float RenderDistance = 50;

	// Mesh component for rocks
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Meshes")
		UInstancedStaticMeshWithReuse* BlocksMeshComponentRock = nullptr;

  // Mesh component for dirt
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Meshes")
		UInstancedStaticMeshWithReuse* BlocksMeshComponentDirt = nullptr;

  // Mesh component for grass
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Meshes")
		UInstancedStaticMeshWithReuse* BlocksMeshComponentGrass = nullptr;

	// Template for the blocks, taken from the default UE cube
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMesh* BlockMesh = nullptr;

	// Template for static meshes for each block
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AStaticMeshActor> BlockTemplateClass;

	// Material for the blocks
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterial* BlockMaterial;

  FTimerHandle ChunkLoadTimer;

	// Sets default values for this actor's properties
	ABlockGenerator();

	// Called after the C++ constructor and after the properties have been initialized.
	virtual void PostInitializeComponents() override;

	// Called when the actor has begun playing in the level
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	
protected:
	UInstancedStaticMeshWithReuse* GetBlockStaticMesh(uint32 blockType);
	virtual void PopulateNearPlayer();
private:
	using InstArray = TArray<BlockInstInfo>;
	using InstanceMap = TMap<World::Chunk::Coords, InstArray>;

	FVector _lastPlayerPos;
	InstanceMap _chunkInstances;
  World::MapManager _mapManager;
};
