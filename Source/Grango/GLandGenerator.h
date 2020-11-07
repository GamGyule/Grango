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
	int Width = 51;
	
	UPROPERTY(EditAnywhere)
	int Height = 51;

	UPROPERTY(EditAnywhere)
	int Scale = 100;

	UPROPERTY(EditAnywhere)
	FVector2D Offset;

	UPROPERTY(EditAnywhere)
	float MeshHeightMultiply = 13;

	UPROPERTY(EditAnywhere)
	UCurveFloat* HeightCurve;

	UFUNCTION()
	void CreateMesh(FLandMeshData MeshData, UTexture2D* Texture);

	UFUNCTION()
	void CreateNoiseMap();

	


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
