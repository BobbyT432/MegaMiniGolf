// Fill out your copyright notice in the Description page of Project Settings.
/*
	This file is used to hold information for each hole. This will be constructed n times (n amount of holes) and placed into a map
*/
#pragma once

#include "Engine/TriggerVolume.h"
#include "CoreMinimal.h"

class MEGAMINIGOLF_API CourseNode
{
private:
	// ** Hole Data **
	int par;
	int maxAmtStrokes; // how many strokes before transitioning to next map
	int playerStrokeCount; // strokes for that hole
	ATriggerVolume* boundaryBox; // this will be set in the editor, but referenced in C++ in other files
	ATriggerVolume* cupBoundaryBox;
	FVector spawnPoint;

public:
	CourseNode();
	~CourseNode();

	// ** Hole Functions **

	// ** Setters **
	void setBoundaryBox(ATriggerVolume* boundaryBoxSet);
	void setCupBoundaryBox(ATriggerVolume* cupBoundaryBoxSet);
	void setPar(int parSet);
	void setMaxAmtStrokes(int maxAmtStrokesSet);
	void setSpawnPoint(FVector spawnPointSet);
	void setStrokeCount(int strokeCount);

	// ** Getters **
	ATriggerVolume* getBoundaryBox();
	ATriggerVolume* getCupBoundaryBox();
	int getPar();
	int getMaxAmtStrokes();
	int getStrokeCount();
	FVector getSpawnPoint();
};
