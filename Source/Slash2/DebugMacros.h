#pragma once

#include "DrawDebugHelpers.h"

#define DRAW_SPHERE(Location) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 50, 12, FColor::Red, true, -1, 0, 1);
#define PRINTLOG(Message) UE_LOG(LogTemp, Warning, TEXT(Message))
#define DRAW_SPHERE_COLOR_TEMP(Location, Color, Duration) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 15, 12, Color, false, Duration, 0, 1);
#define DRAW_LINE(StartLocation, Endlocation) if (GetWorld()) DrawDebugLine(GetWorld(), StartLocation, Endlocation, FColor::Green, true);
#define DRAW_POINT(Location) if (GetWorld()) DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Green, true, -1, 0);
#define DRAW_VECTOR(StartLocation, EndLocation) if (GetWorld()) \
{ \
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Green, true, -1.f, 0, 1); \
	DrawDebugPoint(GetWorld(), EndLocation, 12.f, FColor::Green, true); \
}
#define DRAW_SPHERE_CUSTOM(Location, Color) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 50, 12, Color, true);
#define DRAW_BOX(Location) if (GetWorld()) DrawDebugBox(GetWorld(), Location, FVector(25, 25, 25), FColor::Green, true, -1, 0, 1);
#define DRAW_SPHERE_FRAME(Location) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 50, 12, FColor::Red, false, 0, -1.f, 1);
#define DRAW_LINE_FRAME(StartLocation, Endlocation) if (GetWorld()) DrawDebugLine(GetWorld(), StartLocation, Endlocation, FColor::Green, false, -1.f, 0, 1);
#define DRAW_POINT_FRAME(Location) if (GetWorld()) DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Green, false, -1.f, 0);
#define DRAW_VECTOR_FRAME(StartLocation, EndLocation) if (GetWorld()) \
{ \
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Green, false, -1.f, 0, 1); \
	DrawDebugPoint(GetWorld(), EndLocation, 12.f, FColor::Green, false, -1.f); \
}