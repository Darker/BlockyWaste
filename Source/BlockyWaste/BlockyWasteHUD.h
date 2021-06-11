// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BlockyWasteHUD.generated.h"

UCLASS()
class ABlockyWasteHUD : public AHUD
{
	GENERATED_BODY()

public:
	ABlockyWasteHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

