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

TArray<FFloat2DMatrix> UAdvancedGameInstance::GetNoiseMatrix(int _MapWidth, int _MapHeight, float _Scale, float _Seed, int _Octaves, float _Persistance, float _Lacunarity, FVector2D _Offset)
{
    FRandomStream Stream;
    Stream.Initialize(_Seed);
    TArray<FVector2D> OctaveOffsets;
    OctaveOffsets.Init(FVector2D(0,0),_Octaves);

    float MaxPossibleHeight = 0;
    float amplitude = 1;
    float frequency = 1;
    
    for(int i = 0; i < _Octaves; i++)
    {
        float offsetX = Stream.FRandRange(-100000,100000) + _Offset.X;
        float offsetY = Stream.FRandRange(-100000, 100000) - _Offset.Y;
        OctaveOffsets[i] = FVector2D(offsetX,offsetY);

        MaxPossibleHeight += amplitude;
        amplitude *= _Persistance;
    }
    
    TArray<FFloat2DMatrix> NoiseMap;
    FFloat2DMatrix temp;
    temp.arr.Init(0,_MapHeight);
    NoiseMap.Init(temp, _MapWidth);
    if(_Scale <= 0)
    {
        _Scale = 0.0001f;
    }

    float maxLocalNoiseHeight = FLT_MIN;
    float minLocalNoiseHeight = FLT_MAX;

    float halfWidth = _MapWidth / 2;
    float halfHeight = _MapHeight / 2;


    for(int y = 0; y < _MapHeight; y++)
    {
        for(int x = 0; x < _MapWidth; x++)
        {

            amplitude = 1;
            frequency = 1;
            float noiseHeight = 0;
            
            for(int i = 0; i < _Octaves; i++)
            {
                float sampleX = (x - halfWidth + OctaveOffsets[i].X) / _Scale * frequency;
                float sampleY = (y - halfHeight + OctaveOffsets[i].Y) / _Scale * frequency;

                float perlinValue = FMath::PerlinNoise2D(FVector2D(sampleX,sampleY)) * 2 - 1;
                noiseHeight += perlinValue * amplitude;

                amplitude *= _Persistance;
                frequency *= _Lacunarity;
            }

            if(noiseHeight > maxLocalNoiseHeight)
            {
                maxLocalNoiseHeight = noiseHeight;
            }else if(noiseHeight < minLocalNoiseHeight)
            {
                minLocalNoiseHeight = noiseHeight;
            }
            NoiseMap[x].arr[y] = noiseHeight;
        }
    }

    for(int y = 0; y < _MapHeight; y++)
    {
        for(int x = 0; x < _MapWidth; x++)
        {
            if(normalizeMode == ENormalizeMode::Local)
            {
                NoiseMap[x].arr[y] = (NoiseMap[x].arr[y] - minLocalNoiseHeight) / (maxLocalNoiseHeight - minLocalNoiseHeight);
            }else
            {
                float NormalizeHeight = (NoiseMap[x].arr[y] + 1)/(2*MaxPossibleHeight / 7.f);
                NoiseMap[x].arr[y] = NormalizeHeight;
            }
            
        }
    }
    

    return NoiseMap;
}

TArray<FFloat2DMatrix> UAdvancedGameInstance::GetRadialGradient(int _Width, int _Height)
{
    TArray<FFloat2DMatrix> RadialMap;
    FFloat2DMatrix temp;
    temp.arr.Init(0,_Height);
    RadialMap.Init(temp, _Width);

    FVector2D MaskCenter = FVector2D(_Width * 0.5f,_Height * 0.5f);

    for(int y = 0; y < _Height; y++)
    {
        for(int x = 0; x < _Width; x++)
        {
            float DistFromCenter = FVector2D::Distance(MaskCenter,FVector2D(x,y));
            float MaskPixel = (0.5f-(DistFromCenter / _Width)) * 2.0f;
            RadialMap[x].arr[y] = MaskPixel;
        }
    }

    return RadialMap;
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
            colorMap[Columns * x + Rows] = FLinearColor::LerpUsingHSV(FColor::Black,FColor::White,_HeightMap[Rows].arr[Columns]).ToFColor(false);
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
    int SizeX = 50;
    int SizeY = 50;
    
    int Width = _HeightMap[0].arr.Num();
    int Height = _HeightMap.Num();
    float TopLeftX = ((Width-1) / -2)*SizeX;
    float TopLeftY = ((Height-1) / 2)*SizeY;
    

    FLandMeshData MeshData = FLandMeshData(Width,Height);
    int VertexIndex = 0;

    for(int y = 0; y < Height; y++)
    {
        for(int x = 0; x < Width; x++)
        {
            float HeightValue = _HeightMap[x].arr[y];

                
            MeshData.vertices[VertexIndex] = FVector(TopLeftX + (x*SizeX),TopLeftY - (y*SizeY),CurveHeight->GetFloatValue(HeightValue)*100);
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

TArray<FFloat2DMatrix> UAdvancedGameInstance::GetMixedRadialNoise(TArray<FFloat2DMatrix> _Noise,
    TArray<FFloat2DMatrix> _Radial)
{
    int Width = _Noise[0].arr.Num();
    int Height = _Noise.Num();

    TArray<FFloat2DMatrix> IslandNoiseMap;
    FFloat2DMatrix temp;
    temp.arr.Init(0,Height);
    IslandNoiseMap.Init(temp, Width);

    for(int i = 0; i < Width; i ++)
    {
        for(int j = 0; j < Height; j ++)
        {
            IslandNoiseMap[i].arr[j] = _Noise[i].arr[j] + _Radial[i].arr[j];
        }
    }
    return IslandNoiseMap;
}
