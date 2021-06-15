#include "ABlockGenerator.h"

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
  BlocksMeshComponentRock = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Instanced blocks mesh Rock"));
	BlocksMeshComponentRock->RegisterComponent();
  BlocksMeshComponentDirt = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Instanced blocks mesh Dirt"));
  BlocksMeshComponentDirt->RegisterComponent();
  BlocksMeshComponentGrass = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Instanced blocks mesh Grass"));
  BlocksMeshComponentGrass->RegisterComponent();

	//RootComponent = BlocksMeshComponent;
}
void ABlockGenerator::PostInitializeComponents()
{
	AActor::PostInitializeComponents();
}

void ABlockGenerator::BeginPlay()
{
	AActor::BeginPlay();
  GetWorldTimerManager().SetTimer(ChunkLoadTimer, this, &ABlockGenerator::PopulateNearPlayer, 0.5f, true, 2.0f);
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
  GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player: [") + FString::SanitizeFloat(playerLocation.X) + "," + FString::SanitizeFloat(playerLocation.Y) + "," + FString::SanitizeFloat(playerLocation.Z) + "]");
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
        if (chunksToAdd.Num() > 10)
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
    return A.GetWorldMidpoint().DistanceTo(worldPlayerCoords) > B.GetWorldMidpoint().DistanceTo(worldPlayerCoords);
  });
  // add chunk blocks to map
  for (Chunk* chunk : chunksToAdd)
  {
    if (blocksAdded > 5000)
    {
      break;
    }

    const Chunk::Coords pos = chunk->GetPosition();
    // create region info
    InstArray& instances = _chunkInstances.FindOrAdd(pos);
    if (!_chunkInstances.Contains(pos))
    {
      GEngine->AddOnScreenDebugMessage(-1, 7.f, FColor::Red, TEXT("Fatal error: position not correctly stored in map!"));
    }

    for (size_t i = 0; i < World::BLOCKS_PER_CHUNK; ++i)
    {
      const Block& block = chunk->operator[](i);
      
      if (block.id > 0)
      {
        const World::Vector3<long long> blockInChunkCoords = World::Vector3<long long>::FromArrayOffset<World::CHUNK_SIZE>(i);
        const World::Vector3<long long> blockInWorldCoords = blockInChunkCoords + pos * (long long)World::CHUNK_SIZE;

        const FVector location(100 * blockInWorldCoords[0], 100 * blockInWorldCoords[1], 100 * blockInWorldCoords[2]);
        FTransform t(location);
        instances.Add({ block.id, GetBlockStaticMesh(block.id)->AddInstance(t) });

        ++blocksAdded;
      }
    }
  }
  Vector3F locationAddedWorld = coords.ToWorldCoords();
  GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Added blocks:") + FString::FromInt(blocksAdded));
  GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("around coords: [") + FString::SanitizeFloat(locationAddedWorld[0]) + "," + FString::SanitizeFloat(locationAddedWorld[1]) + "," + FString::SanitizeFloat(locationAddedWorld[2]) + "]");
}

UInstancedStaticMeshComponent* ABlockGenerator::GetBlockStaticMesh(uint32 blockType)
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
