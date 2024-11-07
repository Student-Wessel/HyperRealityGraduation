// Fill out your copyright notice in the Description page of Project Settings.
#include "HyperRealityRoomUtils.h"

// This function sorts the wall based on the position where the player is looking.
// The room requires to be rectangluar and it expect 4 walls. If it has more than 4 walls this will crash.
// It will always assign one of the smaller walls north and the other south, it does that based on the direction the player is looking.
// The wall the player is looking at will be the north wall.
TArray<FTransform> UHyperRealityRoomUtils::SortWalls(const TArray<FTransform>& Walls, const FVector PlayerForward, const FVector RoomCenter)
{
    TArray<FTransform> sortedList;

    // Temporary array to hold pairs of distance and wall transforms
    TArray<TPair<float, FTransform>> distanceWallPairs;

    // Populate the distanceWallPairs array
    for (const FTransform& Wall : Walls)
    {
        // Calculate the distance between the wall's location and the room center's location
        float distance = FVector::Dist(Wall.GetLocation(), RoomCenter);

        // Add the distance and corresponding wall transform to the array
        distanceWallPairs.Add(TPair<float, FTransform>(distance, Wall));
    }

    // Sort the array by distance in descending order
    distanceWallPairs.Sort([](const TPair<float, FTransform>& A, const TPair<float, FTransform>& B)
        {
            return A.Key > B.Key; // Sort in descending order
        });

    // Populate sortedList with sorted walls
    for (const TPair<float, FTransform>& pair : distanceWallPairs)
    {
        sortedList.Add(pair.Value); // Add the wall transform to the sorted list
    }

    // Get the wall forward vectors for index 0 and 1
    FVector WallA = sortedList[0].GetRotation().Vector();
    FVector WallB = sortedList[1].GetRotation().Vector();

    // Calculate the dot products to determine the North wall using the player's forward vector
    float DotProduct0 = FVector::DotProduct(PlayerForward, WallA);
    float DotProduct1 = FVector::DotProduct(PlayerForward, WallB);

    // Determine North and South walls
    FTransform NorthWall = (DotProduct0 > DotProduct1) ? sortedList[0] : sortedList[1];
    FTransform SouthWall = (DotProduct0 > DotProduct1) ? sortedList[1] : sortedList[0]; // No need to recalculate dot product

    // Get the wall forward vectors for index 2 and 3
    FVector WallC = sortedList[2].GetRotation().Vector();
    FVector WallD = sortedList[3].GetRotation().Vector();

    // Get the forward vector of the North Wall to determine East and West walls
    FVector NorthWallForwardVector = NorthWall.GetRotation().Vector();

    // Calculate the dot products with NorthWall's right vector (rotated by 90 degrees)
    FVector NorthRightVector = NorthWallForwardVector.RotateAngleAxis(90, FVector::UpVector);
    float DotProductEast = FVector::DotProduct(NorthRightVector, WallC);
    float DotProductWest = FVector::DotProduct(NorthRightVector, WallD);

    // Determine East and West walls
    FTransform EastWall = (DotProductEast > DotProductWest) ? sortedList[2] : sortedList[3];
    FTransform WestWall = (DotProductEast > DotProductWest) ? sortedList[3] : sortedList[2]; // No need to recalculate

    // Now you have NorthWall, SouthWall, EastWall, and WestWall

    // Add them in a clockwise order
    sortedList.Empty();
    sortedList.Add(NorthWall);
    sortedList.Add(EastWall);
    sortedList.Add(SouthWall);
    sortedList.Add(WestWall);

    return sortedList;
}

