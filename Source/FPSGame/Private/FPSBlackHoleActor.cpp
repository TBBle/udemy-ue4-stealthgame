// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSBlackHoleActor.h"

#include "Components/PrimitiveComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Containers/Array.h"

namespace {
void SetCollisionProfile(UPrimitiveComponent* Component)
{
	// Since I'm trying to do this all in C++, this is really a hard-coded version of
	// UPrimitiveComponent::SetCollisionProfile. It might be better to create a CollisionProfile
	// from C++ if I'm going to drive this entirely from code, and then hook it up here?

	// Query-only collision for Overlap with Physics Bodies
	// Annoyingly, there's no clear way in the Editor to tell a BP subclass "Revert to parent"
	// for the collision response setup.
	Component->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	// TODO: Mess with this list.
	Component->SetCollisionResponseToAllChannels(ECR_Ignore);
	Component->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Component->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
	// "Projectile"... Not sure if there's a good way to validate or sync this?
	// See DefaultEngine.ini [/Script/Engine.CollisionProfile] DefaultChannelResponses
	Component->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
}
}

// Sets default values
AFPSBlackHoleActor::AFPSBlackHoleActor() :
	MeshComponent{CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"))},
	AttractorComponent{CreateDefaultSubobject<USphereComponent>(TEXT("Attractor"))},
	DestroyerComponent{CreateDefaultSubobject<USphereComponent>(TEXT("Destroyer"))}
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = MeshComponent;
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	DestroyerComponent->SetupAttachment(MeshComponent);
	SetCollisionProfile(DestroyerComponent);

	AttractorComponent->SetupAttachment(MeshComponent);
	SetCollisionProfile(AttractorComponent);
}

void AFPSBlackHoleActor::BeginPlay()
{
	Super::BeginPlay();

	// TIL: https://answers.unrealengine.com/questions/32320/oncomponentbeginoverlap-not-working.html
	DestroyerComponent->OnComponentBeginOverlap.AddDynamic(this, &AFPSBlackHoleActor::OnDestroyerBeginOverlap);
}

void AFPSBlackHoleActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const auto forceSource = GetActorLocation();
	const auto forceRadius = AttractorComponent->GetScaledSphereRadius();

	auto overlappers = TArray<UPrimitiveComponent*>{};
	AttractorComponent->GetOverlappingComponents(overlappers);
	for (auto overlapper : overlappers)
	{
		// TODO: I suspect Projectiles don't work here as they use a ProjectileMover, and hence aren't using Physics.
		if (overlapper->IsSimulatingPhysics())
		{
			overlapper->AddRadialForce(forceSource, forceRadius, -AttactionForce, RIF_Linear);
		}
	}
}

void AFPSBlackHoleActor::OnDestroyerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (!OtherActor)
		return;

	OtherActor->Destroy();
}