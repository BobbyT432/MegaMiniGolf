// Fill out your copyright notice in the Description page of Project Settings.

#include "DataManager.h"
#include "MegaMiniGolfGameModeBase.h" // this is not circular dependency since we are calling it in cpp

// Sets default values
ADataManager::ADataManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ADataManager::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetAuthGameMode<AMegaMiniGolfGameModeBase>()->setManager(this); // tells the gamemode where it is

	// ** Populate Map **
	CourseNode* newHole = new CourseNode;
	newHole->setPar(parHoleOne);
	newHole->setMaxAmtStrokes(maxAmtStrokesHoleOne);
	newHole->setBoundaryBox(boundaryBoxHoleOne);
	newHole->setCupBoundaryBox(cupBoundaryBoxHoleOne);
	newHole->setSpawnPoint(spawnPointHoleOne->GetActorLocation());
	courseMap.insert({ 0, newHole });

	newHole = new CourseNode;
	newHole->setPar(parHoleTwo);
	newHole->setMaxAmtStrokes(maxAmtStrokesHoleTwo);
	newHole->setBoundaryBox(boundaryBoxHoleTwo);
	newHole->setCupBoundaryBox(cupBoundaryBoxHoleTwo);
	newHole->setSpawnPoint(spawnPointHoleTwo->GetActorLocation());
	courseMap.insert({ 1, newHole });

	newHole = new CourseNode;
	newHole->setPar(parHoleThree);
	newHole->setMaxAmtStrokes(maxAmtStrokesHoleThree);
	newHole->setBoundaryBox(boundaryBoxHoleThree);
	newHole->setCupBoundaryBox(cupBoundaryBoxHoleThree);
	newHole->setSpawnPoint(spawnPointHoleThree->GetActorLocation());
	courseMap.insert({ 2, newHole });

	newHole = new CourseNode;
	newHole->setPar(parHoleFour);
	newHole->setMaxAmtStrokes(maxAmtStrokesHoleFour);
	newHole->setBoundaryBox(boundaryBoxHoleFour);
	newHole->setCupBoundaryBox(cupBoundaryBoxHoleFour);
	newHole->setSpawnPoint(spawnPointHoleFour->GetActorLocation());
	courseMap.insert({ 3, newHole });
}

// Called every frame
void ADataManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

std::map<int, CourseNode*>& ADataManager::getMap()
{
	return courseMap;
}


