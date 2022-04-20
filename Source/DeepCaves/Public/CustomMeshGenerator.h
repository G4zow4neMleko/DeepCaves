// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "CustomMeshGenerator.generated.h"

UCLASS()
class DEEPCAVES_API ACustomMeshGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACustomMeshGenerator();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void PostLoad() override;
	void PostActorCreated() override;
	void CreateMesh();
	void GenerateNoiseMap();
	
	#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	#endif // WITH_EDITOR

private:
	UPROPERTY(VisibleAnywhere,Category="ProceduralMesh")
	UProceduralMeshComponent * mesh;
	
	UPROPERTY(EditAnywhere,Category="Map props");
	int MapWidth=100;
	UPROPERTY(EditAnywhere,Category="Map props");
	int MapHeight=100;
	UPROPERTY(EditAnywhere,Category="Map props");
	float MapScale=25.0f;
	UPROPERTY(EditAnywhere,Category="Map props");
	int MapOctaves=2;
	UPROPERTY(EditAnywhere,Category="Map props");
	float MapPersistance=0.5f;
	UPROPERTY(EditAnywhere,Category="Map props");
	float MapLacunarity=2.0f;
	UPROPERTY(EditAnywhere,Category="Map props");
	int MapSeed=0;
	UPROPERTY(EditAnywhere,Category="Map props");
	float MapHighMultipier=15.0f;
	
	TArray<TArray<float>> NoiseMap;

};
