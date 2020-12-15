// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayer.h"
#include "GameFramework/SpringArmComponent.h" // can't do a forward decleration, we need its functions
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/MovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Engine/TriggerVolume.h"

// Sets default values
AMainPlayer::AMainPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	golfBall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GolfBall"));
	golfBall->SetupAttachment(RootComponent);

	rotatorBall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RotatorBall"));
	rotatorBall->SetupAttachment(RootComponent);

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm")); // creates a spring arm and returns the pointer to it
	springArm->SetupAttachment(rotatorBall); // attaches to parent
	springArm->TargetArmLength = springArmLength;

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(springArm, USpringArmComponent::SocketName); // attaches to spring arm

	// ** Defaults **
	boundedDistance = FVector(0, 0, 0);
	bCanPutt = true;
	arrowDistance = 225.0f;
	strokeCount = 0;
}

// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	cameraMovement();
}

// Called to bind functionality to input
void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAction("Pan", IE_Pressed, this, &AMainPlayer::panEngage); // Even though this is being made in BeginPlay(), "BindAction takes a callback and stores it in a list, when InputComponent detects relevant input it will call it you may not see the ticking but it's there"
	PlayerInputComponent->BindAction("Pan", IE_Released, this, &AMainPlayer::panRelease);
	PlayerInputComponent->BindAction("Putt", IE_Pressed, this, &AMainPlayer::powerPutt);
	PlayerInputComponent->BindAction("ZoomIn", IE_Pressed, this, &AMainPlayer::zoomIn);
	PlayerInputComponent->BindAction("ZoomOut", IE_Pressed, this, &AMainPlayer::zoomOut);
	PlayerInputComponent->BindAxis("MouseYaw", this, &AMainPlayer::yaw);
	PlayerInputComponent->BindAxis("MousePitch", this, &AMainPlayer::pitch);
}

void AMainPlayer::cameraMovement()
{
	// ** Camera Movement **
	if (panToggle) // right click must be held down
	{
		FRotator newYaw = rotatorBall->GetRelativeRotation(); // need to get the correct component I'm rotating (in this case its a static mesh inside the golf ball)
		newYaw.Yaw += mouseInput.X; // add to the yaw the direction (could be negative or positive)
		rotatorBall->SetRelativeRotation(newYaw);

		FRotator newPitch = springArm->GetRelativeRotation(); // the spring arm will only be used to edit pitch, that way you can't rotate the YAW and PITCH of the rotator ball, that'll mess up its axis
		newPitch.Pitch = FMath::Clamp(newPitch.Pitch + mouseInput.Y, -pitchDistance, pitchDistance); // clamp allows us to set a min / max value so that the camera can't exceed over the players head
		springArm->SetRelativeRotation(newPitch);
	}
	else if (!panToggle && !bIsPlayerMoving())
	{
		// ** Render everything about the ball **
		renderCursor();
	}
	correctPlayerOutsideMap();
}

void AMainPlayer::panEngage()
{
	panToggle = true;
}

void AMainPlayer::panRelease()
{
	panToggle = false;
	mouseInput.X = 0; // turn off speed
	mouseInput.Y = 0;
}

void AMainPlayer::yaw(float axis)
{
	mouseInput.X = axis; // set to the speed value of the mouse
}

void AMainPlayer::pitch(float axis)
{
	mouseInput.Y = axis;
}

void AMainPlayer::zoomIn()
{
	springArm->TargetArmLength = FMath::Clamp(springArm->TargetArmLength - zoomSpeed, zoomMinCap, zoomMaxCap);
}

void AMainPlayer::zoomOut()
{
	springArm->TargetArmLength = FMath::Clamp(springArm->TargetArmLength + zoomSpeed, zoomMinCap, zoomMaxCap);
}

void AMainPlayer::renderCursor()
{
	// ** Screen coordinate implementation **
		/* ** NOTE **
		DeprojectScreenPositionToWorld takes the 2D X,Y coordinates of your mouse cursor relative to your screen.
		It then projects from THAT point on your screen OUTWARD into 3D space.
		What we want to do is intersect that end point of the outward ray into a 3D plane.*/

	GetController<APlayerController>()->GetMousePosition(cursor2DX, cursor2DY);
	GetController<APlayerController>()->DeprojectScreenPositionToWorld
	(
		cursor2DX,
		cursor2DY,
		cursorWorldLoc,
		cursorWorldDir
	);

	updatedPowerLocation = golfBall->GetComponentLocation();
	updatedPowerLocation.Z -= 20;

	const FPlane PawnPlane(updatedPowerLocation, FVector::UpVector); // draw a plane around the player
	
	cursorWorldLoc = FMath::RayPlaneIntersection(cursorWorldLoc, cursorWorldDir, PawnPlane); // check for intersection on that plane casted from camera X, Y coordinates

	totalDistance = sqrt(pow(updatedPowerLocation.X - cursorWorldLoc.X, 2) + pow(updatedPowerLocation.Y - cursorWorldLoc.Y, 2)); // distance between arrow and ball

	// ** Arrow must have a limit **
	if (totalDistance > arrowDistance)
	{
		// https://math.stackexchange.com/questions/175896/finding-a-point-along-a-line-a-certain-distance-away-from-another-point
		FVector newVector = cursorWorldLoc - updatedPowerLocation;
		float vectorMagnitude = sqrt(pow(newVector.X, 2) + pow(newVector.Y, 2));
		FVector unitVector;
		unitVector.X = newVector.X / vectorMagnitude;
		unitVector.Y = newVector.Y / vectorMagnitude;

		cursorWorldLoc = updatedPowerLocation + (arrowDistance * unitVector);

		// ** Calculate new distance **
		totalDistance = sqrt(pow(updatedPowerLocation.X - cursorWorldLoc.X, 2) + pow(updatedPowerLocation.Y - cursorWorldLoc.Y, 2)); // distance between arrow and ball
	}
	
	// calculate how hard to hit the ball
	powerHit = cursorWorldLoc - updatedPowerLocation; // currently cursorWorldLoc is relative to world origin, this makes it relative to the ball
	//powerHit *= .5; // slow the hell down bro
	//totalDistance *= .5; // SLOW
	powerHit *= totalDistance; // the further the cursor is from the ball, higher the power hit is
	powerHit /= 9;
	powerHit.Z = 0;

	DrawDebugLine
	(
		GetWorld(),
		updatedPowerLocation,
		cursorWorldLoc, // 2D coordinates now represent a space in 3D space
		FColor(236, 21, 80),
		false,
		5.0f,
		1,
		4.0f
	);
}

void AMainPlayer::powerPutt()
{
	if (bCanPutt)
	{
		// ** Get Location Before They Hit **
		puttLocation = updatedPowerLocation;
		puttLocation.Z += 20;
		golfBall->AddImpulse(powerHit, TEXT("golfBall"), true);
		strokeCount++;
	}
}

int AMainPlayer::getStrokeCount()
{
	return strokeCount;
}

void AMainPlayer::resetStrokeCount()
{
	strokeCount = 0;
}

bool AMainPlayer::bIsPlayerMoving()
{
	float minVelo = 9.0f;
	
	if (abs(GetVelocity().X) < abs(minVelo) && abs(GetVelocity().Y) < abs(minVelo) && abs(GetVelocity().Z) < abs(minVelo))
	{
		bCanPutt = true;
		return false;
	}
	bCanPutt = false;
	return true;
}

bool AMainPlayer::bHasPlayerScored()
{
	if (!cupBoundaryBox)
	{
		UE_LOG(LogTemp, Error, TEXT("RUH OH! CUP BOUNDARY BOX NOT FOUND!"));
	}
	if ((cupBoundaryBox) && cupBoundaryBox->IsOverlappingActor(this))
	{
		return true;
	}
	return false;
}

void AMainPlayer::correctPlayerOutsideMap()
{
	if (!boundaryBox)
	{
		UE_LOG(LogTemp, Error, TEXT("RUH OH! BOUNDARY BOX NOT FOUND!"));
	}
	if ((boundaryBox) && !boundaryBox->IsOverlappingActor(this))
	{
		
		
		SetActorLocation(puttLocation);
	}
}

void AMainPlayer::setBoundaryBox(ATriggerVolume* boundaryBoxSet)
{
	boundaryBox = boundaryBoxSet;
}

void AMainPlayer::setCupBoundaryBox(ATriggerVolume* cupBoundaryBoxSet)
{
	cupBoundaryBox = cupBoundaryBoxSet;
}

