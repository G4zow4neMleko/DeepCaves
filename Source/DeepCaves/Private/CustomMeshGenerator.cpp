// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomMeshGenerator.h"
#include <limits>
#include "KismetProceduralMeshLibrary.h"

// Sets default values
ACustomMeshGenerator::ACustomMeshGenerator()
{
	mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = mesh;
	// New in UE 4.17, multi-threaded PhysX cooking.
	mesh->bUseAsyncCooking = true;
	
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACustomMeshGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACustomMeshGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// This is called when actor is spawned (at runtime or when you drop it into the world in editor)
void ACustomMeshGenerator::PostActorCreated()
{
	Super::PostActorCreated();
	GenerateTopNoiseMap(NoiseMapT);
	GenerateBottomNoiseMap(NoiseMapB);
	CreateMesh();
}

// This is called when actor is already in level and map is opened
void ACustomMeshGenerator::PostLoad()
{
	Super::PostLoad();
	/*enerateTopNoiseMap(NoiseMapT);
	GenerateBottomNoiseMap(NoiseMapB);
	CreateMesh();*/
}

void ACustomMeshGenerator::CreateMesh()
{
	/*if(NoiseMapT.Num()<1)
		GenerateTopNoiseMap(NoiseMapT);
	if(NoiseMapB.Num()<1)
		GenerateBottomNoiseMap(NoiseMapB);*/
	
	TArray<FVector> vertices;
	TArray<int32> triangles;
	TArray<FVector2D> UV0;
	TArray<FLinearColor> vertexColors;
	TArray<FVector> normals;
	TArray<FProcMeshTangent> tangents;
	
	vertices.SetNum(MapWidth*MapHeight*2);
	triangles.SetNum((MapWidth-1)*(MapHeight-1)*6 *2);
	UV0.SetNum(MapWidth*MapHeight*2);
	normals.Init(FVector(0,0,0),MapWidth*MapHeight*2);
	tangents.Init(FProcMeshTangent(0, 0, 0), MapWidth*MapHeight*2);
	vertexColors.Init(FLinearColor(0.75, 0.75, 0.75, 1.0),MapWidth*MapHeight*2);
	
	int vertexIndex = 0;
	int triangleIndex = 0;
	float topLeftX = (MapWidth-1)/-2.0f;
	float topLeftY = (MapHeight-1)/2.0f;

	int verticesHalfIndex = vertices.Num()/2;
	
	for(int y=0; y<MapHeight; ++y){
		for(int x=0; x<MapWidth; ++x)
		{
			// noiseMap to heightMap
			
			vertices[vertexIndex] = FVector(topLeftX + x, topLeftY - y, NoiseMapB[y][x]*BottomMapHighMultipier);
			UV0[vertexIndex] = FVector2D(x/(float)MapWidth, y/(float)MapHeight);

			vertices[verticesHalfIndex + vertexIndex] = FVector(topLeftX + x, topLeftY - y, NoiseMapT[y][x]*TopMapHighMultipier+TopMapHighMultipier/2);
			UV0[verticesHalfIndex + vertexIndex] = FVector2D(x/(float)MapWidth, y/(float)MapHeight);
			
			
			if( x < (MapWidth-1) && y < (MapHeight-1) )
			{
				triangles[triangleIndex] = vertexIndex;
				triangles[triangleIndex+1] = vertexIndex + MapWidth +1;
				triangles[triangleIndex+2] = vertexIndex + MapWidth;
				
				triangles[triangleIndex+3] = vertexIndex + MapWidth +1;
				triangles[triangleIndex+4] = vertexIndex;
				triangles[triangleIndex+5] = vertexIndex + 1;
				triangleIndex += 6;

				triangles[triangles.Num()-1-triangleIndex] = verticesHalfIndex + vertexIndex;
				triangles[triangles.Num()-1-triangleIndex-1] = verticesHalfIndex + vertexIndex + MapWidth +1;
				triangles[triangles.Num()-1-triangleIndex-2] = verticesHalfIndex + vertexIndex + MapWidth;
				
				triangles[triangles.Num()-1-triangleIndex-3] = verticesHalfIndex + vertexIndex + MapWidth +1;
				triangles[triangles.Num()-1-triangleIndex-4] = verticesHalfIndex + vertexIndex;
				triangles[triangles.Num()-1-triangleIndex-5] = verticesHalfIndex + vertexIndex + 1;

			}
			vertexIndex++;
		}
	}

	/*triangleIndex = triangles.Num()-1;
	for(int y=0; y<MapHeight; ++y){
		for(int x=0; x<MapWidth; ++x)
		{
			// noiseMap to heightMap
			
			vertices[vertexIndex] = FVector(topLeftX + x, topLeftY - y, NoiseMapT[y][x]*TopMapHighMultipier+TopMapHighMultipier/2);
			UV0[vertexIndex] = FVector2D(x/(float)MapWidth, y/(float)MapHeight);
			
			//vertexColors.Emplace(FLinearColor(0.75, 0.75, 0.75, 1.0));
			
			if( x < (MapWidth-1) && y < (MapHeight-1) )
			{
				triangles[triangleIndex] = vertexIndex;
				triangles[triangleIndex-1] = vertexIndex + MapWidth +1;
				triangles[triangleIndex-2] = vertexIndex + MapWidth;
				triangleIndex -= 3;
				
				triangles[triangleIndex] = vertexIndex + MapWidth +1;
				triangles[triangleIndex-1] = vertexIndex;
				triangles[triangleIndex-2] = vertexIndex + 1;
				triangleIndex -= 3;
			}
			vertexIndex++;
		}
	}*/
	
	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(vertices, triangles, UV0,normals,tangents);
	
	mesh->CreateMeshSection_LinearColor(0, vertices, triangles, normals, UV0, vertexColors, tangents, true);
        
	// Enable collision data
	mesh->ContainsPhysicsTriMeshData(true);
}

void ACustomMeshGenerator::GenerateBottomNoiseMap(TArray<TArray<float>> &Map)
{
	if(MapWidth > 120)
		MapWidth = 120;
	if(MapHeight > 120)
		MapHeight = 120;
	
	Map.SetNum(MapHeight);
	for(auto &row : Map)
	{
		row.SetNum(0);
		row.Init(0, MapWidth);
	}
	
	if(MapSeed == 0)
	{
		MapSeed = FMath::Rand();
	}
	FRandomStream rgen = FRandomStream(MapSeed);
	TArray<FVector2D> MapOctavesOffset;
	for(int i=0; i<BottomMapOctaves; ++i)
	{
		float offsetX = rgen.FRandRange(-100'000,100'000);
		float offsetY = rgen.FRandRange(-100'000,100'000);
		MapOctavesOffset.Emplace(FVector2D(offsetX,offsetY));
	}
	
	if(MapScale <= 0.0f){MapScale = 0.0001f;}
	//float maxNoiseHeight = std::numeric_limits<float>::min();
	//float minNoiseHeight = std::numeric_limits<float>::max();
	constexpr float minNoiseHeight = 0.0f;

	for(int y=0; y<MapHeight; ++y){
		for(int x=0; x<MapWidth; ++x)
		{
			float amplitude = 1;
			float frequency = 1;
			float noiseHeight = 0;
			
			for(int i=0; i<BottomMapOctaves; ++i)
			{
				float sampleX = x/MapScale*frequency + MapOctavesOffset[i].X;
				float sampleY = y/MapScale*frequency + MapOctavesOffset[i].Y;

				float perlinValue =	FMath::PerlinNoise2D(FVector2D(sampleX,sampleY));
				noiseHeight += perlinValue*amplitude;
				if(noiseHeight < minNoiseHeight)
					noiseHeight = 0.0f;
				
				amplitude *= BottomMapPersistance;
				frequency *= BottomMapLacunarity;
			}
			/*if(noiseHeight > maxNoiseHeight)
				maxNoiseHeight = noiseHeight;*/
			/*else if(noiseHeight < minNoiseHeight)
				minNoiseHeight = noiseHeight;*/
			
			Map[y][x] = noiseHeight; 
		}
	}
	/*for(int y=0; y<MapHeight; ++y)
	{
		for(int x=0; x<MapWidth; ++x)
			Map[y][x] = (Map[y][x]+FMath::Abs(minNoiseHeight)) / (FMath::Abs(minNoiseHeight) + FMath::Abs(maxNoiseHeight));
	}*/
}

void ACustomMeshGenerator::GenerateTopNoiseMap(TArray<TArray<float>> &Map)
{
	if(MapWidth > 120)
		MapWidth = 120;
	if(MapHeight > 120)
		MapHeight = 120;
	
	Map.SetNum(MapHeight);
	for(auto &row : Map)
	{
		row.SetNum(0);
		row.Init(0, MapWidth);
	}
	
	if(MapSeed == 0)
	{
		MapSeed = FMath::Rand();
	}
	FRandomStream rgen = FRandomStream(MapSeed);
	TArray<FVector2D> MapOctavesOffset;
	for(int i=0; i<TopMapOctaves; ++i)
	{
		float offsetX = rgen.FRandRange(-100'000,100'000);
		float offsetY = rgen.FRandRange(-100'000,100'000);
		MapOctavesOffset.Emplace(FVector2D(offsetX,offsetY));
	}
	
	if(MapScale <= 0.0f){MapScale = 0.0001f;}
	/*float maxNoiseHeight = std::numeric_limits<float>::min();
	float minNoiseHeight = std::numeric_limits<float>::max();*/
	//constexpr float minNoiseHeight = 0.0f;

	for(int y=0; y<MapHeight; ++y){
		for(int x=0; x<MapWidth; ++x)
		{
			float amplitude = 1;
			float frequency = 1;
			float noiseHeight = 0;
			
			for(int i=0; i<TopMapOctaves; ++i)
			{
				float sampleX = x/MapScale*frequency + MapOctavesOffset[i].X;
				float sampleY = y/MapScale*frequency + MapOctavesOffset[i].Y;

				float perlinValue =	FMath::PerlinNoise2D(FVector2D(sampleX,sampleY));
				noiseHeight += perlinValue*amplitude*-1;
				
				amplitude *= TopMapPersistance;
				frequency *= TopMapLacunarity;
			}
			/*if(noiseHeight > maxNoiseHeight)
				maxNoiseHeight = noiseHeight;
			else if(noiseHeight < minNoiseHeight)
				minNoiseHeight = noiseHeight;*/
			
			Map[y][x] = noiseHeight; 
		}
	}
	/*for(int y=0; y<MapHeight; ++y)
	{
		for(int x=0; x<MapWidth; ++x)
			Map[y][x] = (Map[y][x]+FMath::Abs(minNoiseHeight)) / (FMath::Abs(minNoiseHeight) + FMath::Abs(maxNoiseHeight));
	}*/
}

void ACustomMeshGenerator::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if(PropertyChangedEvent.GetPropertyName().ToString().Contains("Top"))
		GenerateTopNoiseMap(NoiseMapT);
	else if(PropertyChangedEvent.GetPropertyName().ToString().Contains("Bottom"))
		GenerateBottomNoiseMap(NoiseMapB);
	else
	{
		GenerateTopNoiseMap(NoiseMapT);
		GenerateBottomNoiseMap(NoiseMapB);
	}
	
	CreateMesh();
}
