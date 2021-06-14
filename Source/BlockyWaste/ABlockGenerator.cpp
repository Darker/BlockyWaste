#include "ABlockGenerator.h"
#include "World/math/Vector3.h"

#include "Math/TransformNonVectorized.h"

ABlockGenerator::ABlockGenerator()
 : block(Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Game/Geometry/Meshes/1M_Cube.1M_Cube"))))
{


}
void ABlockGenerator::PostInitializeComponents()
{
	AActor::PostInitializeComponents();

}

void ABlockGenerator::BeginPlay()
{
	AActor::BeginPlay();

	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		for (int i = 0; i < 1000; ++i)
		{
			const World::Vector3<float> coords = World::Vector3<int>::FromArrayOffset(i).Convert<float>();
			FTransformation t(FVector(coords[0], coords[1], coords[2]));
			_blockMesh->AddInstance(t);
		}
		
			//const FRotator SpawnRotation = GetControlRotation();
			//// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			//const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

			////Set Spawn Collision Handling Override
			//FActorSpawnParameters ActorSpawnParams;
			//ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			//// spawn the projectile at the muzzle
			//World->SpawnActor<ABlockyWasteProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
	}
	


}
//
//void ABlockGenerator::Tick(float DeltaSeconds)
//{}
