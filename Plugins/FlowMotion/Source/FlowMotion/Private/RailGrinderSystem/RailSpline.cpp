﻿// Copyright VUEDK, Inc. All Rights Reserved.

#include "RailGrinderSystem/RailSpline.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Engine/World.h"

ARailSpline::ARailSpline()
{
	PrimaryActorTick.bCanEverTick = true;
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	RootComponent = SplineComponent;
}

void ARailSpline::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	GenerateMeshes();
}

void ARailSpline::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
#if WITH_EDITORONLY_DATA
	DrawUpVectors();
#endif
}

#if WITH_EDITOR
void ARailSpline::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	GenerateMeshes();
}

void ARailSpline::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);
	if (!bFinished) return;
	GenerateMeshes();
}

void ARailSpline::PostEditUndo()
{
	Super::PostEditUndo();
	GenerateMeshes();
}

void ARailSpline::PostDuplicate(EDuplicateMode::Type DuplicateMode)
{
	Super::PostDuplicate(DuplicateMode);
	GenerateMeshes();
}
#endif

void ARailSpline::GenerateMeshes()
{
	ClearMeshes();

	if (!MeshToUse || !SplineComponent) return;
	
	for (int32 i = 0; i < SplineComponent->GetNumberOfSplinePoints() - 1; ++i)
	{
		USplineMeshComponent* SplineMesh = NewObject<USplineMeshComponent>(this);
		SplineMesh->SetStaticMesh(MeshToUse);
		SplineMesh->SetForwardAxis(MeshForwardAxis, true);
		
		if (MaterialOverride)
			SplineMesh->SetMaterial(0, MaterialOverride);

		SplineMesh->SetMobility(RootComponent->Mobility);
		SplineMesh->RegisterComponentWithWorld(GetWorld());
		SplineMesh->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);

		FVector StartPos, StartTangent, EndPos, EndTangent;
		SplineComponent->GetLocationAndTangentAtSplinePoint(i, StartPos, StartTangent, ESplineCoordinateSpace::Local);
		SplineComponent->GetLocationAndTangentAtSplinePoint(i + 1, EndPos, EndTangent, ESplineCoordinateSpace::Local);

		FVector UpVector = SplineComponent->GetUpVectorAtSplinePoint(i, ESplineCoordinateSpace::Local);
		SplineMesh->SetSplineUpDir(UpVector, ESplineCoordinateSpace::Local);

		SplineMesh->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent);
		
		SplineMesh->SetCollisionProfileName(CollisionProfileName.Name);
		SplineMesh->SetCollisionEnabled(CollisionEnabled);
		
		SplineMeshComponents.Add(SplineMesh);
	}
}

void ARailSpline::ClearMeshes()
{
	TArray<USceneComponent*> SplineChildren = SplineComponent->GetAttachChildren();

	for (USceneComponent* Child : SplineChildren)
	{
		if (USplineMeshComponent* Mesh = Cast<USplineMeshComponent>(Child))
			Mesh->DestroyComponent();
	}

	SplineMeshComponents.Empty();
}

#if WITH_EDITORONLY_DATA
void ARailSpline::DrawUpVectors() const
{
	if (!bShowDebug || !SplineComponent) return;
	
	for (int32 i = 0; i < SplineComponent->GetNumberOfSplinePoints(); ++i)
	{
		FVector UpVector = SplineComponent->GetUpVectorAtSplinePoint(i, ESplineCoordinateSpace::World);
		FVector StartPoint = SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World);
		FVector EndPoint = StartPoint + UpVector * ArrowsLength;
		DrawDebugDirectionalArrow(GetWorld(), StartPoint, EndPoint, ArrowsSize, FColor::Red, false, -1.f, 0, 2.f);
	}
}
#endif
