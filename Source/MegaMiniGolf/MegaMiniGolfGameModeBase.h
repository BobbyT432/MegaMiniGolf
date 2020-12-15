// Copyright Epic Games, Inc. All Rights Reserved.
/*
	Bobby Templin / Keagen Brendle MegaMiniGolf

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
//UENUM()
//enum class Score : uint8
//{
//	CLEAR,
//	HOLEINONE,
//	BIRDIE,
//	EAGLE,
//	ALBATROSS,
//	PAR,
//	BOGEY,
//	DOUBLEBOGEY
//};

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
	float lastScoreRecieved; // keeps track of the time since the last score was updated
	float scoreDisperseDelay; // how long of a delay until the score goes away
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
	UPROPERTY(BlueprintReadWrite);
	int currentScore; // wish I could've used an enum for this but Unreal has a weird way of implementing that
	UPROPERTY(BlueprintReadWrite);
	bool bIsGameOver;
	UPROPERTY(BlueprintReadWrite);
	int holeOnePlayerPar;
	UPROPERTY(BlueprintReadWrite);
	int holeTwoPlayerPar;
	UPROPERTY(BlueprintReadWrite);
	int holeThreePlayerPar;
	UPROPERTY(BlueprintReadWrite);
	int holeFourPlayerPar;
	UPROPERTY(BlueprintReadWrite);
	int holeFivePlayerPar;
	UPROPERTY(BlueprintReadWrite);
	int playerSumPar;
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

	// ** FIVE TEST CASES PERFORMED IN HERE **
	void testCases();
	void testCaseOne();
	void testCaseTwo();
	void testCaseThree();
	void testCaseFour();
	void testCaseFive();
};
