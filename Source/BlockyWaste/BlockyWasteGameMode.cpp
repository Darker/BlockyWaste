// Copyright Epic Games, Inc. All Rights Reserved.

#include "BlockyWasteGameMode.h"
#include "BlockyWasteHUD.h"
#include "BlockyWasteCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABlockyWasteGameMode::ABlockyWasteGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ABlockyWasteHUD::StaticClass();
}

//void ABlockyWasteGameMode::StartPlay()
//{
//	AGameModeBase::StartPlay();
//
//	UE_LOG(LogTemp, Warning, TEXT("Hello"));
//
//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("This message will appear on the screen!"));
//
//}

