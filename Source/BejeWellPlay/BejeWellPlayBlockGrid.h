// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnumHelper.h"
#include "BejeWellPlayBlockGrid.generated.h"

enum MoveDirections : int
{
	None,
	Top,
	Bottom,
	Left,
	Right
};

class ABejeWellPlayBlock;

/** Class used to spawn blocks and manage score */
UCLASS(minimalapi)
class ABejeWellPlayBlockGrid : public AActor
{
	GENERATED_BODY()
		

	/** Dummy root component */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* m_dummyRoot;

	/** Text component for the score */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UTextRenderComponent* m_textRenderComponent;
public:
	ABejeWellPlayBlockGrid();

	USceneComponent* GetDummyRoot() const { return m_dummyRoot; }
	UTextRenderComponent* GetTextRenderComponent() const { return m_textRenderComponent; }

	int GetBlockByRow() { return m_blockByRow; }
	int GetBlockByColumn() { return m_blockByColumn; }

	float GetGridSize() { return m_gridSize; }

	ABejeWellPlayBlock* GetBlockAtIndices(int _rowIndex, int _columnIndex);

	// Return true if the swap succeeded
	bool Swap(ABejeWellPlayBlock* _blockToSwap, MoveDirections _direction);

	int GetBlockRowIndex(ABejeWellPlayBlock* _block);
	int GetBlockColumnIndex(ABejeWellPlayBlock* _block);
protected:
	virtual void BeginPlay() override;

private:

	int m_blockByRow = 8;
	int m_blockByColumn = 8;

	float m_gridSize = 1000.0f;

	TArray<TArray<ABejeWellPlayBlock*>> m_blocks;
};



