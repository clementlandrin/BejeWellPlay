// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EnumHelper.h"
#include "BejeWellPlayPawn.generated.h"

enum MoveDirections;
class ABejeWellPlayBlock;

UCLASS(config=Game)
class ABejeWellPlayPawn : public APawn
{
	GENERATED_UCLASS_BODY()

public:

	virtual void Tick(float _deltaSeconds) override;

	virtual void SetupPlayerInputComponent(UInputComponent* _playerInputComponent) override;

	virtual void CalcCamera(float _deltaTime, struct FMinimalViewInfo& _outResult) override;

protected:
	void TriggerPress();
	void TriggerRelease();
	void TraceForBlock(const FVector& _start, const FVector& _end);
	void ClearBlockUnderMouse();
	void SetFocusedBlock(ABejeWellPlayBlock* _newFocusedBlock);

	ABejeWellPlayBlock* m_focusedBlock;
	ABejeWellPlayBlock* m_selectedBlock;
	MoveDirections m_neighborDirection;
};
