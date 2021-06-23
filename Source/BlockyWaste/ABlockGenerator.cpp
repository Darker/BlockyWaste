#include "ABlockGenerator.h"
#include "BlockyWasteCharacter.h"
#include "InstancedStaticMeshWithReuse.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "Materials/Material.h"
#include "Kismet/GameplayStatics.h"


#include "world/map/Chunk.h"

using namespace World;

ABlockGenerator::ABlockGenerator()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshFinder(TEXT("/Engine/BasicShapes/Cube"));
  if (MeshFinder.Object)
  {
    BlockMesh = MeshFinder.Object;
  }
	else
	{
		((ABlockGenerator*)nullptr)->BeginPlay();
	}

	
	//BlocksMeshComponent->SetStaticMesh(BlockMesh);
	//BlocksMeshComponent->SetMaterial(0, BlockMaterial);
  BlocksMeshComponentRock = CreateDefaultSubobject<UInstancedStaticMeshWithReuse>(TEXT("Instanced blocks mesh Rock"));
  BlocksMeshComponentDirt = CreateDefaultSubobject<UInstancedStaticMeshWithReuse>(TEXT("Instanced blocks mesh Dirt"));
  BlocksMeshComponentGrass = CreateDefaultSubobject<UInstancedStaticMeshWithReuse>(TEXT("Instanced blocks mesh Grass"));
  

	//RootComponent = BlocksMeshComponent;
}
void ABlockGenerator::PostInitializeComponents()
{
	AActor::PostInitializeComponents();
  BlocksMeshComponentRock->RegisterComponent();
  BlocksMeshComponentDirt->RegisterComponent();
  BlocksMeshComponentGrass->RegisterComponent();
}

void ABlockGenerator::BeginPlay()
{
	AActor::BeginPlay();
  GetWorldTimerManager().SetTimer(ChunkLoadTimer, this, &ABlockGenerator::PopulateNearPlayer, 0.1f, true, 0.0f);

  // Move player slightly above ground level
  FVector playerLocation = PlayerCharacter->GetActorLocation();
  int height = _mapManager.GetWorldHeight({ { playerLocation.X, playerLocation.Y } });
  playerLocation.Z = height * 100.0f+1500.0f;
  PlayerCharacter->SetActorLocation(playerLocation, false, nullptr, ETeleportType::TeleportPhysics);
  //PopulateNearPlayer();
	//Cast<ABlockyWasteCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	//UWorld* const GameWorld = GetWorld();
	//if (GameWorld)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("This message will appear on the screen!"));

	//	//FVector playerOrigin = PlayerCharacter->
	//	// for a test, just add them everywhere
	//	for (int i = 0; i < 2000; ++i)
	//	{

	//		const World::Vector3<float> coords = World::Vector3<int>::FromArrayOffset<World::CHUNK_SIZE>(i).Convert<float>();
	//		const FVector location(100*coords[0], 100 * coords[1], 300 + 100 * coords[2]);
	//		FTransform t(location);
	//		BlocksMeshComponent->AddInstance(t);
	//		
	//		// if I use individual meshes it works fine
	//		//if (BlockMaterial)
	//		//{
	//		//	AStaticMeshActor* actor = GameWorld->SpawnActorDeferred<AStaticMeshActor>(BlockTemplateClass, t, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	//		//	actor->GetStaticMeshComponent()->SetStaticMesh(BlockMesh);
	//		//	actor->GetStaticMeshComponent()->SetMaterial(0, BlockMaterial);
	//		//	UGameplayStatics::FinishSpawningActor(actor, t);
	//		//}

	//	}
	//}
}

void ABlockGenerator::PopulateNearPlayer()
{
  
  // calculate chunks around player
  FVector playerLocation = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation();
  //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player: [") + FString::SanitizeFloat(playerLocation.X) + "," + FString::SanitizeFloat(playerLocation.Y) + "," + FString::SanitizeFloat(playerLocation.Z) + "]");
  // to chunk coordinate 
  Vector3F worldPlayerCoords{ { playerLocation.X, playerLocation.Y, playerLocation.Z } };
  Chunk::Coords coords = Chunk::Coords::FromWorldCoords(worldPlayerCoords);
  
  constexpr Chunk::Coords::CoordType range = 3;
  Chunk::Coords coordsStart = coords - Chunk::Coords{ {range, range, range} };

  TArray<Chunk*> chunksToAdd;

  for (Chunk::Coords::CoordType z = 0; z < range*2; ++z)
  {
    if (z < 1)
    {
      continue;
    }
    for (Chunk::Coords::CoordType y = 0; y < range * 2; ++y)
    {
      for (Chunk::Coords::CoordType x = 0; x < range * 2; ++x)
      {
        if (chunksToAdd.Num() > 80)
        {
          z = 100;
          y = 100;
          break;
        }
        // make a vector
        Chunk::Coords chunkCoords = coordsStart + Chunk::Coords{ { x,y,z} };
        if (!_chunkInstances.Contains(chunkCoords))
        {
          
          Chunk* chunk = _mapManager.GetChunk(chunkCoords);
          if (!chunk->IsEmpty())
          {
            chunksToAdd.Add(chunk);
          }
          else
          {
            // dummy array so that we know we've seen this chunk
            _chunkInstances.FindOrAdd(chunkCoords);
          }
        }
      }
    }
  }
  size_t blocksAdded = 0;

  chunksToAdd.Sort([worldPlayerCoords](const Chunk& A, const Chunk& B)
  {
    return A.GetWorldMidpoint().DistanceTo(worldPlayerCoords) < B.GetWorldMidpoint().DistanceTo(worldPlayerCoords);
  });


  struct InstanceTransformAndChunks
  {
    TArray<FTransform> transforms;
    TArray<Chunk::Coords> chunks;
  };

  TMap<uint32, InstanceTransformAndChunks> newInstances;

  //TArray<int32> 

  // add chunk blocks to map
  for (Chunk* chunk : chunksToAdd)
  {
    if (blocksAdded > 1100)
    {
      break;
    }

    const Chunk::Coords pos = chunk->GetPosition();
    // create region info
    InstArray& instances = _chunkInstances.FindOrAdd(pos);

    
    //if (!_chunkInstances.Contains(pos))
    //{
    //  GEngine->AddOnScreenDebugMessage(-1, 7.f, FColor::Red, TEXT("Fatal error: position not correctly stored in map!"));
    //}

    for (size_t i = 0; i < World::BLOCKS_PER_CHUNK; ++i)
    {
      const Block& block = chunk->operator[](i);
      
      if (block.id > 0)
      {
        const World::Vector3<long long> blockInChunkCoords = World::Vector3<long long>::FromArrayOffset<World::CHUNK_SIZE>(i);
        const World::Vector3<long long> blockInWorldCoords = blockInChunkCoords + pos * (long long)World::CHUNK_SIZE;

        InstanceTransformAndChunks& newCoords = newInstances.FindOrAdd(block.id);

        const FVector location(100 * blockInWorldCoords[0], 100 * blockInWorldCoords[1], 100 * blockInWorldCoords[2]);
        FTransform t(location);
        newCoords.chunks.Add(pos);
        newCoords.transforms.Add(t);
        //instances.Add({ block.id, GetBlockStaticMesh(block.id)->AddInstance(t) });

        ++blocksAdded;
      }
    }
  }

  // add the blocks to meshes
  for (const auto& blockDataPair : newInstances)
  {
    UInstancedStaticMeshComponent* c = GetBlockStaticMesh(blockDataPair.Key);
    //TArray<int32> newIds;
    //newIds.Reserve(blockDataPair.Value.chunks.Num());
    TArray<int32> newIds = c->AddInstances(blockDataPair.Value.transforms, true);
    // add new instances to the index
    for (size_t i = 0; i < blockDataPair.Value.chunks.Num(); ++i)
    {
      InstArray& instances = _chunkInstances.FindOrAdd(blockDataPair.Value.chunks[i]);
      instances.Add({ blockDataPair.Key, newIds[i] });
    }
  }



  Vector3F locationAddedWorld = coords.ToWorldCoords();
  if (blocksAdded > 0)
  {
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Added blocks:") + FString::FromInt(blocksAdded));
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("around coords: [") + FString::SanitizeFloat(locationAddedWorld[0]) + "," + FString::SanitizeFloat(locationAddedWorld[1]) + "," + FString::SanitizeFloat(locationAddedWorld[2]) + "]");
  }

  int blocksRemoved = 0;
  // find chunks that are too far and unload them
  TArray<Chunk::Coords> toRemove;
  for (auto& pair : _chunkInstances)
  {
    // check distance to player
    float distance = pair.Key.ToWorldCenterPoint().DistanceTo(worldPlayerCoords);
    if (distance > RenderDistance * 100.0f * RenderDistance * 100.0f)
    {
      toRemove.Add(pair.Key);
      // remove the blocks
      for (const BlockInstInfo& block : pair.Value)
      {
        GetBlockStaticMesh(block.id)->MarkUnused(block.instId);

        ++blocksRemoved;
      }
    }
    if (toRemove.Num() > 5)
    {
      break;
    }
  }

  if (blocksRemoved > 0)
  {
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Removed blocks:") + FString::FromInt(blocksRemoved));
  }

  // remove the instance records
  for (const Chunk::Coords chunkRemoveCoords : toRemove)
  {
    _chunkInstances.Remove(chunkRemoveCoords);
  }
}

UInstancedStaticMeshWithReuse* ABlockGenerator::GetBlockStaticMesh(uint32 blockType)
{
  switch (blockType)
  {
  case 1: return BlocksMeshComponentGrass;
  case 2: return BlocksMeshComponentDirt;
  case 3: return BlocksMeshComponentRock;
  default: return BlocksMeshComponentRock;
  }
}
//
void ABlockGenerator::Tick(float DeltaSeconds)
{

}
