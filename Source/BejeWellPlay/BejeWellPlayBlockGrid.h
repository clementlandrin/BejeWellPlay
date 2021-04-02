// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnumHelper.h"
#include "BejeWellPlayBlockGrid.generated.h"

UENUM()
enum class MoveDirections : uint8
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

	int GetNumberOfRows() { return m_numberOfRows; }
	int GetNumberOfColumns() { return m_numberOfColumns; }

	float GetGridSize() { return m_gridSize; }

	ABejeWellPlayBlock* GetBlockAtIndices(int _rowIndex, int _columnIndex);

	void Swap(ABejeWellPlayBlock* _blockToSwap, MoveDirections _direction, bool _isReversedSwap = false);

	int GetBlockRowIndex(ABejeWellPlayBlock* _block);
	int GetBlockColumnIndex(ABejeWellPlayBlock* _block);

	bool BlocksAreMoving();

	float GetSpeed() { return m_speed; };
protected:
	virtual void BeginPlay() override;

private:
	void CreateNewBlock(int _rowIndex, int _columnIndex);

	UFUNCTION()
	void EndSwap(ABejeWellPlayBlock* _blockToSwap, MoveDirections _direction, bool _isReversedSwap);
	bool CheckGrid();
	void CheckColumns(TArray<ABejeWellPlayBlock*>& _blocksToDelete);
	void CheckRows(TArray<ABejeWellPlayBlock*>& _blocksToDelete);
	bool DeleteBlocks(TArray<ABejeWellPlayBlock*> _blocksToDelete);
	void ApplyGravity();
	void AttractBlockAbove(int _rowIndex, int _columnIndex);
	bool FillGrid();

	int m_numberOfRows = 8;
	int m_numberOfColumns = 8;

	float m_gridSize = 1000.0f;
	float m_speed = 6.0f;
	int m_numberOfMovingBlocks;

	TArray<TArray<ABejeWellPlayBlock*>> m_blocks;
};



