// Copyright Epic Games, Inc. All Rights Reserved.

#include "BejeWellPlayPawn.h"
#include "BejeWellPlayBlock.h"
#include "BejeWellPlayBlockGrid.h"
#include "Components/TextRenderComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"

#define LOCTEXT_NAMESPACE "Pawn"

ABejeWellPlayPawn::ABejeWellPlayPawn(const FObjectInitializer& _objectInitializer) 
	: Super(_objectInitializer)
{
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void ABejeWellPlayPawn::Tick(float _deltaSeconds)
{
	Super::Tick(_deltaSeconds);

	if (APlayerController* playerController = Cast<APlayerController>(GetController()))
	{
		FVector start, dir, end;
		playerController->DeprojectMousePositionToWorld(start, dir);
		end = start + (dir * 8000.0f);

		TraceForBlock(start, end);
	}
}

void ABejeWellPlayPawn::SetupPlayerInputComponent(UInputComponent* _playerInputComponent)
{
	Super::SetupPlayerInputComponent(_playerInputComponent);

	_playerInputComponent->BindAction("TriggerClick", EInputEvent::IE_Pressed, this, &ABejeWellPlayPawn::TriggerPress);
	_playerInputComponent->BindAction("TriggerClick", EInputEvent::IE_Released, this, &ABejeWellPlayPawn::TriggerRelease);
}

void ABejeWellPlayPawn::CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult)
{
	Super::CalcCamera(DeltaTime, OutResult);

	OutResult.Rotation = FRotator(-90.0f, -90.0f, 0.0f);
}

void ABejeWellPlayPawn::TriggerPress()
{
	if (m_focusedBlock)
	{
		if (!m_focusedBlock->GetGrid()->BlocksAreMoving())
		{
			m_focusedBlock->Select();
			m_selectedBlock = m_focusedBlock;
		}
		else
		{
			m_selectedBlock = nullptr;
			UE_LOG(LogTemp, Error, TEXT("Blocks are moving"));
		}
	}
}

void ABejeWellPlayPawn::TriggerRelease()
{
	if (m_selectedBlock && m_focusedBlock && m_focusedBlock != m_selectedBlock)
	{
		m_selectedBlock->GetGrid()->Swap(m_selectedBlock, m_neighborDirection);
		m_selectedBlock->Deselect();
		m_selectedBlock = nullptr;
	}
	else if(m_selectedBlock)
	{
		m_selectedBlock->Deselect();
		m_selectedBlock = nullptr;
	}
	
}

void ABejeWellPlayPawn::ClearBlockUnderMouse()
{
	if (m_focusedBlock)
	{
		m_focusedBlock->Highlight(false);
		m_focusedBlock = nullptr;
	}
}

void ABejeWellPlayPawn::SetFocusedBlock(ABejeWellPlayBlock* _newFocusedBlock)
{
	m_focusedBlock = _newFocusedBlock;
	m_focusedBlock->GetGrid()->GetTextRenderComponent()->SetText(FText::Format(LOCTEXT("Debug text", "Score: {0}"), FText::AsNumber(m_score)));
}

void ABejeWellPlayPawn::TraceForBlock(const FVector& _start, const FVector& _end)
{
	FHitResult hitResult;

	GetWorld()->LineTraceSingleByChannel(hitResult, _start, _end, ECC_Visibility);
	
	if (hitResult.Actor.IsValid())
	{
		ABejeWellPlayBlock* hitBlock = Cast<ABejeWellPlayBlock>(hitResult.Actor.Get());
		if (hitBlock)
		{
			// If there is a selected block, chose the hit block as it's neighbor in the good direction
			if (m_selectedBlock)
			{
				int hitRow = hitBlock->GetRowIndex();
				int hitColumn = hitBlock->GetColumnIndex();
				int rowSignedDistance = hitRow - m_selectedBlock->GetRowIndex();
				int columnSignedDistance = hitColumn - m_selectedBlock->GetColumnIndex();

				int hitRowIndex = m_selectedBlock->GetRowIndex();
				int hitColumnIndex = m_selectedBlock->GetColumnIndex();

				// Choose the axis where the hitBlock is the most far away from the selected block
				if (abs(rowSignedDistance) > abs(columnSignedDistance))
				{
					if (rowSignedDistance > 0)
					{
						hitRowIndex += 1;
						m_neighborDirection = MoveDirections::Top;
					}
					else if (rowSignedDistance < 0)
					{
						hitRowIndex -= 1;
						m_neighborDirection = MoveDirections::Bottom;
					}
					else
					{
						m_neighborDirection = MoveDirections::None;
					}
				}
				else
				{
					if (columnSignedDistance > 0)
					{
						hitColumnIndex += 1;
						m_neighborDirection = MoveDirections::Right;
					}
					else if (columnSignedDistance < 0)
					{
						hitColumnIndex -= 1;
						m_neighborDirection = MoveDirections::Left;
					}
					else
					{
						m_neighborDirection = MoveDirections::None;
					}
				}

				// Ask the grid for the block at indices hitRowIndex, hitColumnIndex
				ABejeWellPlayBlock* neighborBlock = m_selectedBlock->GetGrid()->GetBlockAtIndices(hitRowIndex, hitColumnIndex);
				if (neighborBlock)
				{
					hitBlock = neighborBlock;
				}
				else
				{
					hitBlock = m_selectedBlock;
				}
			}
			else
			{
				m_neighborDirection = MoveDirections::None;
			}
			// If a new block is under the mouse, highlight the correct one and set it as under the mouse.
			if (hitBlock != m_focusedBlock)
			{
				if (m_focusedBlock)
				{
					m_focusedBlock->Highlight(false);
				}
				if (hitBlock)
				{
					hitBlock->Highlight(true);
				}
				SetFocusedBlock(hitBlock);
			}
		}
		else
		{
			ClearBlockUnderMouse();
		}
	}
	else
	{
		ClearBlockUnderMouse();
	}
}

#undef LOCTEXT_NAMESPACE