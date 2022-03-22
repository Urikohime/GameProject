#include "PlayerHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

APlayerHUD::APlayerHUD()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> HealthBarObj(TEXT("/Game/Assets/UI/PlayerHUD"));
	HUDWidget = HealthBarObj.Class;
}

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	if (HUDWidget != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidget);

		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}
}