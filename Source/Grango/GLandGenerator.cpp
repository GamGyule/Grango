// Fill out your copyright notice in the Description page of Project Settings.


#include "GLandGenerator.h"

#include "AdvancedGameInstance.h"

// Sets default values
AGLandGenerator::AGLandGenerator()
{
	LandMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("LandMesh"));
	LandMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AGLandGenerator::BeginPlay()
{
	Super::BeginPlay();

	auto* GGameInstance = Cast<UAdvancedGameInstance>(GetGameInstance());
	TArray<FFloat2DMatrix> HeightMap = GGameInstance->GetNoiseMatrix(Width,Height,50,1,1,1,1);
	FLandMeshData MeshData = GGameInstance->GetLandMeshData(HeightMap,HeightCurve);
	UTexture2D* Texture = GGameInstance->GetNoiseColorMap(HeightMap);
	CreateMesh(MeshData,Texture);
}

void AGLandGenerator::CreateMesh(FLandMeshData MeshData, UTexture2D* Texture)
{
	LandMesh->bUseComplexAsSimpleCollision = true;
	LandMesh->CreateMeshSection(0,MeshData.vertices,MeshData.triangles,TArray<FVector>(),MeshData.uvs,TArray<FColor>(),TArray<FProcMeshTangent>(),true);
	LandMesh->SetCollisionConvexMeshes({MeshData.vertices});
	LandMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(Material,this);
	DynamicMaterial->SetTextureParameterValue(TEXT("Texture"),Texture);
	LandMesh->SetMaterial(0,DynamicMaterial);
	
	
}





