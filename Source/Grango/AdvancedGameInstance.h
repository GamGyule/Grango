// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdvancedFriendsGameInstance.h"
#include "GObjectDataStruct.h"
#include "ProceduralMeshComponent.h"
#include "Engine/DataTable.h"
#include "AdvancedGameInstance.generated.h"
/**
 * 
 */


USTRUCT()
struct FFloat2DMatrix
{
	GENERATED_BODY()
	TArray<float> arr;
};

USTRUCT(BlueprintType)
struct FLandType
{
	GENERATED_BODY()
private:
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Height;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor Color;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* Material;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* Object;
};

USTRUCT()
struct FLandMeshData
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FVector> vertices;
	UPROPERTY()
	TArray<int> triangles;
	UPROPERTY()
	TArray<FVector2D> uvs;
	

	FLandMeshData(){}


	FLandMeshData(int width, int height)
	{
		vertices.SetNum(width*height);
		triangles.SetNum((width-1)*(height-1)*6);
		uvs.SetNum(width*height);
	}
	
	void AddTriangle(int a, int b, int c)
	{
		triangles.Add(a);
		triangles.Add(b);
		triangles.Add(c);
	}
};
UENUM()
enum class ENormalizeMode
{
	Local,
	Global
};

UCLASS()
class GRANGO_API UAdvancedGameInstance : public UAdvancedFriendsGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	FObjectDataStruct GObjectData(FString GObjectName);

	UFUNCTION(BlueprintCallable)
	AActor* SpawnGObject( FString GObjectName);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* GObjectDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (titleProperty = "TerrainType"))
	TArray<FLandType> Regions;

	UPROPERTY(EditAnywhere)
	UCurveFloat* LandMeshCurve;

	UPROPERTY()
	ENormalizeMode normalizeMode = ENormalizeMode::Global;

	FORCEINLINE void ScreenMsg(const FString& Msg)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, *Msg);
	}

	UFUNCTION()
	TArray<FFloat2DMatrix> GetNoiseMatrix(int _MapWidth, int _MapHeight, float _Scale, float _Seed, int _Octaves, float _Persistance, float _Lacunarity, FVector2D _Offset);

	UFUNCTION()
	TArray<FFloat2DMatrix> GetRadialGradient(int _Width, int _Height);


	UFUNCTION()
	UTexture2D* GetNoiseHeightMap(TArray<FFloat2DMatrix> _HeightMap);

	UFUNCTION()
    UTexture2D* GetNoiseColorMap(TArray<FFloat2DMatrix> _HeightMap);

	UFUNCTION()
    FLandMeshData GetLandMeshData(TArray<FFloat2DMatrix> _HeightMap, UCurveFloat* CurveHeight);

	UFUNCTION()
	TArray<FFloat2DMatrix> GetMixedRadialNoise(TArray<FFloat2DMatrix> _Noise, TArray<FFloat2DMatrix> _Radial);
};
