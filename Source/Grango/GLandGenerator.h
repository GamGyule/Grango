// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "GameFramework/Actor.h"
#include "AdvancedGameInstance.h"
#include "GLandGenerator.generated.h"

UCLASS()
class GRANGO_API AGLandGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGLandGenerator();
	
	UPROPERTY()
	UProceduralMeshComponent* LandMesh;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* Material;

	UPROPERTY(EditAnywhere)
	int Width = 128;
	
	UPROPERTY(EditAnywhere)
	int Height = 128;

	UPROPERTY(EditAnywhere)
	float MeshHeightMultiply = 13;

	UPROPERTY(EditAnywhere)
	UCurveFloat* HeightCurve;

	UFUNCTION()
	void CreateMesh(FLandMeshData MeshData, UTexture2D* Texture);

	


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
