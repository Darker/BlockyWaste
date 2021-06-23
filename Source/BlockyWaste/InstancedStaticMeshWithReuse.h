#pragma once

#ifdef __INTELLISENSE__

#define OVERRIDE_PLATFORM_HEADER_NAME ./
//#define PRAGMA_DISABLE_DEPRECATION_WARNINGS if 0
//#define PRAGMA_ENABLE_DEPRECATION_WARNINGS endif
#else
#endif

#include "CoreMinimal.h"

#include "Components/InstancedStaticMeshComponent.h"

#include "Containers/Array.h"

#include "InstancedStaticMeshWithReuse.generated.h"

UCLASS()
class UInstancedStaticMeshWithReuse : public UInstancedStaticMeshComponent
{
  GENERATED_BODY()

  using InstanceId = int32;
public:
  /** Add an instance to this component. Transform is given in local space of this component. */
  virtual int32 AddInstance(const FTransform& InstanceTransform) override;
  /** Add multiple instances to this component. Transform is given in local space of this component. */
  virtual TArray<int32> AddInstances(const TArray<FTransform>& InstanceTransforms, bool bShouldReturnIndices) override;
  //! Mark the instance as unused but keep it in the engine, leaving it to be collected later on
  virtual bool MarkUnused(InstanceId id);
  //! "Soft delete" by changing the instance transform to scale it to 0,0,0. This will not affect render state and will not be immediate
  virtual bool SoftDelete(InstanceId id);
  //! Add any lazy instances now
  //virtual void
private:
  //! Instance IDs that can be reused for new instances
  TArray<int32> _unusedIds;
  //! Instances that will be added by CommitInstances call.
  TArray<FTransform> _lazyInstances;
};

//inline UInstancedStaticMeshWithReuse::UInstancedStaticMeshWithReuse(class FObjectInitializer const& init) : UInstancedStaticMeshComponent(init) {}