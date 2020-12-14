// Copyright Epic Games, Inc. All Rights Reserved.
/*
	This file will be for the overall logic of the entire game.
*/

#pragma once

#include <map>
#include "Engine/TriggerVolume.h"
#include "DataManager.h"
#include "MainPlayer.h"
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MegaMiniGolfGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MEGAMINIGOLF_API AMegaMiniGolfGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
//protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;

private:
	// ** OUR ADVANCED DATA STRUCTURE **
	std::map<int, CourseNode*> courseMap; // this pointer will hold the address of the map recieved from DataManager
	// ** Current Map **
	int currentMap;

public:
	
	// ** Recieve Data Externally **
	AActor* dataManagerActor;
	AMainPlayer* mainPlayer;
	ACharacter* myCharacter;
	ADataManager* dataManager;

	// ** For Blueprint To Read **
	UPROPERTY(BlueprintReadWrite);
	int currentPar;
	UPROPERTY(BlueprintReadWrite);
	int currentStrokeCount;

protected:
	virtual void BeginPlay() override;
public:

	virtual void Tick(float DeltaTime) override;

	AMegaMiniGolfGameModeBase();
	bool bIsHoleComplete();

	// ** Game Logic **
	void gameLogic();
	void calculateScore();
	void transitionMap();
	bool bCanTransitionMap();

	// ** Recieve Data From Data Manager **
	void setBoundaryBox();
	void setCupBoundaryBox();
	void setManager(AActor* dataManager_set); // this is better than looping through each actor and trying to find it, this way the actor itself can tell the gamemode where it is
};
