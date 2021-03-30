// Copyright Epic Games, Inc. All Rights Reserved.

#include "BejeWellPlayBlockGrid.h"
#include "Components/TextRenderComponent.h"
#include "BejeWellPlayBlock.h"
#include "Engine/World.h"
#include "Math/RandomStream.h"

#define LOCTEXT_NAMESPACE "PuzzleBlockGrid"

ABejeWellPlayBlockGrid::ABejeWellPlayBlockGrid()
{
	// Create dummy root scene component
	m_dummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = m_dummyRoot;

	// Create static mesh component
	m_textRenderComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ScoreText0"));
	m_textRenderComponent->SetupAttachment(m_dummyRoot);

	m_blocks.AddZeroed(m_numberOfRows);
	for (int i = 0; i < m_blocks.Num(); i++)
	{
		m_blocks[i].AddZeroed(m_numberOfColumns);
	}

	m_numberOfMovingBlocks = 0;
}

void ABejeWellPlayBlockGrid::BeginPlay()
{
	Super::BeginPlay();
	
	float blockWidth = m_gridSize / (m_numberOfColumns);
	float blockHeight = m_gridSize / (m_numberOfRows);

	// Loop to spawn each block
	for(int i = 0 ; i < m_numberOfRows; i++)
	{
		for (int j = 0; j < m_numberOfColumns; j++)
		{
			// offset grows with index, displaced of m_gridSize * 0.5f to center on zero and take the block size into account
			const float xOffset = i * m_gridSize / m_numberOfColumns - m_gridSize * 0.5f + blockWidth * 0.5f;
			const float yOffset = j * m_gridSize / m_numberOfRows - m_gridSize * 0.5f + blockHeight * 0.5f;

			// Make position vector, offset from Grid location
			const FVector blockLocation = FVector(xOffset, yOffset, 0.f) + GetActorLocation();

			// Spawn a block
			ABejeWellPlayBlock* newBlock = GetWorld()->SpawnActor<ABejeWellPlayBlock>(blockLocation, FRotator(0, 0, 0));

			// Divide the actor scale by the bouding volume size so it fits any mesh, regardless its dimensions.
			FVector location = FVector();
			FVector size = FVector();;
			newBlock->GetActorBounds(false, location, size, false);
			newBlock->SetActorScale3D(FVector(blockWidth / size.X * 0.5f, blockHeight / size.Y * 0.5f, 0.1f));

			// Tell the block about its owner
			if (newBlock)
			{
				int random = FMath::RandRange(0, 4);
				newBlock->SetOwningGrid(this);
				newBlock->Init(i, j, random);
				m_blocks[i][j] = newBlock;
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Error"));
			}
		}
	}

	this->CheckGrid();

	m_textRenderComponent->SetText(FText::Format(LOCTEXT("Debug text", "Debug: {0}"), FText::AsNumber(m_numberOfRows * m_numberOfColumns)));
}

ABejeWellPlayBlock* ABejeWellPlayBlockGrid::GetBlockAtIndices(int _rowIndex, int _columnIndex)
{
	if (_rowIndex < 0 || _rowIndex > m_numberOfRows || _columnIndex < 0 || _columnIndex > m_numberOfColumns)
	{
		return nullptr;
	}

	return m_blocks[_rowIndex][_columnIndex];
}

bool ABejeWellPlayBlockGrid::BlocksAreMoving()
{
	int movingBlocks = 0;
	for (int i = 0; i < m_numberOfRows; i++)
	{
		for (int j = 0; j < m_numberOfColumns; j++)
		{
			if (m_blocks[i][j] && !m_blocks[i][j]->IsPendingKill())
			{
				if (m_blocks[i][j]->IsMoving())
				{
					movingBlocks++;
				}
			}
		}
	}
	return movingBlocks > 0;
}
void ABejeWellPlayBlockGrid::Swap(ABejeWellPlayBlock* _blockToSwap, MoveDirections _direction, bool _isReversedSwap)
{
	ABejeWellPlayBlock* otherBlock = nullptr;
	switch (_direction)
	{
	case MoveDirections::Top:
		UE_LOG(LogTemp, Error, TEXT("Move top"));
		if (_blockToSwap->GetRowIndex() >= m_numberOfRows - 1)
		{
			return;
		}
		otherBlock = m_blocks[_blockToSwap->GetRowIndex() + 1][_blockToSwap->GetColumnIndex()];
		m_blocks[_blockToSwap->GetRowIndex()][_blockToSwap->GetColumnIndex()] = otherBlock;
		m_blocks[_blockToSwap->GetRowIndex() + 1][_blockToSwap->GetColumnIndex()] = _blockToSwap;
		_blockToSwap->MoveTop();
		otherBlock->MoveBottom();
		break;
	case MoveDirections::Bottom:
		UE_LOG(LogTemp, Error, TEXT("Move bottom"));
		if (_blockToSwap->GetRowIndex() <= 0)
		{
			return;
		}
		otherBlock = m_blocks[_blockToSwap->GetRowIndex() - 1][_blockToSwap->GetColumnIndex()];
		m_blocks[_blockToSwap->GetRowIndex()][_blockToSwap->GetColumnIndex()] = otherBlock;
		m_blocks[_blockToSwap->GetRowIndex() - 1][_blockToSwap->GetColumnIndex()] = _blockToSwap;
		_blockToSwap->MoveBottom();
		otherBlock->MoveTop();
		break;
	case MoveDirections::Left:
		UE_LOG(LogTemp, Error, TEXT("Move left"));
		if (_blockToSwap->GetColumnIndex() <= 0)
		{
			return;
		}
		otherBlock = m_blocks[_blockToSwap->GetRowIndex()][_blockToSwap->GetColumnIndex() - 1];
		m_blocks[_blockToSwap->GetRowIndex()][_blockToSwap->GetColumnIndex()] = otherBlock;
		m_blocks[_blockToSwap->GetRowIndex()][_blockToSwap->GetColumnIndex() - 1] = _blockToSwap;
		_blockToSwap->MoveLeft();
		otherBlock->MoveRight();
		break;
	case MoveDirections::Right:
		UE_LOG(LogTemp, Error, TEXT("Move right"));
		if (_blockToSwap->GetColumnIndex() >= m_numberOfColumns - 1)
		{
			return;
		}
		otherBlock = m_blocks[_blockToSwap->GetRowIndex()][_blockToSwap->GetColumnIndex() + 1];
		m_blocks[_blockToSwap->GetRowIndex()][_blockToSwap->GetColumnIndex()] = otherBlock;
		m_blocks[_blockToSwap->GetRowIndex()][_blockToSwap->GetColumnIndex() + 1] = _blockToSwap;
		_blockToSwap->MoveRight();
		otherBlock->MoveLeft();
		break;
	}

	FTimerDelegate TimerDel;
	FTimerHandle TimerHandle;

	//Binding the function with specific values
	TimerDel.BindUFunction(this, FName("EndSwap"), _blockToSwap, _direction, _isReversedSwap);
	//Calling MyUsefulFunction after 5 seconds without looping
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, 1.f, false);
}

void ABejeWellPlayBlockGrid::EndSwap(ABejeWellPlayBlock* _blockToSwap, MoveDirections _direction, bool _isReversedSwap)
{
	bool swapWasSuccessFull = CheckGrid();
	if (!_isReversedSwap && !swapWasSuccessFull)
	{
		MoveDirections oppositeDirection;
		switch (_direction)
		{
		case MoveDirections::Right:
			oppositeDirection = MoveDirections::Left;
			break;
		case MoveDirections::Left:
			oppositeDirection = MoveDirections::Right;
			break; 
		case MoveDirections::Top:
			oppositeDirection = MoveDirections::Bottom;
			break;
		case MoveDirections::Bottom:
			oppositeDirection = MoveDirections::Top;
			break;
		default:
			oppositeDirection = MoveDirections::None;
			break;
		}
		Swap(_blockToSwap, oppositeDirection, true);
	}
}

int ABejeWellPlayBlockGrid::GetBlockRowIndex(ABejeWellPlayBlock* _block)
{
	return (_block->GetActorLocation() - GetActorLocation()).X / m_gridSize * m_numberOfColumns + m_numberOfRows * 0.5;
}

int ABejeWellPlayBlockGrid::GetBlockColumnIndex(ABejeWellPlayBlock* _block)
{
	return (_block->GetActorLocation() - GetActorLocation()).Y / m_gridSize * m_numberOfRows + m_numberOfColumns * 0.5;
}

bool ABejeWellPlayBlockGrid::CheckGrid()
{
	TArray<ABejeWellPlayBlock*> blocksToDelete = TArray<ABejeWellPlayBlock*>();
	CheckColumns(blocksToDelete);
	CheckRows(blocksToDelete);
	bool deletedBlocks = false;
	while (DeleteBlocks(blocksToDelete))
	{
		deletedBlocks = true;
		ApplyGravity();
		blocksToDelete = TArray<ABejeWellPlayBlock*>();
		CheckColumns(blocksToDelete);
		CheckRows(blocksToDelete);
	}
	return deletedBlocks;
}

void ABejeWellPlayBlockGrid::CheckRows(TArray<ABejeWellPlayBlock*>& _blocksToDelete)
{
	UE_LOG(LogTemp, Error, TEXT("CheckGrid"));
	for (int i = 0; i < m_numberOfRows; i++)
	{
		int tempBlockType = -1;
		int beginColumnIndex = 0;
		for (int j = 0; j < m_numberOfColumns; j++)
		{
			if (m_blocks[i][j] == nullptr || m_blocks[i][j]->IsPendingKill())
			{
				beginColumnIndex = j + 1;
				tempBlockType = -1;
			}
			else if (m_blocks[i][j]->GetType() == tempBlockType)
			{
				if (j - beginColumnIndex == 2)
				{
					_blocksToDelete.Push(m_blocks[i][beginColumnIndex]);
					_blocksToDelete.Push(m_blocks[i][beginColumnIndex + 1]);
					_blocksToDelete.Push(m_blocks[i][beginColumnIndex + 2]);
				}
				else if (j - beginColumnIndex > 2)
				{
					_blocksToDelete.Push(m_blocks[i][j]);
				}
			}
			else
			{
				tempBlockType = m_blocks[i][j]->GetType();
				beginColumnIndex = j;
			}
		}
	}
}

void ABejeWellPlayBlockGrid::CheckColumns(TArray<ABejeWellPlayBlock*>& _blocksToDelete)
{
	UE_LOG(LogTemp, Error, TEXT("CheckGrid"));
	for (int j = 0; j < m_numberOfColumns; j++)
	{
		int tempBlockType = -1;
		int beginRowIndex = 0;
		for (int i = 0; i < m_numberOfRows; i++)
		{
			if (m_blocks[i][j] == nullptr || m_blocks[i][j]->IsPendingKill())
			{
				beginRowIndex = i + 1;
				tempBlockType = -1;
			}
			else if (m_blocks[i][j]->GetType() == tempBlockType)
			{
				if (i - beginRowIndex == 2)
				{
					_blocksToDelete.Push(m_blocks[beginRowIndex][j]);
					_blocksToDelete.Push(m_blocks[beginRowIndex + 1][j]);
					_blocksToDelete.Push(m_blocks[beginRowIndex + 2][j]);
				}
				else if (i - beginRowIndex > 2)
				{
					_blocksToDelete.Push(m_blocks[i][j]);
				}
			}
			else
			{
				tempBlockType = m_blocks[i][j]->GetType();
				beginRowIndex = i;
			}
		}
	}
}

bool ABejeWellPlayBlockGrid::DeleteBlocks(TArray<ABejeWellPlayBlock*> _blocksToDelete)
{
	if (_blocksToDelete.Num() == 0)
	{
		return false;
	}

	for (int i = 0; i < _blocksToDelete.Num(); i++)
	{
		if (_blocksToDelete[i])
		{
			_blocksToDelete[i]->Destroy();
		}
	}
	
	return true;
}

void ABejeWellPlayBlockGrid::ApplyGravity()
{
	UE_LOG(LogTemp, Error, TEXT("ApplyGravity"));

	// For each column
	for (int columnIndex = 0; columnIndex < m_numberOfColumns; columnIndex++)
	{
		// Fill empty blocks starting from bottom
		// Don't try to attract blocks above for the top row
		for (int rowIndex = 0; rowIndex < m_numberOfRows - 1; rowIndex++)
		{
			if (m_blocks[rowIndex][columnIndex] == nullptr || m_blocks[rowIndex][columnIndex]->IsPendingKill())
			{
				UE_LOG(LogTemp, Error, TEXT("Empty block here %d %d"), rowIndex, columnIndex);
				AttractBlockAbove(rowIndex, columnIndex);
				FillColumnWithNewBlocks();
			}
		}
	}
}

void ABejeWellPlayBlockGrid::AttractBlockAbove(int _rowIndex, int _columnIndex)
{
	for (int rowIndex = _rowIndex + 1; rowIndex < m_numberOfRows; rowIndex++)
	{
		if (m_blocks[rowIndex][_columnIndex] != nullptr && !m_blocks[rowIndex][_columnIndex]->IsPendingKill())
		{
			UE_LOG(LogTemp, Error, TEXT("Attracting block at %d %d"), rowIndex, _columnIndex);
			for (int i = 0; i < rowIndex - _rowIndex; i++)
			{
				m_blocks[rowIndex][_columnIndex]->MoveBottom();
			}
			m_blocks[_rowIndex][_columnIndex] = m_blocks[rowIndex][_columnIndex];
			m_blocks[rowIndex][_columnIndex] = nullptr;
			return;
		}
	}
}

void ABejeWellPlayBlockGrid::FillColumnWithNewBlocks()
{

}
#undef LOCTEXT_NAMESPACE