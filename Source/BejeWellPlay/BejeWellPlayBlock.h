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

	/** Returns DummyRoot subobject **/
	USceneComponent* GetDummyRoot() const { return m_dummyRoot; }
	/** Returns BlockMesh subobject **/
	UStaticMeshComponent* GetStaticMeshComponent() const { return m_staticMeshComponent; }

	/*void BlockClicked(UPrimitiveComponent* _clickedComp, FKey _buttonClicked);

	void OnFingerPressedBlock(ETouchIndex::Type _fingerIndex, UPrimitiveComponent* _touchedComponent);

	void HandleClicked();*/

	void Highlight(bool _value);

	void SetOwningGrid(ABejeWellPlayBlockGrid* _grid) { m_owningGrid = _grid; }

	void Init(int _rowId, int _columnId, int _blockType);

	int GetRowIndex(); 
	int GetColumnIndex();

	ABejeWellPlayBlockGrid* GetGrid() { return m_owningGrid; }

	void Select();
	void Deselect();

	void MoveLeft();
	void MoveRight();
	void MoveTop();
	void MoveBottom();
private:
	/** Dummy root component */
	USceneComponent* m_dummyRoot;

	/** StaticMesh component for the clickable block */
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

	/** Grid that owns us */
	ABejeWellPlayBlockGrid* m_owningGrid;

	bool m_selected;

	int m_blockType;

	UMaterialInstance* GetBaseMaterial();
	UMaterialInstance* GetSelectedMaterial();
	UMaterialInstance* GetHighlightedMaterial();
};



