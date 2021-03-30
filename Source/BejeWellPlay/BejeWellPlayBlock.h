// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BejeWellPlayBlock.generated.h"

class ABejeWellPlayBlockGrid;

/** A block that can be clicked */
UCLASS(minimalapi)
class ABejeWellPlayBlock : public AActor
{
	GENERATED_BODY()

public:
	ABejeWellPlayBlock();

	virtual void Tick(float _deltaSeconds) override;

	USceneComponent* GetDummyRoot() const { return m_dummyRoot; }
	UStaticMeshComponent* GetStaticMeshComponent() const { return m_staticMeshComponent; }

	void Highlight(bool _value);

	void SetOwningGrid(ABejeWellPlayBlockGrid* _grid) { m_owningGrid = _grid; }

	void Init(int _blockType);

	int GetRowIndex(); 
	int GetColumnIndex();
	int GetType() { return m_blockType; }

	ABejeWellPlayBlockGrid* GetGrid() { return m_owningGrid; }

	void Select();
	void Deselect();

	void MoveLeft();
	void MoveRight();
	void MoveTop();
	void MoveBottom();

	bool IsMoving();
private:
	/** Dummy root component */
	USceneComponent* m_dummyRoot;

	UStaticMeshComponent* m_staticMeshComponent;

	UMaterialInstance* Gem1BaseMaterial;
	UMaterialInstance* Gem2BaseMaterial;
	UMaterialInstance* Gem3BaseMaterial;
	UMaterialInstance* Gem4BaseMaterial;
	UMaterialInstance* Gem5BaseMaterial;

	UMaterialInstance* Gem1SelectedMaterial;
	UMaterialInstance* Gem2SelectedMaterial;
	UMaterialInstance* Gem3SelectedMaterial;
	UMaterialInstance* Gem4SelectedMaterial;
	UMaterialInstance* Gem5SelectedMaterial;

	UMaterialInstance* Gem1HighlightedMaterial;
	UMaterialInstance* Gem2HighlightedMaterial;
	UMaterialInstance* Gem3HighlightedMaterial;
	UMaterialInstance* Gem4HighlightedMaterial;
	UMaterialInstance* Gem5HighlightedMaterial;

	ABejeWellPlayBlockGrid* m_owningGrid;

	bool m_selected;

	int m_blockType;

	FVector m_movingVector;
	float m_movingTimeRemaining;

	UMaterialInstance* GetBaseMaterial();
	UMaterialInstance* GetSelectedMaterial();
	UMaterialInstance* GetHighlightedMaterial();
};



