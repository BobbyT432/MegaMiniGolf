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
	if (mainPlayer->bHasPlayerScored())
	{
		calculateScore();
		transitionMap();
	}
	// ** Update the HUD **
	currentPar = courseMap[currentMap]->getPar();
	currentStrokeCount = mainPlayer->getStrokeCount();

	if ((GetWorld()->GetTimeSeconds() - lastScoreRecieved) >= scoreDisperseDelay)
	{
		currentScore = 0;
	}
}

void AMegaMiniGolfGameModeBase::calculateScore()
{
	courseMap[currentMap]->setStrokeCount(mainPlayer->getStrokeCount()); // holds stroke count for this hole
	
	// can't use a switch statement unfortunately as the values are non-constant
	if (mainPlayer->getStrokeCount() == 1)
	{
		currentScore = 1; // Hole in One
	}
	else if (mainPlayer->getStrokeCount() == (courseMap[currentMap]->getPar() - 1))
	{
		currentScore = 2; // Birdie
	}
	else if (mainPlayer->getStrokeCount() == (courseMap[currentMap]->getPar() - 2))
	{
		currentScore = 3; // Eagle
	}
	else if (mainPlayer->getStrokeCount() == (courseMap[currentMap]->getPar() - 3))
	{
		currentScore = 4; // Albatross
	}
	else if (mainPlayer->getStrokeCount() == courseMap[currentMap]->getPar())
	{
		currentScore = 5; // Par
	}
	else if (mainPlayer->getStrokeCount() == courseMap[currentMap]->getPar() + 1)
	{
		currentScore = 6; // Bogey
	}
	else if (mainPlayer->getStrokeCount() == courseMap[currentMap]->getPar() + 2)
	{
		currentScore = 7; // Double Bogey
	}
	else if (mainPlayer->getStrokeCount() > courseMap[currentMap]->getPar() + 2)
	{
		UE_LOG(LogTemp, Error, TEXT("GOOD TRY!"));
	}
	lastScoreRecieved = GetWorld()->GetTimeSeconds();
}

void AMegaMiniGolfGameModeBase::transitionMap()
{
	if (!bCanTransitionMap())
	{
		bIsGameOver = true;
		
		// ** Update for the score card **
		holeOnePlayerPar = courseMap[0]->getStrokeCount();
		holeTwoPlayerPar = courseMap[1]->getStrokeCount();
		holeThreePlayerPar = courseMap[2]->getStrokeCount();
		holeFourPlayerPar = courseMap[3]->getStrokeCount();
		holeFivePlayerPar = courseMap[4]->getStrokeCount();
		playerSumPar = holeOnePlayerPar + holeTwoPlayerPar + holeThreePlayerPar + holeFourPlayerPar + holeFivePlayerPar;
	}
}

bool AMegaMiniGolfGameModeBase::bCanTransitionMap()
{
	if (courseMap[currentMap + 1] != nullptr) // if the next map is available, run the code below
	{
		//currentScore = 0;
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

void AMegaMiniGolfGameModeBase::testCases()
{
	// For each map we perform a different test
	switch (currentMap)
	{
	case 1: testCaseOne();
		break;
	case 2: testCaseTwo();
		break;
	case 3: testCaseThree();
		break;
	case 4: testCaseFour();
		break;
	case 5: testCaseFive();
		break;
	}
}

void AMegaMiniGolfGameModeBase::testCaseOne()
{
	UE_LOG(LogTemp, Display, TEXT("** Test Case One: Ensure that stroke count matches HUD **"));
	UE_LOG(LogTemp, Display, TEXT("---- Players Actual Stroke Count: %d"), mainPlayer->getStrokeCount());
}

void AMegaMiniGolfGameModeBase::testCaseTwo()
{
	UE_LOG(LogTemp, Display, TEXT("** Test Case Two: Ensure that current PAR matches the correct one on the HUD **"));
	UE_LOG(LogTemp, Display, TEXT("---- Maps Actual Par: %d"), courseMap[currentMap]->getPar());
}

void AMegaMiniGolfGameModeBase::testCaseThree()
{
	UE_LOG(LogTemp, Display, TEXT("** Test Case Three: Ensure that the players velocity is working below (not zeroed)**"));
	UE_LOG(LogTemp, Display, TEXT("---- Players Velocity: %s -----"), *mainPlayer->GetVelocity().ToString());
}

void AMegaMiniGolfGameModeBase::testCaseFour()
{
	UE_LOG(LogTemp, Display, TEXT("** Test Case Four: Ensure that the players location is being updated (used for many cases) **"));
	UE_LOG(LogTemp, Display, TEXT("---- Players Location: %s"), *mainPlayer->GetActorLocation().ToString());
}

void AMegaMiniGolfGameModeBase::testCaseFive()
{
	UE_LOG(LogTemp, Display, TEXT("** Test Case Five: Ensure that the game outputs 'DONE' to output log when finished **"));
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
	lastScoreRecieved = 0;
	scoreDisperseDelay = 5;
	bIsGameOver = false;
	holeOnePlayerPar = 0;
	holeTwoPlayerPar = 0;
	holeThreePlayerPar = 0;
	holeFourPlayerPar = 0;
	holeFivePlayerPar = 0;
	playerSumPar = 0;
	mainPlayer->setBoundaryBox(courseMap[currentMap]->getBoundaryBox());
	mainPlayer->setCupBoundaryBox(courseMap[currentMap]->getCupBoundaryBox());
}

void AMegaMiniGolfGameModeBase::Tick(float DeltaTime) // main game loop
{
	Super::Tick(DeltaTime);
	
	gameLogic(); 

	testCases();
}
