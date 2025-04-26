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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rail Grind|Mesh")
	UStaticMesh* MeshToUse;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rail Grind|Mesh")
	UMaterialInterface* MaterialOverride;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rail Grind|Mesh")
	TEnumAsByte<ESplineMeshAxis::Type> MeshForwardAxis = ESplineMeshAxis::X;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rail Grind|Collision")
	FCollisionProfileName CollisionProfileName = UCollisionProfile::BlockAll_ProfileName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rail Grind|Collision")
	TEnumAsByte<ECollisionEnabled::Type> CollisionEnabled = ECollisionEnabled::QueryAndPhysics;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rail Grind|Debug")
	bool bShowDebug;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rail Grind|Debug", meta = (EditCondition = "bShowDebug"))
	float ArrowsLength = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rail Grind|Debug", meta = (EditCondition = "bShowDebug"))
	float ArrowsSize = 25.f;
#endif
	
private:
	UPROPERTY()
	USplineComponent* SplineComponent;
	UPROPERTY(Transient)
	TArray<USplineMeshComponent*> SplineMeshComponents;

public:
	ARailSpline();

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void Tick(float DeltaSeconds) override;
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual void PostEditMove(bool bFinished) override;

	virtual void PostEditUndo() override;

	virtual void PostDuplicate(EDuplicateMode::Type DuplicateMode) override;
#endif
	
private:
	/**
	 * @brief Creates the spline mesh components based on the spline points.
	 */
	void GenerateMeshes();

	/**
	 * @brief Clears the existing spline mesh components.
	 */
	void ClearMeshes();

#if WITH_EDITORONLY_DATA
	void DrawUpVectors() const;
#endif
};
