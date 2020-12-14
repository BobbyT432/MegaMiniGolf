// Fill out your copyright notice in the Description page of Project Settings.
/*
	This file is used to read data from the world, and has the gamemode read the data.
	We have to do this implementation because gamemode is not strictly in the world, so It can't read data
	when its not there. 

	This file also holds our Advanced Data Structure (Map).
*/
#pragma once

#include <map>
#include "CourseNode.h"
#include "Engine/TriggerVolume.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DataManager.generated.h"

class AMegaMiniGolfGameModeBase; // forward decleration to prevent a circular dependency error (this way we don't got to include the other file)

UCLASS()
class MEGAMINIGOLF_API ADataManager : public AActor
{
	GENERATED_BODY()
	
private:
	// ** OUR ADVANCED DATA STRUCTURE **
	std::map<int, CourseNode*> courseMap; // holds all information on the course data
	
	// ** Hole Data **
	int strokeCount; // stroke count that applies to each hole

	// -- Hole One --
	UPROPERTY(EditAnywhere, Category = "Hole One");
	int parHoleOne;
	UPROPERTY(EditAnywhere, Category = "Hole One");
	int maxAmtStrokesHoleOne;
	UPROPERTY(EditAnywhere, Category = "Hole One");
	ATriggerVolume* boundaryBoxHoleOne;
	UPROPERTY(EditAnywhere, Category = "Hole One");
	ATriggerVolume* cupBoundaryBoxHoleOne;
	UPROPERTY(EditAnywhere, Category = "Hole One");
	AActor* spawnPointHoleOne;

	// -- Hole Two --
	UPROPERTY(EditAnywhere, Category = "Hole Two");
	int parHoleTwo;
	UPROPERTY(EditAnywhere, Category = "Hole Two");
	int maxAmtStrokesHoleTwo;
	UPROPERTY(EditAnywhere, Category = "Hole Two");
	ATriggerVolume* boundaryBoxHoleTwo;
	UPROPERTY(EditAnywhere, Category = "Hole Two");
	ATriggerVolume* cupBoundaryBoxHoleTwo;
	UPROPERTY(EditAnywhere, Category = "Hole Two");
	AActor* spawnPointHoleTwo;

	// -- Hole Three --
	UPROPERTY(EditAnywhere, Category = "Hole Three");
	int parHoleThree;
	UPROPERTY(EditAnywhere, Category = "Hole Three");
	int maxAmtStrokesHoleThree;
	UPROPERTY(EditAnywhere, Category = "Hole Three");
	ATriggerVolume* boundaryBoxHoleThree;
	UPROPERTY(EditAnywhere, Category = "Hole Three");
	ATriggerVolume* cupBoundaryBoxHoleThree;
	UPROPERTY(EditAnywhere, Category = "Hole Three");
	AActor* spawnPointHoleThree;

	// -- Hole Four --
	UPROPERTY(EditAnywhere, Category = "Hole Four");
	int parHoleFour;
	UPROPERTY(EditAnywhere, Category = "Hole Four");
	int maxAmtStrokesHoleFour;
	UPROPERTY(EditAnywhere, Category = "Hole Four");
	ATriggerVolume* boundaryBoxHoleFour;
	UPROPERTY(EditAnywhere, Category = "Hole Four");
	ATriggerVolume* cupBoundaryBoxHoleFour;
	UPROPERTY(EditAnywhere, Category = "Hole Four");
	AActor* spawnPointHoleFour;
public:	
	// Sets default values for this actor's properties
	ADataManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	std::map<int, CourseNode*>& getMap();
};
