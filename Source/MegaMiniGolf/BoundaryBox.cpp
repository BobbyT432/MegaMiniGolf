// Fill out your copyright notice in the Description page of Project Settings.


#include "BoundaryBox.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UBoundaryBox::UBoundaryBox()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// ...
}


// Called when the game starts
void UBoundaryBox::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void UBoundaryBox::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	if (character)
	{
		if (GetOwner()->IsOverlappingActor(character))
		{
			UE_LOG(LogTemp, Error, TEXT("TEST"));
		}

	}
	// ...
}

