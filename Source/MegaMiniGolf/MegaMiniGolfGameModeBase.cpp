// Copyright Epic Games, Inc. All Rights Reserved.


#include "MegaMiniGolfGameModeBase.h"
#include "MainPlayer.h"
#include "GameFramework/Character.h"

AMegaMiniGolfGameModeBase::AMegaMiniGolfGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;

	/*CourseNode holeOne;
	
	holeOne.setPar(parHoleOne);
	holeOne.setMaxAmtStrokes(maxAmtStrokesHoleOne);
	holeOne.setBoundaryBox(boundaryBoxHoleOne);
	holeOne.setCupBoundaryBox(cupBoundaryBoxHoleOne);
	holeOne.setSpawnPoint(spawnPointHoleOne);

	courseMap.insert(std::pair<int, CourseNode>(0, holeOne));
	*/
}

void AMegaMiniGolfGameModeBase::gameLogic()
{	

}

void AMegaMiniGolfGameModeBase::calculateScore()
{
	courseMap[currentMap]->setStrokeCount(mainPlayer->getStrokeCount()); // holds stroke count for this hole

	
	// can't use a switch statement unfortunately as the values are non-constant
	if (mainPlayer->getStrokeCount() == 1)
	{
		UE_LOG(LogTemp, Error, TEXT("HOLE IN ONE!"));
	}
	else if (mainPlayer->getStrokeCount() == (courseMap[currentMap]->getPar() - 1))
	{
		UE_LOG(LogTemp, Error, TEXT("BIRDIE!"));
	}
	else if (mainPlayer->getStrokeCount() == (courseMap[currentMap]->getPar() - 2))
	{
		UE_LOG(LogTemp, Error, TEXT("EAGLE!"));
	}
	else if (mainPlayer->getStrokeCount() == (courseMap[currentMap]->getPar() - 3))
	{
		UE_LOG(LogTemp, Error, TEXT("ALBATROSS!"));
	}
	else if (mainPlayer->getStrokeCount() == courseMap[currentMap]->getPar())
	{
		UE_LOG(LogTemp, Error, TEXT("PAR!"));
	}
	else if (mainPlayer->getStrokeCount() == courseMap[currentMap]->getPar() + 1)
	{
		UE_LOG(LogTemp, Error, TEXT("BOGEY!"));
	}
	else if (mainPlayer->getStrokeCount() == courseMap[currentMap]->getPar() + 2)
	{
		UE_LOG(LogTemp, Error, TEXT("DOUBLE BOGEY!"));
	}
	else if (mainPlayer->getStrokeCount() > courseMap[currentMap]->getPar() + 2)
	{
		UE_LOG(LogTemp, Error, TEXT("GOOD TRY!"));
	}
}

void AMegaMiniGolfGameModeBase::transitionMap()
{
	if (!bCanTransitionMap())
	{
		UE_LOG(LogTemp, Error, TEXT("Done!"));
	}
}

bool AMegaMiniGolfGameModeBase::bCanTransitionMap()
{
	if (courseMap[currentMap + 1] != nullptr)
	{
		currentMap++;
		mainPlayer->setBoundaryBox(courseMap[currentMap]->getBoundaryBox()); // transition to the next map
		mainPlayer->setCupBoundaryBox(courseMap[currentMap]->getCupBoundaryBox());
		mainPlayer->SetActorLocation(courseMap[currentMap]->getSpawnPoint());
		mainPlayer->resetStrokeCount();
		return 1;
	}
	return 0;
}

void AMegaMiniGolfGameModeBase::setManager(AActor* dataManager_set)
{
	dataManagerActor = dataManager_set;
}

void AMegaMiniGolfGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	// your character in the game should be AMainPlayer, however, you point to a AMainPlayer in a pointer of type ACharacter since 
	//AMainPlayer inherits from ACharacter when you are casting your pointer to another type with Cast it will fail if what you are 
	//casting from is not acually storing the provided template parameter type (or is inheriting from it)
	// Discord on why this works ---^
	// ** Setting up the Player **
	myCharacter = GetWorld()->GetFirstPlayerController()->GetCharacter(); // this has been a major issue for some reason, any project crashes? this is why (FIXED: IT WAS CRASHING BECAUSE I CALLED gameLogic() IN THE CONSTRUCTOR! THE WORLD ISNT MADE YET THERE!
	mainPlayer = CastChecked<AMainPlayer>(myCharacter);

	// ** Setting up the Data Manager **
	dataManager = CastChecked<ADataManager>(dataManagerActor); // takes the actor that has the class attached to it and casts the correct class to it so we can access it
	courseMap = dataManager->getMap(); // passes the map by reference
	
	currentPar = 0;
	currentMap = 0;
	mainPlayer->setBoundaryBox(courseMap[currentMap]->getBoundaryBox());
	mainPlayer->setCupBoundaryBox(courseMap[currentMap]->getCupBoundaryBox());
}

void AMegaMiniGolfGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (mainPlayer->bHasPlayerScored())
	{
		calculateScore();
		transitionMap();
	}
	// ** Update the HUD **
	currentPar = courseMap[currentMap]->getPar();
	currentStrokeCount = mainPlayer->getStrokeCount();
}
