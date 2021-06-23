#include "InstancedStaticMeshWithReuse.h"
#include <memory>
#include <algorithm>

int32 UInstancedStaticMeshWithReuse::AddInstance(const FTransform& InstanceTransform)
{
  if (_unusedIds.Num() > 0)
  {
    InstanceId id = _unusedIds[_unusedIds.Num() - 1];
    _unusedIds.RemoveAt(_unusedIds.Num() - 1);
    UInstancedStaticMeshComponent::UpdateInstanceTransform(id, InstanceTransform, false, true, true);
    return id;
  }
  else
  {
    return UInstancedStaticMeshComponent::AddInstance(InstanceTransform);
  }
}

TArray<int32> UInstancedStaticMeshWithReuse::AddInstances(const TArray<FTransform>& InstanceTransforms, bool bShouldReturnIndices)
{
  const TArray<FTransform>* toBeAddedPtr = &InstanceTransforms;
  TArray<FTransform> toBeAddedFiltered;
  TArray<InstanceId> returnIndices;
 
  const int32 unusedIndiceCount = _unusedIds.Num();

  // we sadly must sort the array, othverwise we're not able to use batch updates
  _unusedIds.Sort();

  // see what instances can be reused
  if (unusedIndiceCount > 0)
  {
    const bool unusedIndicesCoverAll = InstanceTransforms.Num() <= unusedIndiceCount;
    //InstanceId lastId = _unusedIds[unusedIndiceCount - 1];
    TArray<FTransform> pendingChanges;
    // loop from the back, that's much nicer place to remove items from
    for (int32 i = unusedIndiceCount - 1; i > -1; --i)
    {
      const int32 instanceTransformIndex = unusedIndiceCount - i - 1;
      if (instanceTransformIndex >= InstanceTransforms.Num())
        break;

      InstanceId id = _unusedIds[i];
      if(bShouldReturnIndices)
        returnIndices.Add(id);

      const bool markDirty = i == 0 && unusedIndicesCoverAll;
      pendingChanges.Add(InstanceTransforms[instanceTransformIndex]);

      if (i == 0 || _unusedIds[i - 1] != id - 1)
      {
        BatchUpdateInstancesTransforms(id, pendingChanges, false, markDirty, true);
        pendingChanges.Empty();
      }
    }

    if (unusedIndicesCoverAll)
    {
      if (unusedIndiceCount > InstanceTransforms.Num())
      {
        _unusedIds.RemoveAt(unusedIndiceCount - InstanceTransforms.Num(), InstanceTransforms.Num(), false);
      }
        
      return returnIndices;
    }
    else
    {
      _unusedIds.Empty();
      // add remaining transforms to filtered list
      for (int32 i = unusedIndiceCount; i < InstanceTransforms.Num(); ++i)
      {
        toBeAddedFiltered.Add(InstanceTransforms[i]);
      }
      toBeAddedPtr = &toBeAddedFiltered;
    }
  }

  const TArray<FTransform>& toBeAdded = *toBeAddedPtr;
  // if the function reaches this point, unused indices did not suffice to add all items
  TArray<InstanceId> addedIds = UInstancedStaticMeshComponent::AddInstances(toBeAdded, bShouldReturnIndices);

  if (returnIndices.Num() > 0)
  {
    returnIndices.Append(addedIds);
    return returnIndices;
  }
  else
  {
    return addedIds;
  }
}

bool UInstancedStaticMeshWithReuse::MarkUnused(InstanceId id)
{
  if (!_unusedIds.Contains(id))
  {
    _unusedIds.Add(id);
    return true;
  }
  return false;
}

bool UInstancedStaticMeshWithReuse::SoftDelete(InstanceId id)
{
  FTransform oldTransform;
  if (!GetInstanceTransform(id, oldTransform, false))
  {
    return false;
  }

  UpdateInstanceTransform(id, oldTransform, false, false, true);
  return MarkUnused(id);
}
