// Copyright Epic Games, Inc. All Rights Reserved.

#include "BejeWellPlayBlock.h"
#include "BejeWellPlayBlockGrid.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstance.h"

ABejeWellPlayBlock::ABejeWellPlayBlock()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;

		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> Gem1BaseMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> Gem2BaseMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> Gem3BaseMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> Gem4BaseMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> Gem5BaseMaterial;

		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> Gem1SelectedMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> Gem2SelectedMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> Gem3SelectedMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> Gem4SelectedMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> Gem5SelectedMaterial;

		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> Gem1HighlightedMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> Gem2HighlightedMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> Gem3HighlightedMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> Gem4HighlightedMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> Gem5HighlightedMaterial;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Puzzle/Meshes/PuzzleCube.PuzzleCube"))

			, Gem1BaseMaterial(TEXT("/Game/Puzzle/Meshes/Gem1Material.Gem1Material"))
			, Gem2BaseMaterial(TEXT("/Game/Puzzle/Meshes/Gem2Material.Gem2Material"))
			, Gem3BaseMaterial(TEXT("/Game/Puzzle/Meshes/Gem3Material.Gem3Material"))
			, Gem4BaseMaterial(TEXT("/Game/Puzzle/Meshes/Gem4Material.Gem4Material"))
			, Gem5BaseMaterial(TEXT("/Game/Puzzle/Meshes/Gem5Material.Gem5Material"))

			, Gem1SelectedMaterial(TEXT("/Game/Puzzle/Meshes/Gem1SelectedMaterial.Gem1SelectedMaterial"))
			, Gem2SelectedMaterial(TEXT("/Game/Puzzle/Meshes/Gem2SelectedMaterial.Gem2SelectedMaterial"))
			, Gem3SelectedMaterial(TEXT("/Game/Puzzle/Meshes/Gem3SelectedMaterial.Gem3SelectedMaterial"))
			, Gem4SelectedMaterial(TEXT("/Game/Puzzle/Meshes/Gem4SelectedMaterial.Gem4SelectedMaterial"))
			, Gem5SelectedMaterial(TEXT("/Game/Puzzle/Meshes/Gem5SelectedMaterial.Gem5SelectedMaterial"))

			, Gem1HighlightedMaterial(TEXT("/Game/Puzzle/Meshes/Gem1HighlightedMaterial.Gem1HighlightedMaterial"))
			, Gem2HighlightedMaterial(TEXT("/Game/Puzzle/Meshes/Gem2HighlightedMaterial.Gem2HighlightedMaterial"))
			, Gem3HighlightedMaterial(TEXT("/Game/Puzzle/Meshes/Gem3HighlightedMaterial.Gem3HighlightedMaterial"))
			, Gem4HighlightedMaterial(TEXT("/Game/Puzzle/Meshes/Gem4HighlightedMaterial.Gem4HighlightedMaterial"))
			, Gem5HighlightedMaterial(TEXT("/Game/Puzzle/Meshes/Gem5HighlightedMaterial.Gem5HighlightedMaterial"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Create dummy root scene component
	m_dummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = m_dummyRoot;

	// Create static mesh component
	m_staticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh0"));
	m_staticMeshComponent->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());
	m_staticMeshComponent->SetRelativeScale3D(FVector(1.f,1.f,0.25f));
	m_staticMeshComponent->SetRelativeLocation(FVector(0.f,0.f,25.f));
	m_staticMeshComponent->SetupAttachment(m_dummyRoot);
	/*m_staticMeshComponent->OnClicked.AddDynamic(this, &ABejeWellPlayBlock::BlockClicked);
	m_staticMeshComponent->OnInputTouchBegin.AddDynamic(this, &ABejeWellPlayBlock::OnFingerPressedBlock);*/

	Gem1BaseMaterial = ConstructorStatics.Gem1BaseMaterial.Get();
	Gem2BaseMaterial = ConstructorStatics.Gem2BaseMaterial.Get();
	Gem3BaseMaterial = ConstructorStatics.Gem3BaseMaterial.Get();
	Gem4BaseMaterial = ConstructorStatics.Gem4BaseMaterial.Get();
	Gem5BaseMaterial = ConstructorStatics.Gem5BaseMaterial.Get();

	Gem1SelectedMaterial = ConstructorStatics.Gem1SelectedMaterial.Get();
	Gem2SelectedMaterial = ConstructorStatics.Gem2SelectedMaterial.Get();
	Gem3SelectedMaterial = ConstructorStatics.Gem3SelectedMaterial.Get();
	Gem4SelectedMaterial = ConstructorStatics.Gem4SelectedMaterial.Get();
	Gem5SelectedMaterial = ConstructorStatics.Gem5SelectedMaterial.Get();

	Gem1HighlightedMaterial = ConstructorStatics.Gem1HighlightedMaterial.Get();
	Gem2HighlightedMaterial = ConstructorStatics.Gem2HighlightedMaterial.Get();
	Gem3HighlightedMaterial = ConstructorStatics.Gem3HighlightedMaterial.Get();
	Gem4HighlightedMaterial = ConstructorStatics.Gem4HighlightedMaterial.Get();
	Gem5HighlightedMaterial = ConstructorStatics.Gem5HighlightedMaterial.Get();

	m_selected = false;
}

void ABejeWellPlayBlock::Init(int _rowId, int _columnId, int _blockType)
{
	m_blockType = _blockType;
	m_staticMeshComponent->SetMaterial(0, GetBaseMaterial());
}

int ABejeWellPlayBlock::GetRowIndex()
{
	return m_owningGrid->GetBlockRowIndex(this);
}

int ABejeWellPlayBlock::GetColumnIndex()
{
	return m_owningGrid->GetBlockColumnIndex(this);
}

void ABejeWellPlayBlock::Select()
{
	m_selected = true;
	m_staticMeshComponent->SetMaterial(0, GetSelectedMaterial());
	UE_LOG(LogTemp, Error, TEXT("Selected"));
}

void ABejeWellPlayBlock::Deselect()
{
	m_selected = false;
	m_staticMeshComponent->SetMaterial(0, GetBaseMaterial());
	UE_LOG(LogTemp, Error, TEXT("Deselected"));
}

void ABejeWellPlayBlock::MoveLeft()
{
	SetActorLocation(GetActorLocation() - FVector(0.0f, GetGrid()->GetGridSize() / GetGrid()->GetBlockByRow(), 0.0f));
}

void ABejeWellPlayBlock::MoveRight()
{
	SetActorLocation(GetActorLocation() + FVector(0.0f, GetGrid()->GetGridSize() / GetGrid()->GetBlockByRow(), 0.0f));
}

void ABejeWellPlayBlock::MoveTop()
{
	SetActorLocation(GetActorLocation() + FVector(GetGrid()->GetGridSize() / GetGrid()->GetBlockByColumn(), 0.0f, 0.0f));
}

void ABejeWellPlayBlock::MoveBottom()
{
	SetActorLocation(GetActorLocation() - FVector(GetGrid()->GetGridSize() / GetGrid()->GetBlockByColumn(), 0.0f, 0.0f));
}

/*void ABejeWellPlayBlock::BlockClicked(UPrimitiveComponent* _clickedComp, FKey _buttonClicked)
{
	HandleClicked();
}


void ABejeWellPlayBlock::OnFingerPressedBlock(ETouchIndex::Type _fingerIndex, UPrimitiveComponent* _touchedComponent)
{
	HandleClicked();
}

void ABejeWellPlayBlock::HandleClicked()
{

}*/

UMaterialInstance* ABejeWellPlayBlock::GetBaseMaterial()
{
	switch (m_blockType)
	{
	default:
	case 0:
		return Gem1BaseMaterial;
	case 1:
		return Gem2BaseMaterial;
	case 2:
		return Gem3BaseMaterial;
	case 3:
		return Gem4BaseMaterial;
	case 4:
		return Gem5BaseMaterial;
	}
}

UMaterialInstance* ABejeWellPlayBlock::GetSelectedMaterial()
{
	switch (m_blockType)
	{
	default:
	case 0:
		return Gem1SelectedMaterial;
	case 1:
		return Gem2SelectedMaterial;
	case 2:
		return Gem3SelectedMaterial;
	case 3:
		return Gem4SelectedMaterial;
	case 4:
		return Gem5SelectedMaterial;
	}
}

UMaterialInstance* ABejeWellPlayBlock::GetHighlightedMaterial()
{
	switch (m_blockType)
	{
	default:
	case 0:
		return Gem1HighlightedMaterial;
	case 1:
		return Gem2HighlightedMaterial;
	case 2:
		return Gem3HighlightedMaterial;
	case 3:
		return Gem4HighlightedMaterial;
	case 4:
		return Gem5HighlightedMaterial;
	}
}

void ABejeWellPlayBlock::Highlight(bool _value)
{
	if (!m_selected)
	{
		if (_value)
		{
			m_staticMeshComponent->SetMaterial(0, GetHighlightedMaterial());
		}
		else
		{
			m_staticMeshComponent->SetMaterial(0, GetBaseMaterial());
		}
	}
}
