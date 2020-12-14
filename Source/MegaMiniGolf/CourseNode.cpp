// Fill out your copyright notice in the Description page of Project Settings.

#include "CourseNode.h"

CourseNode::CourseNode()
{
	playerStrokeCount = 0; // playerStrokeCount is zero at the beggining since the game does not know the players anticipated score
}

CourseNode::~CourseNode()
{
}

void CourseNode::setBoundaryBox(ATriggerVolume* boundaryBoxSet)
{
	boundaryBox = boundaryBoxSet;
}

void CourseNode::setCupBoundaryBox(ATriggerVolume* cupBoundaryBoxSet)
{
	cupBoundaryBox = cupBoundaryBoxSet;
}

void CourseNode::setPar(int parSet)
{
	par = parSet;
}

void CourseNode::setMaxAmtStrokes(int maxAmtStrokesSet)
{
	maxAmtStrokes = maxAmtStrokesSet;
}

void CourseNode::setSpawnPoint(FVector spawnPointSet)
{
	spawnPoint = spawnPointSet;
}

void CourseNode::setStrokeCount(int strokeCount)
{
	playerStrokeCount = strokeCount;
}

ATriggerVolume* CourseNode::getBoundaryBox()
{
	return boundaryBox;
}

ATriggerVolume* CourseNode::getCupBoundaryBox()
{
	return cupBoundaryBox;
}

int CourseNode::getPar()
{
	return par;
}

int CourseNode::getMaxAmtStrokes()
{
	return maxAmtStrokes;
}

int CourseNode::getStrokeCount()
{
	return playerStrokeCount;
}

FVector CourseNode::getSpawnPoint()
{
	return spawnPoint;
}
