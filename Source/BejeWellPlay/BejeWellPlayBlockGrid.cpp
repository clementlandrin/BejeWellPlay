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

	m_blocks.AddZeroed(m_blockByRow);
	for (int i = 0; i < m_blocks.Num(); i++)
	{
		m_blocks[i].AddZeroed(m_blockByColumn);
	}
}

void ABejeWellPlayBlockGrid::BeginPlay()
{
	Super::BeginPlay();
	
	float blockWidth = m_gridSize / (m_blockByRow);
	float blockHeight = m_gridSize / (m_blockByColumn);

	// Loop to spawn each block
	for(int i = 0 ; i < m_blockByRow; i++)
	{
		for (int j = 0; j < m_blockByColumn; j++)
		{
			// offset grows with index, displaced of m_gridSize * 0.5f to center on zero and take the block size into account
			const float xOffset = i * m_gridSize / m_blockByRow - m_gridSize * 0.5f + blockWidth * 0.5f;
			const float yOffset = j * m_gridSize / m_blockByColumn - m_gridSize * 0.5f + blockHeight * 0.5f;

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

	m_textRenderComponent->SetText(FText::Format(LOCTEXT("Debug text", "Debug: {0}"), FText::AsNumber(m_blockByRow * m_blockByColumn)));
}

ABejeWellPlayBlock* ABejeWellPlayBlockGrid::GetBlockAtIndices(int _rowIndex, int _columnIndex)
{
	if (_rowIndex < 0 || _rowIndex > m_blockByRow || _columnIndex < 0 || _columnIndex > m_blockByColumn)
	{
		return nullptr;
	}

	return m_blocks[_rowIndex][_columnIndex];
}

bool ABejeWellPlayBlockGrid::Swap(ABejeWellPlayBlock* _blockToSwap, MoveDirections _direction)
{
	ABejeWellPlayBlock* otherBlock = nullptr;
	switch (_direction)
	{
	case MoveDirections::Top:
		UE_LOG(LogTemp, Error, TEXT("Move top"));
		if (_blockToSwap->GetRowIndex() >= m_blockByColumn - 1)
		{
			return false;
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
			return false;
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
			return false;
		}
		otherBlock = m_blocks[_blockToSwap->GetRowIndex()][_blockToSwap->GetColumnIndex() - 1];
		m_blocks[_blockToSwap->GetRowIndex()][_blockToSwap->GetColumnIndex()] = otherBlock;
		m_blocks[_blockToSwap->GetRowIndex()][_blockToSwap->GetColumnIndex() - 1] = _blockToSwap;
		_blockToSwap->MoveLeft();
		otherBlock->MoveRight();
		break;
	case MoveDirections::Right:
		UE_LOG(LogTemp, Error, TEXT("Move right"));
		if (_blockToSwap->GetColumnIndex() >= m_blockByColumn - 1)
		{
			return false;
		}
		otherBlock = m_blocks[_blockToSwap->GetRowIndex()][_blockToSwap->GetColumnIndex() + 1];
		m_blocks[_blockToSwap->GetRowIndex()][_blockToSwap->GetColumnIndex()] = otherBlock;
		m_blocks[_blockToSwap->GetRowIndex()][_blockToSwap->GetColumnIndex() + 1] = _blockToSwap;
		_blockToSwap->MoveRight();
		otherBlock->MoveLeft();
		break;
	}
	return true;
}

int ABejeWellPlayBlockGrid::GetBlockRowIndex(ABejeWellPlayBlock* _block)
{
	return (_block->GetActorLocation() - GetActorLocation()).X / m_gridSize * m_blockByRow + m_blockByRow * 0.5;
}

int ABejeWellPlayBlockGrid::GetBlockColumnIndex(ABejeWellPlayBlock* _block)
{
	return (_block->GetActorLocation() - GetActorLocation()).Y / m_gridSize * m_blockByRow + m_blockByColumn * 0.5;
}

#undef LOCTEXT_NAMESPACE