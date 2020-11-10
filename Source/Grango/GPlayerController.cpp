// Fill out your copyright notice in the Description page of Project Settings.


#include "GPlayerController.h"
#include "GrangoCharacter.h"
#include "ImageUtils.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/GameplayStatics.h"

AGPlayerController::AGPlayerController()
{
}
void AGPlayerController::BeginPlay()
{
    Initialize();
    GameUMG = CreateWidget<UGGameWidget>(GetWorld()->GetFirstPlayerController(),GameWidget);
    GameUMG->AddToViewport();

    Grid = GetWorld()->SpawnActor<AGGrid>(GridClass,FVector(0,0,201),FRotator::ZeroRotator);
    Grid->SetActorHiddenInGame(true);
    Grid->LineProceduralMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    Grid->MaterialInstance = GridMaterial;
    Grid->InitGrid();

    
}

void AGPlayerController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    
    BuildSystem();
    /*UpdatePosition();
    AddChunk();
    RemoveChunk();*/
}

void AGPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    InputComponent->BindAction("Tab",IE_Pressed,this,&AGPlayerController::OnTab);
    InputComponent->BindAction("Mouseleft",IE_Pressed,this,&AGPlayerController::OnMouseLeftClick);
    InputComponent->BindAction("MouseRight",IE_Pressed,this,&AGPlayerController::OnMouseRightClick);
    InputComponent->BindAction("Esc",IE_Pressed,this,&AGPlayerController::OnEsc);
}

void AGPlayerController::ShowGrid()
{
    
    if(Grid->IsHidden())
    {
        AGrangoCharacter* GPlayerCharacter = Cast<AGrangoCharacter>(GetPawn());
        bIsBuild = true;

        FVector GridPosition = FVector(FMath::FloorToInt(GPlayerCharacter->GetActorLocation().X/Grid->TileSize)*Grid->TileSize-3750,FMath::FloorToInt(GPlayerCharacter->GetActorLocation().Y/Grid->TileSize)*Grid->TileSize-3750,201);
        Grid->SetActorLocation(GridPosition);
        Grid->SetActorHiddenInGame(false);
        GameUMG->BuildHelpPanel->SetVisibility(ESlateVisibility::Visible);
    }else
    {
        bIsBuild = false;
        Grid->SetActorHiddenInGame(true);
        GameUMG->BuildHelpPanel->SetVisibility(ESlateVisibility::Hidden);
    }
}

void AGPlayerController::OnMouseLeftClick()
{
    if(IsValid(BuildObject))
    {
        if(bCanBuild)
        {
            //ShowGrid();
            bIsBuild = false;
            BuildObject->BuildSuccess();
            BuildObject->SetObject();
            BuildObject = nullptr;
        }
    }
}

void AGPlayerController::OnTab()
{
    /*TestPerlin();*/
    if(!GameUMG->MenuPanel->IsVisible())
    {
        GameUMG->PanelVisible(GameUMG->MenuPanel,ESlateVisibility::Visible);
        GameUMG->MenuPanel->SetRenderOpacity(1);
    }else
    {
        GameUMG->MenuPanel->SetRenderOpacity(0);
        GameUMG->PanelVisible(GameUMG->MenuPanel,ESlateVisibility::Hidden);
    }
}

void AGPlayerController::OnEsc()
{
    if(bIsBuild)
    {
        bIsBuild = false;
        //ShowGrid();
        GetWorld()->DestroyActor(BuildObject);
    }
    ScreenMsg(FString::FromInt(GameUMG->OpenPanels.Num()));
    if(GameUMG->OpenPanels.Num() > 0)
    {
        for(UPanelWidget* Panel : GameUMG->OpenPanels)
        {
            GameUMG->PanelVisible(Panel,ESlateVisibility::Hidden);
        }
    }
}

void AGPlayerController::OnMouseRightClick()
{
    if(IsValid(BuildObject))
    {
        GetWorld()->DestroyActor(BuildObject);
        bIsBuild = false;
        //ShowGrid();
    }
}

void AGPlayerController::BuildSystem()
{
    if(bIsBuild)
    {
        FVector NewPosition;
        UpdateCursorPosition();
        ObjectLocationToGrid(HitLocation,NewPosition.X,NewPosition.Y,NewPosition.Z);
        if(IsValid(BuildObject))
        {
            bool ObjectCanBuild = (HitLocation.Z < 199)?false:true;
            BuildObject->CanBuild(ObjectCanBuild);
            BuildObject->SetActorLocation(NewPosition);
            int r;
            int c;
            bool a;
            Grid->LocationToTile(NewPosition,a,r,c);
            
        }
    }
}

void AGPlayerController::ObjectLocationToGrid(FVector Location, float& X, float& Y, float& Z)
{
    int TileSize = 1;
    X = (FMath::FloorToInt(Location.X/TileSize)*TileSize)+(TileSize/2);
    Y = (FMath::FloorToInt(Location.Y/TileSize)*TileSize)+(TileSize/2);
    Z = 200;
}

void AGPlayerController::UpdateCursorPosition()
{
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectType;
    FHitResult hit;
    ObjectType.Add(EObjectTypeQuery::ObjectTypeQuery9);
    if(GetHitResultUnderCursorForObjects(ObjectType,true,hit))
    {
        HitLocation = hit.Location;
    }
}

bool AGPlayerController::UpdatePosition()
{
    
    AGrangoCharacter* GPlayerCharacter = Cast<AGrangoCharacter>(GetPawn());
    if(IsValid(GPlayerCharacter))
    {
        CharacterPosition = GPlayerCharacter->GetActorLocation() * FVector(1.f,1.f,0);
        float x = CharacterPosition.X / ChunkSizeHalf;
        float y = CharacterPosition.Y / ChunkSizeHalf;

        int CX = FMath::FloorToInt(x);
        int CY = FMath::FloorToInt(y);

        if(ChunkX != CX || ChunkY != CY)
        {
            ChunkX = CX;
            ChunkY = CY;
            return true;
        }
    }
    return false;
}

void AGPlayerController::Initialize()
{
    ChunkSize = ChunkLineElement * VoxelSize;
    ChunkSizeHalf = ChunkSize;
    
}

void AGPlayerController::AddChunk()
{
    for(int i = RenderRange*-1; i <= RenderRange; i++) // X
    {
        for(int j = RenderRange*-1; j <= RenderRange; j++) // Y
        {
            float x = i + ChunkX;
            float y = j + ChunkY;

            int XCenter = x*ChunkSize + ChunkSizeHalf; // X Center
            int YCenter = y*ChunkSize + ChunkSizeHalf; // Y Center

            if(CheckRadius(XCenter, YCenter))
            {
                if(!ChunksCords.Contains(FVector2D(x,y)))
                {
                    ChunksCords.Add(FVector2D(x,y));
                    auto Chunk = GetWorld()->SpawnActor<AActor>(Floor,FVector(x*ChunkSize,y*ChunkSize,0),FRotator::ZeroRotator);
                    
                    Chunks.Add(Chunk);
                }
            }
        }
    }
}

bool AGPlayerController::CheckRadius(float x, float y)
{
    FVector vector = FVector(x,y,0);
    return (vector - CharacterPosition).Size() < (ChunkSize * RenderRange);
}

void AGPlayerController::RemoveChunk()
{
    int i = 0;
    for(FVector2D vector : ChunksCords)
    {
        int x = ChunkSize*vector.X+ChunkSizeHalf;
        int y = ChunkSize*vector.Y+ChunkSizeHalf;
        if(!CheckRadius(x,y))
        {
            if(IsValid(Chunks[i]))
                GetWorld()->DestroyActor(Chunks[i]);
            ChunksCords.RemoveAt(i);
            Chunks.RemoveAt(i);
        }
        i++;
    }
}

void AGPlayerController::TestPerlin()
{
    GameUMG->NoiseImage->SetBrushFromTexture(nullptr);
    
    int x = GameUMG->NoiseImage->Brush.ImageSize.X;
    int y = GameUMG->NoiseImage->Brush.ImageSize.Y;
    auto GGameInstance = Cast<UAdvancedGameInstance>(GetGameInstance());

    float Scales = GameUMG->ScaleInput->GetValue();
    int Octaves = GameUMG->OctInput->GetValue();
    float Persistance = GameUMG->PerInput->GetValue();
    float Lacunarity = GameUMG->LacInput->GetValue();
    float Seed = GameUMG->SeedInput->GetValue();
    FVector2D Offset = FVector2D(GameUMG->OffsetXInput->GetValue(),GameUMG->OffsetYInput->GetValue());
    
    TArray<FFloat2DMatrix> NoiseMap = GGameInstance->GetNoiseMatrix(x,y,Scales,Seed,Octaves,Persistance,Lacunarity,Offset);
    /*TArray<FFloat2DMatrix> RadialMap = GGameInstance->GetRadialGradient(x,y);
    TArray<FFloat2DMatrix> RadialHeightMap = GGameInstance->GetMixedRadialNoise(NoiseMap,RadialMap);*/
    
    UTexture2D* Texture = GGameInstance->GetNoiseColorMap(NoiseMap);
    GameUMG->NoiseImage->SetBrushFromTexture(Texture);


    
    /*
    TArray<FColor> colorMap;
    colorMap.Init(FColor::White,x*y);

    TArray<FColor> heightColor;
    heightColor.Init(FColor::White,x*y);

    for(int j = 0; j < y; j ++)
    {
        for(int k = 0; k < x; k ++)
        {
            heightColor[j * x + k] = FLinearColor::LerpUsingHSV(FColor::Black,FColor::White,NoiseMap[k].arr[j]).ToFColor(false);
            float currentHeight = NoiseMap[k].arr[j];
            for(int i = 0; i < GGameInstance->Regions.Num(); i++)
            {
                if(currentHeight <= GGameInstance->Regions[i].Height)
                {
                    colorMap[j * x + k] = GGameInstance->Regions[i].Color;
                    break;
                }
            }
        }
    }
    
    UTexture2D* TestTexture;
    TestTexture = FImageUtils::CreateTexture2D(x,y,colorMap,this,TEXT("NoiseTexture"),EObjectFlags::RF_Public|EObjectFlags::RF_Transient,FCreateTexture2DParameters());


    UTexture2D* HeightTexture;
    HeightTexture = FImageUtils::CreateTexture2D(x,y,heightColor,this,TEXT("NoiseTexture"),EObjectFlags::RF_Public|EObjectFlags::RF_Transient,FCreateTexture2DParameters());
    */
}