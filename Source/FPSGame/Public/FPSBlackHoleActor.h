// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSBlackHoleActor.generated.h"

// Forward declarations
class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class FPSGAME_API AFPSBlackHoleActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSBlackHoleActor();

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* AttractorComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* DestroyerComponent;

	// I have no idea what units these are in. LRN2Physics
	UPROPERTY(EditAnywhere, Category = "Black Hole")
	float AttactionForce = 10000000.f;

private:
	void BeginPlay() override;
	void Tick(float DeltaSeconds) override;

	// FComponentBeginOverlapSignature
	UFUNCTION()
	void OnDestroyerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};
