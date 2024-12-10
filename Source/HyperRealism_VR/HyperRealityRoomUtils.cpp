// Fill out your copyright notice in the Description page of Project Settings.
#include "HyperRealityRoomUtils.h"

// This function sorts the wall based on the position where the player is Standing.
// The player should stand close to the South wall for this to work. We determine the rotation of the room this way.

TArray<FTransform> UHyperRealityRoomUtils::SortWalls(const TArray<FTransform>& Walls, const FVector PlayerStartPosition, const FVector RoomCenter)
{
    TArray<FTransform> sortedList;

    // Temporary array to hold pairs of distance from room center and wall transforms
    TArray<TPair<float, FTransform>> distanceWallPairs;

    // Populate the distanceWallPairs array
    for (const FTransform& Wall : Walls)
    {
        float distance = FVector::Dist(Wall.GetLocation(), RoomCenter);
        distanceWallPairs.Add(TPair<float, FTransform>(distance, Wall));
    }

    // Sort the walls by distance from the room center in ascending order
    distanceWallPairs.Sort([](const TPair<float, FTransform>& A, const TPair<float, FTransform>& B)
        {
            return A.Key < B.Key; // Smaller distance comes first
        });

    // Separate larger walls (indices 0 and 1) and smaller walls (indices 2 and 3)
    FTransform LargeWallA = distanceWallPairs[0].Value;
    FTransform LargeWallB = distanceWallPairs[1].Value;
    FTransform SmallWallA = distanceWallPairs[2].Value;
    FTransform SmallWallB = distanceWallPairs[3].Value;

    // Determine North and South walls using player start position
    FVector SmallWallALocation = SmallWallA.GetLocation();
    FVector SmallWallBLocation = SmallWallB.GetLocation();

    float DistanceToSmallWallA = FVector::Dist(PlayerStartPosition, SmallWallALocation);
    float DistanceToSmallWallB = FVector::Dist(PlayerStartPosition, SmallWallBLocation);

    FTransform NorthWall = (DistanceToSmallWallA > DistanceToSmallWallB) ? SmallWallA : SmallWallB;
    FTransform SouthWall = (DistanceToSmallWallA > DistanceToSmallWallB) ? SmallWallB : SmallWallA;

    // Determine East and West walls using the room center and the North wall's direction
    FVector NorthWallLocation = NorthWall.GetLocation();
    FVector NorthToCenterVector = (RoomCenter - NorthWallLocation).GetSafeNormal();

    FVector LargeWallALocation = LargeWallA.GetLocation();
    FVector LargeWallBLocation = LargeWallB.GetLocation();

    // Project large wall locations onto the vector orthogonal to NorthToCenterVector
    FVector RightVector = NorthToCenterVector.RotateAngleAxis(90, FVector::UpVector);
    float DotProductA = FVector::DotProduct(RightVector, LargeWallALocation - RoomCenter);
    float DotProductB = FVector::DotProduct(RightVector, LargeWallBLocation - RoomCenter);

    FTransform EastWall = (DotProductA > DotProductB) ? LargeWallB : LargeWallA;
    FTransform WestWall = (DotProductA > DotProductB) ? LargeWallA : LargeWallB;

    // Add them in a clockwise order: North -> East -> South -> West
    sortedList.Empty();
    sortedList.Add(NorthWall);
    sortedList.Add(EastWall);
    sortedList.Add(SouthWall);
    sortedList.Add(WestWall);

    return sortedList;
}