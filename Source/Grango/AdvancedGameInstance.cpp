// Fill out your copyright notice in the Description page of Project Settings.


#include "AdvancedGameInstance.h"


#include "EdGraphSchema_K2.h"
#include "ImageUtils.h"


FObjectDataStruct UAdvancedGameInstance::GObjectData(FString GObjectName)
{
    return *(GObjectDataTable->FindRow<FObjectDataStruct>(FName(GObjectName),TEXT("")));
}

AActor* UAdvancedGameInstance::SpawnGObject(FString GObjectName)
{
    FObjectDataStruct row = GObjectData(GObjectName);
    AActor* result = GetWorld()->SpawnActor<AActor>(row.ObjectActor,FVector::ZeroVector,FRotator::ZeroRotator);
    return result;
}

TArray<FFloat2DMatrix> UAdvancedGameInstance::GetNoiseMatrix(int _MapWidth, int _MapHeight, float _Scale, float _Seed, int _Octaves, float _Persistance, float _Lacunarity)
{
    FRandomStream Stream;
    Stream.Initialize(_Seed);
    
    TArray<FFloat2DMatrix> NoiseMap;
    FFloat2DMatrix temp;
    temp.arr.Init(0.f,_MapHeight);
    NoiseMap.Init(temp,_MapWidth);
    if(_Scale <= 0)
    {
        _Scale = 0.0001f;
    }

    float maxNoiseHeight = FLT_MIN;
    float minNoiseHeight = FLT_MAX;

    float halfWidth = _MapWidth / 2;
    float halfHeight = _MapHeight / 2;


    for(int y = 0; y < _MapHeight; y++)
    {
        for(int x = 0; x < _MapWidth; x++)
        {

            float amplitude = 1;
            float frequency = 1;
            float noiseHeight = 0;
            
            for(int i = 0; i < _Octaves; i++)
            {
                float sampleX = (x - halfWidth) / _Scale * frequency + _Seed;
                float sampleY = (y - halfHeight) / _Scale * frequency + _Seed;

                float perlinValue = FMath::PerlinNoise2D(FVector2D(sampleX,sampleY)) * 2 - 1;
                noiseHeight += perlinValue * amplitude;

                amplitude *= _Persistance;
                frequency *= _Lacunarity;
            }

            if(noiseHeight > maxNoiseHeight)
            {
                maxNoiseHeight = noiseHeight;
            }else if(noiseHeight < minNoiseHeight)
            {
                minNoiseHeight = noiseHeight;
            }
            NoiseMap[x].arr[y] = noiseHeight;
        }
    }

    for(int y = 0; y < _MapHeight; y++)
    {
        for(int x = 0; x < _MapWidth; x++)
        {
            NoiseMap[x].arr[y] = (NoiseMap[x].arr[y] - minNoiseHeight) / (maxNoiseHeight - minNoiseHeight);
        }
    }
    

    return NoiseMap;
}

UTexture2D* UAdvancedGameInstance::GetNoiseHeightMap(TArray<FFloat2DMatrix> _HeightMap)
{
    TArray<FColor> colorMap;
    colorMap.Init(FColor::White,_HeightMap.Num() * _HeightMap[0].arr.Num());

    int x = _HeightMap[0].arr.Num();
    int y = _HeightMap.Num();

    for(int Rows = 0; Rows < y; Rows++)
    {
        for(int Columns = 0; Columns < x; Columns++)
        {
            colorMap[Rows * x + Columns] = FLinearColor::LerpUsingHSV(FColor::Black,FColor::White,_HeightMap[Rows].arr[Columns]).ToFColor(false);
        }
    }

    UTexture2D* HeightTexture = FImageUtils::CreateTexture2D(x,y,colorMap,this,TEXT("HeightMapTexture"),EObjectFlags::RF_Public|EObjectFlags::RF_Transient,FCreateTexture2DParameters());
    return HeightTexture;
}

UTexture2D* UAdvancedGameInstance::GetNoiseColorMap(TArray<FFloat2DMatrix> _HeightMap)
{
    TArray<FColor> colorMap;
    colorMap.Init(FColor::White,_HeightMap.Num() * _HeightMap[0].arr.Num());

    int x = _HeightMap[0].arr.Num();
    int y = _HeightMap.Num();

    for(int Rows = 0; Rows < y; Rows++)
    {
        for(int Columns = 0; Columns < x; Columns++)
        {
            float currentHeight = _HeightMap[Rows].arr[Columns];
            for(int i = 0; i < Regions.Num(); i++)
            {
                if(currentHeight <= Regions[i].Height)
                {
                    colorMap[Columns * x + Rows] = Regions[i].Color;
                    break;
                }
            }
        }
    }

    UTexture2D* ColorTexture = FImageUtils::CreateTexture2D(x,y,colorMap,this,TEXT("HeightMapTexture"),EObjectFlags::RF_Public|EObjectFlags::RF_Transient,FCreateTexture2DParameters());
    return ColorTexture;
}

FLandMeshData UAdvancedGameInstance::GetLandMeshData(TArray<FFloat2DMatrix> _HeightMap, UCurveFloat* CurveHeight)
{
    int Width = _HeightMap[0].arr.Num();
    int Height = _HeightMap.Num();
    float TopLeftX = (Width-1) / -20.0f + 3;
    float TopLeftY = (Height-1) / 20.0f - 3;

    int SizeX = 20;
    int SizeY = 20;

    FLandMeshData MeshData = FLandMeshData(Width,Height);
    int VertexIndex = 0;

    for(int y = 0; y < Height; y++)
    {
        for(int x = 0; x < Width; x++)
        {
            float HeightValue = _HeightMap[x].arr[y];

                
            MeshData.vertices[VertexIndex] = FVector(TopLeftX + (x*SizeX),TopLeftY - (y*SizeY),CurveHeight->GetFloatValue(HeightValue)*30);
            MeshData.uvs[VertexIndex] = FVector2D(x/(float)Width, y/(float)Height);

            if(x < Width-1 && y < Height-1)
            {
                MeshData.AddTriangle(VertexIndex,VertexIndex+Width+1,VertexIndex+Width);
                MeshData.AddTriangle(VertexIndex+Width+1, VertexIndex,VertexIndex+1);
            }

            
            VertexIndex++;
        }
    }

    return MeshData;
}
