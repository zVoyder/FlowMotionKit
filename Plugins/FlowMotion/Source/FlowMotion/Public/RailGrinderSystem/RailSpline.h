// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInterface.h"
#include "RailSpline.generated.h"

UCLASS()
class FLOWMOTION_API ARailSpline : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rail Grind")
	bool bIsEnabled = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rail Grind|Mesh")
	UStaticMesh* MeshToUse;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rail Grind|Mesh")
	UMaterialInterface* MaterialOverride;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rail Grind|Mesh")
	TEnumAsByte<ESplineMeshAxis::Type> MeshForwardAxis = ESplineMeshAxis::X;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	FCollisionProfileName CollisionProfileName = UCollisionProfile::BlockAll_ProfileName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	TEnumAsByte<ECollisionEnabled::Type> CollisionEnabled = ECollisionEnabled::QueryAndPhysics;
	
private:
	UPROPERTY()
	USplineComponent* SplineComponent;
	UPROPERTY(Transient)
	TArray<USplineMeshComponent*> SplineMeshComponents;
	
public:
	ARailSpline();

	virtual void OnConstruction(const FTransform& Transform) override;

	USplineComponent* GetRailSplineComponent() const;
	
	float GetClosestDistanceOnSpline(const FVector& WorldLocation) const;
	
private:
	void GenerateMeshes();

	void ClearMeshes();
};
