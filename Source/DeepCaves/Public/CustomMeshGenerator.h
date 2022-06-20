// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "Components/PointLightComponent.h"
#include "Engine/PointLight.h"
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
	void GenerateTopNoiseMap(TArray<TArray<float>> &Map);
	void GenerateBottomNoiseMap(TArray<TArray<float>> &Map);
	void CalculateLightPos();

	#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	#endif // WITH_EDITOR

private:
	UPROPERTY(VisibleAnywhere,Category="ProceduralMesh")
	UProceduralMeshComponent * mesh;
	
	UPROPERTY(EditAnywhere,Category="Map props");
	int MapWidth=50;
	UPROPERTY(EditAnywhere,Category="Map props");
	int MapHeight=50;
	UPROPERTY(EditAnywhere,Category="Map props");
	float MapScale=25.0f;
	UPROPERTY(EditAnywhere,Category="Map props");
	int MapSeed=0;
	UPROPERTY(EditAnywhere,Category="Map props");
	bool CalculateLight=false;
	
	UPROPERTY(EditAnywhere,Category="BottomMap props");
	int BottomMapOctaves=2;
	UPROPERTY(EditAnywhere,Category="BottomMap props");
	float BottomMapPersistance=0.5f;
	UPROPERTY(EditAnywhere,Category="BottomMap props");
	float BottomMapLacunarity=2.0f;
	UPROPERTY(EditAnywhere,Category="BottomMap props");
	float BottomMapHighMultipier=15.0f;

	UPROPERTY(EditAnywhere,Category="TopMap props");
	int TopMapOctaves=3;
	UPROPERTY(EditAnywhere,Category="TopMap props");
	float TopMapPersistance=0.5f;
	UPROPERTY(EditAnywhere,Category="TopMap props");
	float TopMapLacunarity=2.0f;
	UPROPERTY(EditAnywhere,Category="TopMap props");
	float TopMapHighMultipier=25.0f;
	
	TArray<TArray<float>> NoiseMapB;
	TArray<TArray<float>> NoiseMapT;
	
	TArray<APointLight*> LightVector;
	TArray<FVector> positions;
	//UChildActorComponent L2;
};
