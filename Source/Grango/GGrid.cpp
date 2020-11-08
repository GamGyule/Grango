// Fill out your copyright notice in the Description page of Project Settings.


#include "GGrid.h"

// Sets default values
AGGrid::AGGrid()
{
	LineProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("LineProceduralMesh"));
	LineProceduralMesh->SetupAttachment(RootComponent);
}

void AGGrid::LocationToTile(FVector Location, bool& Valid, int& Row, int& Column)
{
	int r = FMath::FloorToInt(((NumRows*Location.X)/GridWidth()));
	int c = FMath::FloorToInt(((NumColumns*(Location.Y)/GridHeight())));
	//int r = FMath::FloorToInt((NumRows*(Location.X - GetActorLocation().X)/GridWidth()));
	//int c = FMath::FloorToInt((NumColumns*(Location.Y - GetActorLocation().Y)/GridHeight()));
	//
	//int x = FMath::FloorToInt((1000*(NewPosition.X)/(1000*150)));
	//int y = FMath::FloorToInt((1000*(NewPosition.Y)/(1000*150)));
	
	Row = r;
	Column = c;

	/*if(TileValid(r,c))
	{
		Valid = TileValid(r,c);
		Row = r;
		Column = c;
	}*/
}

void AGGrid::TileToLocation(FVector& Location, int Row, int Column)
{
	int x = (TileSize/2)+TileSize*Row;
	int y = (TileSize/2)+TileSize*Column;
	Location.X = x;
	Location.Y = y;
	Location.Z = 0;
}

void AGGrid::BeginPlay()
{
}

float AGGrid::GridWidth()
{
	return NumRows*TileSize;
}

float AGGrid::GridHeight()
{
	return NumColumns*TileSize;
}

void AGGrid::CreateLine(FVector Start, FVector End, float Thinkness)
{
	float HalfThickness = Thinkness/2;
	FVector ThicknessDirection = FVector::CrossProduct((FVector)(End-Start).Normalize(),FVector(0.f,0.f,1.f));

	TArray<int> TriangleTemp;
	TriangleTemp.Add(LineVertices.Num()+2);
	TriangleTemp.Add(LineVertices.Num()+1);
	TriangleTemp.Add(LineVertices.Num()+0);
	TriangleTemp.Add(LineVertices.Num()+2);
	TriangleTemp.Add(LineVertices.Num()+3);
	TriangleTemp.Add(LineVertices.Num()+1);

	LineTriangles.Append(TriangleTemp);

	TArray<FVector> VerticeTemp;
	VerticeTemp.Add(Start+(ThicknessDirection*HalfThickness));
	VerticeTemp.Add(End+(ThicknessDirection*HalfThickness));
	VerticeTemp.Add(Start-(ThicknessDirection*HalfThickness));
	VerticeTemp.Add(End-(ThicknessDirection*HalfThickness));

	LineVertices.Append(VerticeTemp);

	
	
}

UMaterialInterface* AGGrid::CreateMaterialInstance(FLinearColor Color, float Opacity)
{
	UMaterialInstanceDynamic* DynMaterial;
	DynMaterial = UMaterialInstanceDynamic::Create(MaterialInstance,this);
	DynMaterial->SetVectorParameterValue("Color",Color);
	DynMaterial->SetScalarParameterValue("Opacity",Opacity);
	return DynMaterial;
}

void AGGrid::InitGrid()
{
	UMaterialInterface* LineMaterialInstance;
	float LineStart = 0;
	float LineEnd = 0;

	LineMaterialInstance = CreateMaterialInstance(LineColor,LineOpacity);
	
	

	for(int i = 0; i < NumRows; i++)
	{
		LineStart = i * TileSize;
		LineEnd = GridWidth();
		CreateLine(FVector(LineStart,0.f,0.f),FVector(LineStart,LineEnd,0.f),LineThickness);
	}

	for(int i = 0; i < NumColumns; i++)
	{
		LineStart = i * TileSize;
		LineEnd = GridHeight();
		CreateLine(FVector(0.f,LineStart,0.f),FVector(LineEnd,LineStart,0.f),LineThickness);
	}

	LineProceduralMesh->SetMaterial(0,LineMaterialInstance);
	LineProceduralMesh->CreateMeshSection(0,LineVertices,LineTriangles,TArray<FVector>(),TArray<FVector2D>(),TArray<FColor>(),TArray<FProcMeshTangent>(),false);
	
}

bool AGGrid::TileValid(int Row, int Column)
{
	if(Row >= 0 && Row < NumRows)
	{
		if(Column >= 0 && Column < NumColumns)
		{
			return true;
		}
	}
	return false;
}


