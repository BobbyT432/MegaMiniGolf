// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainPlayer.generated.h"

// ** Forward Declerations, it tells the program that we will use these later, don't mark as an error **
class USpringArmComponent;  // its okay to do this if we don't need to use the functions that come along with this, if we DO, then we need to include the respected header file
class UCameraComponent;
class UStaticMeshComponent;
class ATriggerVolume;

UCLASS()
class MEGAMINIGOLF_API AMainPlayer : public ACharacter
{
	GENERATED_BODY()

private:
	// ** Camera Data **
	bool panToggle;
	FVector2D mouseInput;

	// ** Putt Data **
	FVector powerLineEnd;
	FVector powerHit;
	FVector updatedPowerLocation;
	FVector boundedDistance;
	float totalDistance; // distance between ball and cursor
	UPROPERTY(EditAnywhere);
	float arrowDistance; // distance between ball and arrow
	bool bCanPutt;
	int strokeCount;

	// ** Projection Data **
	float cursor2DX;
	float cursor2DY;
	FVector cursorWorldLoc;
	FVector cursorWorldDir;
	FVector cursorWorldLocRel; // Relative to the player
	FVector cursorWorldDirRel;

	// ** Map Data ** 
	// we want data such as course par, boundary box, etc.
	// a vector will send information to the player and update them on the current status of the course
	ATriggerVolume* boundaryBox;
	ATriggerVolume* cupBoundaryBox;
	FVector puttLocation;

public:
	// Sets default values for this character's properties
	AMainPlayer();

	// ** Camera Data **
	UPROPERTY(EditAnywhere);
	float pitchDistance = 80.0f; // how high up and down a player can look
	UPROPERTY(EditAnywhere);
	float springArmLength = 400.0f;

	// ** Zoom Data **
	UPROPERTY(EditAnywhere);
	float zoomSpeed = 20.0f;
	UPROPERTY(EditAnywhere);
	float zoomMaxCap = 600.0f;
	UPROPERTY(EditAnywhere);
	float zoomMinCap = 100.0f;

	// ** The player **
	UPROPERTY(VisibleAnywhere, Category = "Components");
	USpringArmComponent* springArm;
	UPROPERTY(VisibleAnywhere, Category = "Components");
	UCameraComponent* camera;
	UPROPERTY(VisibleAnywhere, Category = "Components");
	UStaticMeshComponent* golfBall;
	UPROPERTY(VisibleAnywhere, Category = "Components"); // rotator ball is what the spring arm / camera is attached too and moves freely 
	UStaticMeshComponent* rotatorBall;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// ** Camera Functions **
	void cameraMovement();
	void panEngage();
	void panRelease();
	void yaw(float axis);
	void pitch(float axis);
	void zoomIn();
	void zoomOut();

	// ** Putt Functions **
	void renderCursor();
	void powerPutt();
	int getStrokeCount();
	void resetStrokeCount(); // when player transitions to next map, reset their stroke count

	// ** Player Functions **
	bool bIsPlayerMoving();
	bool bHasPlayerScored();

	// ** Map Functions **
	void correctPlayerOutsideMap();
	void setBoundaryBox(ATriggerVolume* boundaryBoxSet);
	void setCupBoundaryBox(ATriggerVolume* cupBoundaryBoxSet);
};
