// Fill out your copyright notice in the Description page of Project Settings.


#include "GObject.h"

#include "GPlayerController.h"

// Sets default values
AGObject::AGObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GObjectMesh"));
	GObjectMesh->SetupAttachment(RootComponent);
	GObjectMesh->OnComponentBeginOverlap.AddDynamic(this,&AGObject::OnOverlapBegin);
	GObjectMesh->OnComponentEndOverlap.AddDynamic(this,&AGObject::OnOverlapEnd);
	
}

void AGObject::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                              int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bIsOverlap = true;
	GObjectMesh->SetScalarParameterValueOnMaterials("CanBuild",0);
	AGPlayerController* GPlayerController = Cast<AGPlayerController>(GetWorld()->GetFirstPlayerController());
	GPlayerController->bCanBuild = false;
}

void AGObject::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bIsOverlap = false;
	GObjectMesh->SetScalarParameterValueOnMaterials("CanBuild",1);
	AGPlayerController* GPlayerController = Cast<AGPlayerController>(GetWorld()->GetFirstPlayerController());
	GPlayerController->bCanBuild = true;
}

void AGObject::CanBuild(bool bCanBuild)
{
	if(bCanBuild && !bIsOverlap)
	{
		GObjectMesh->SetScalarParameterValueOnMaterials("CanBuild",1);
		AGPlayerController* GPlayerController = Cast<AGPlayerController>(GetWorld()->GetFirstPlayerController());
		GPlayerController->bCanBuild = true;
	}else
	{
		GObjectMesh->SetScalarParameterValueOnMaterials("CanBuild",0);
		AGPlayerController* GPlayerController = Cast<AGPlayerController>(GetWorld()->GetFirstPlayerController());
		GPlayerController->bCanBuild = false;
	}
}

void AGObject::BuildSuccess()
{
	GObjectMesh->SetMaterial(0,Material);
}

void AGObject::SetObject()
{}
