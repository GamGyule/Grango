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
	CreateNoiseMap();
}

void AGLandGenerator::CreateNoiseMap()
{
	auto* GGameInstance = Cast<UAdvancedGameInstance>(GetGameInstance());
	HeightCurve = GGameInstance->LandMeshCurve;
	TArray<FFloat2DMatrix> HeightMap = GGameInstance->GetNoiseMatrix(Width,Height,Scale,1,1,0,1,Offset);

	/*TArray<FFloat2DMatrix> NoiseMap = GGameInstance->GetNoiseMatrix(Width,Height,1,1,300,1,1,Offset);
	TArray<FFloat2DMatrix> RadialMap = GGameInstance->GetRadialGradient(Width,Height);
	TArray<FFloat2DMatrix> RadialHeightMap = GGameInstance->GetMixedRadialNoise(NoiseMap,RadialMap);*/
	
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
	LandMesh->SetCollisionProfileName(FName("Ground"));
	UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(Material,this);
	//DynamicMaterial->SetTextureParameterValue(TEXT("Texture"),Texture);
	LandMesh->SetMaterial(0,DynamicMaterial);
}







