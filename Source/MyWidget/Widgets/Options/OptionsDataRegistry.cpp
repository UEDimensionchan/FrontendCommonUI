// Fill out your copyright notice in the Description page of Project Settings.


#include "OptionsDataRegistry.h"
#include "DataObjects/ListDataObject_Collection.h"
#include "DataObjects/ListDataObject_String.h"
#include "DataObjects/ListDataObject_Scalar.h"
#include "OptionsDataInteractionHelper.h"
#include "../../FontendSettings/FrontendGameUserSettings.h"
#include "../../FrontendGameplayTags.h"
#include "MyWidget/FrontendFunctionLibrary.h"
#include "DataObjects/ListDataObject_StringResolution.h"
#include "Internationalization/StringTableRegistry.h"

#define MAKE_OPTIONS_DATA_CONTROL(SetterOrGetterFuncName) \
	MakeShared<FOptionsDataInteractionHelper>(GET_FUNCTION_NAME_STRING_CHECKED(UFrontendGameUserSettings, SetterOrGetterFuncName))

#define GET_DESCRIPTION(InKey) LOCTABLE("/Game/UI/StringTables/String_OptionsScreen.String_OptionsScreen", InKey)

void UOptionsDataRegistry::InitOptionsDataRegistry(ULocalPlayer* InOwningLocalPlayer)
{
	InitGameplayCollectionTab();
	InitAudioCollectionTab();
	InitVideoCollectionTab();
	InitControlCollectionTab();
}

TArray<UListDataObject_Base*> UOptionsDataRegistry::GetListSourceItemBySelectedTabID(const FName& InSelectedTabID) const
{
	UListDataObject_Collection* const* FoundTabCollectionPtr = RegisteredOptionsTabCollections.FindByPredicate(
		[InSelectedTabID](UListDataObject_Collection* AvailableTabCollection)->bool
		{
			return AvailableTabCollection->GetDataID() == InSelectedTabID;
		}
	);
	checkf(FoundTabCollectionPtr, TEXT("No valid tp found under the ID %s"), *InSelectedTabID.ToString());

	UListDataObject_Collection* FoundTabCollection = *FoundTabCollectionPtr;


	TArray<UListDataObject_Base*> AllChildListItems;

	for (UListDataObject_Base* ChildListData : FoundTabCollection->GetAllChildSettingData())
	{
		if (!ChildListData)
		{
			continue;
		}
		AllChildListItems.Add(ChildListData);

		if (ChildListData->HasAnyChildListData())
		{
			FindChildListDataRecursively(ChildListData, AllChildListItems);
		}
	}

	return AllChildListItems;
}

void UOptionsDataRegistry::FindChildListDataRecursively(UListDataObject_Base* InParentData, TArray<UListDataObject_Base*>& OutFoundChildListData) const
{
	if (!InParentData || !InParentData->HasAnyChildListData())
	{
		return;
	}

	for (UListDataObject_Base* SubChildListData : InParentData->GetAllChildSettingData())
	{
		if (!SubChildListData)
		{
			continue;
		}
		OutFoundChildListData.Add(SubChildListData);
		if (SubChildListData->HasAnyChildListData())
		{
			FindChildListDataRecursively(SubChildListData, OutFoundChildListData);
		}
	}
}

void UOptionsDataRegistry::InitGameplayCollectionTab()
{
	UListDataObject_Collection* GameplayTabCollection = NewObject<UListDataObject_Collection>();
	GameplayTabCollection->SetDataID(FName("GameplayTabCollection"));
	GameplayTabCollection->SetDataDisplayName(FText::FromString(TEXT("Gameplay")));

	/*TSharedPtr<FOptionsDataInteractionHelper> ConstructedHelper =  MakeShared<FOptionsDataInteractionHelper>(
		GET_FUNCTION_NAME_STRING_CHECKED(UFrontendGameUserSettings, GetCurrentGameDifficulty)
	);*/
	

	//Game Difficulty
	{
		UListDataObject_String* GameDifficulty = NewObject<UListDataObject_String>();
		GameDifficulty->SetDataID(FName("GameDifficult"));
		GameDifficulty->SetDataDisplayName(FText::FromString(TEXT("Difficulty")));
		GameDifficulty->SetDescriptionRichText(FText::FromString(TEXT("Adjust the difficulty of the game Experience.\n\n<Bold>Easy:</> Focuses On the story experience. Prvodes the most relaxing combat.\n\n<Bold>Normal:</>Offers slightly harder combat experience\n\n<Bold>Hard:</>Offers a much more challenging combat experience\n\n<Bold>Very Hard:</>Provides the most challenging combat experience.Not recommened for first play through.")));
		GameDifficulty->AddDynamicOption(TEXT("Easy"), FText::FromString(TEXT("Easy")));
		GameDifficulty->AddDynamicOption(TEXT("Normal"), FText::FromString(TEXT("Normal")));
		GameDifficulty->AddDynamicOption(TEXT("Hard"), FText::FromString(TEXT("Hard")));
		GameDifficulty->AddDynamicOption(TEXT("Very Hard"), FText::FromString(TEXT("Very Hard")));
		GameDifficulty->SetDefaultValueFromString(TEXT("Normal"));
		GameDifficulty->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentGameDifficulty));
		GameDifficulty->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentGameDifficulty));
		GameDifficulty->SetShouldApplyChangeImmediatly(true);

		GameplayTabCollection->AddChildListData(GameDifficulty);
	}

	//Game Difficulty
	{
		UListDataObject_String* TestItem = NewObject<UListDataObject_String>();
		TestItem->SetDataID(FName("TestItem"));
		TestItem->SetDataDisplayName(FText::FromString("Test Image Item"));
		TestItem->SetSoftDescriptionImage(UFrontendFunctionLibrary::GetOptionsSoftImageTag(FrontendGameplayTags::Frontend_Image_TestImage));
		TestItem->SetDescriptionRichText(FText::FromString(TEXT("The Image to display can be specified in the project settings. It can be anything the developer asigned in there")));
		
		GameplayTabCollection->AddChildListData(TestItem);
	}


	RegisteredOptionsTabCollections.Add(GameplayTabCollection);
}

void UOptionsDataRegistry::InitAudioCollectionTab()
{
	UListDataObject_Collection* AudioTabCollection = NewObject<UListDataObject_Collection>();
	AudioTabCollection->SetDataID(FName("AudioTabCollection"));
	AudioTabCollection->SetDataDisplayName(FText::FromString(TEXT("Audio")));

	//Volume Category
	{
		UListDataObject_Collection* VoluemeCategoryCollection = NewObject<UListDataObject_Collection>();
		VoluemeCategoryCollection->SetDataID(FName("VoluemeCategoryCollection"));
		VoluemeCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Volume")));

		AudioTabCollection->AddChildListData(VoluemeCategoryCollection);

		//Overall Volume
		{
			UListDataObject_Scalar* OverallVolume = NewObject<UListDataObject_Scalar>();
			OverallVolume->SetDataID(FName("OverallVolume"));
			OverallVolume->SetDataDisplayName(FText::FromString(TEXT("Overall Volume")));
			OverallVolume->SetDescriptionRichText(FText::FromString(TEXT("This is Description for Overall Volume")));
			OverallVolume->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			OverallVolume->SetOutputValueRange(TRange<float>(0.f, 2.f));
			OverallVolume->SetSliderStepSize(0.01f);
			OverallVolume->SetDefaultValueFromString(LexToString(1.f));
			OverallVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
			OverallVolume->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());
			OverallVolume->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetOverallVolume));
			OverallVolume->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetOverallVolume));
			OverallVolume->SetShouldApplyChangeImmediatly(true);

			VoluemeCategoryCollection->AddChildListData(OverallVolume);
		}

		//Music Volume
		{
			UListDataObject_Scalar* MusicVolume = NewObject<UListDataObject_Scalar>();
			MusicVolume->SetDataID(FName("MusicVolume"));
			MusicVolume->SetDataDisplayName(FText::FromString(TEXT("Music Volume")));
			MusicVolume->SetDescriptionRichText(FText::FromString(TEXT("This is Description for Music Volume")));
			MusicVolume->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			MusicVolume->SetOutputValueRange(TRange<float>(0.f, 2.f));
			MusicVolume->SetSliderStepSize(0.01f);
			MusicVolume->SetDefaultValueFromString(LexToString(1.f));
			MusicVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
			MusicVolume->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());
			MusicVolume->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetMusicVolume));
			MusicVolume->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetMusicVolume));
			MusicVolume->SetShouldApplyChangeImmediatly(true);

			VoluemeCategoryCollection->AddChildListData(MusicVolume);
		}

		//Sound FX Volume
		{
			UListDataObject_Scalar* SoundFXVolume = NewObject<UListDataObject_Scalar>();
			SoundFXVolume->SetDataID(FName("SoundFXVolume"));
			SoundFXVolume->SetDataDisplayName(FText::FromString(TEXT("Sound FX Volume")));
			SoundFXVolume->SetDescriptionRichText(FText::FromString(TEXT("This is Description for Sound FX Volume")));
			SoundFXVolume->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			SoundFXVolume->SetOutputValueRange(TRange<float>(0.f, 2.f));
			SoundFXVolume->SetSliderStepSize(0.01f);
			SoundFXVolume->SetDefaultValueFromString(LexToString(1.f));
			SoundFXVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
			SoundFXVolume->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());
			SoundFXVolume->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetSoundFXVolume));
			SoundFXVolume->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetSoundFXVolume));
			SoundFXVolume->SetShouldApplyChangeImmediatly(true);

			VoluemeCategoryCollection->AddChildListData(SoundFXVolume);
		}

	}

	//Sound Category
	{
		UListDataObject_Collection* SoundCategoryCollection = NewObject<UListDataObject_Collection>();
		SoundCategoryCollection->SetDataID(FName("SoundCategoryCollection"));
		SoundCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Sound")));

		AudioTabCollection->AddChildListData(SoundCategoryCollection);

		//Allow BackGround Audio
		{
			UListDataObject_StringBool* AllowBackgroundAudio = NewObject<UListDataObject_StringBool>();
			AllowBackgroundAudio->SetDataID(FName("AllowBackgroundAudio"));
			AllowBackgroundAudio->SetDataDisplayName(FText::FromString(TEXT("Allow Background Audio")));
			AllowBackgroundAudio->OverrideTrueDisplayText(FText::FromString(TEXT("Enabled")));
			AllowBackgroundAudio->OverrideFalseDisplayText(FText::FromString(TEXT("Disabled")));
			AllowBackgroundAudio->SetFalseAsDefaultValue();
			AllowBackgroundAudio->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetAllowBackgroundAudio));
			AllowBackgroundAudio->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetAllowBackgroundAudio));
			AllowBackgroundAudio->SetShouldApplyChangeImmediatly(true);

			SoundCategoryCollection->AddChildListData(AllowBackgroundAudio);
		}

		//Use HDR Audio
		{
			UListDataObject_StringBool* UseHDRAudioMode = NewObject<UListDataObject_StringBool>();
			UseHDRAudioMode->SetDataID(FName("UseHDRAudioMode"));
			UseHDRAudioMode->SetDataDisplayName(FText::FromString(TEXT("Use HDR Audio Mode")));
			UseHDRAudioMode->OverrideTrueDisplayText(FText::FromString(TEXT("Enabled")));
			UseHDRAudioMode->OverrideFalseDisplayText(FText::FromString(TEXT("Disabled")));
			UseHDRAudioMode->SetFalseAsDefaultValue();
			UseHDRAudioMode->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetUseHDRAudioMode));
			UseHDRAudioMode->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetUseHDRAudioMode));
			UseHDRAudioMode->SetShouldApplyChangeImmediatly(true);

			SoundCategoryCollection->AddChildListData(UseHDRAudioMode);
		}
	}

	RegisteredOptionsTabCollections.Add(AudioTabCollection);
}

void UOptionsDataRegistry::InitVideoCollectionTab()
{
	UListDataObject_Collection* VideoTabCollection = NewObject<UListDataObject_Collection>();
	VideoTabCollection->SetDataID(FName("VideoTabCollection"));
	VideoTabCollection->SetDataDisplayName(FText::FromString(TEXT("Video")));

	UListDataObject_StringEnum* CreatedWindowMode = nullptr;

	//Display Category
	{
		UListDataObject_Collection* DisplayCategoryCollection = NewObject<UListDataObject_Collection>();
		DisplayCategoryCollection->SetDataID(FName("DisplayCategoryCollection"));
		DisplayCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Display")));

		VideoTabCollection->AddChildListData(DisplayCategoryCollection);

		FOptionsDataEditConditionDescriptor PackagedBuildOnlyCondition;
		PackagedBuildOnlyCondition.SetEditConditionFunc(
			[]()->bool
			{
				const bool bIsInEditor = GIsEditor || GIsPlayInEditorWorld;

				return !bIsInEditor;
			}
		);
		PackagedBuildOnlyCondition.SetDisabledRichReason(TEXT("\n\n<Disabled>This setting can only be adjusted in a packaged build.</>"));

		//Window Mode
		{
			UListDataObject_StringEnum* WindowMode = NewObject<UListDataObject_StringEnum>();
			WindowMode->SetDataID(FName("WindowMode"));
			WindowMode->SetDataDisplayName(FText::FromString(TEXT("Window Mode")));
			WindowMode->SetDescriptionRichText(GET_DESCRIPTION("WindowModeDescKey"));
			WindowMode->AddEnumOption(EWindowMode::Fullscreen, FText::FromString(TEXT("Fullscreen Mode")));
			WindowMode->AddEnumOption(EWindowMode::WindowedFullscreen, FText::FromString(TEXT("Borderless Window")));
			WindowMode->AddEnumOption(EWindowMode::Windowed, FText::FromString(TEXT("Windowed")));
			WindowMode->SetDefaultValueFromEnumOption(EWindowMode::WindowedFullscreen);
			WindowMode->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetFullscreenMode));
			WindowMode->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetFullscreenMode));
			WindowMode->SetShouldApplyChangeImmediatly(true);
			WindowMode->AddEditCondition(PackagedBuildOnlyCondition);

			CreatedWindowMode = WindowMode;

			DisplayCategoryCollection->AddChildListData(WindowMode);
		}

		//Screen Resolution
		{
			UListDataObject_StringResolution* ScreenResolution = NewObject<UListDataObject_StringResolution>();
			ScreenResolution->SetDataID(FName("ScreenResolution"));
			ScreenResolution->SetDataDisplayName(FText::FromString(TEXT("Screen Resolution")));
			ScreenResolution->SetDescriptionRichText(GET_DESCRIPTION("ScreenResolutionsDescKey"));
			ScreenResolution->InitResolutionValues();
			ScreenResolution->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetScreenResolution));
			ScreenResolution->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetScreenResolution));
			ScreenResolution->SetShouldApplyChangeImmediatly(true);

			ScreenResolution->AddEditCondition(PackagedBuildOnlyCondition);

			FOptionsDataEditConditionDescriptor WindowModeEditCondition;
			WindowModeEditCondition.SetEditConditionFunc(
				[CreatedWindowMode]()->bool
				{
					const bool bIsBoderlessWindow = CreatedWindowMode->GetCurrentValueAsEnum<EWindowMode::Type>() == EWindowMode::WindowedFullscreen;

					return !bIsBoderlessWindow;
				}
			);
			WindowModeEditCondition.SetDisabledRichReason(TEXT("\n\n<Disabled>Screen Resolution is not adjustable when the 'Window Mode' is set to Borderless Window.The value must match with the maximum allowed resolution.</>"));
			WindowModeEditCondition.SetDisabledForcedStringValue(ScreenResolution->GetMaximumAllowedResolution());

			ScreenResolution->AddEditCondition(WindowModeEditCondition);

			ScreenResolution->AddEditDependencyData(CreatedWindowMode);

			DisplayCategoryCollection->AddChildListData(ScreenResolution);
		}

		//Graphics Category
		{
			UListDataObject_Collection* GraphicsCategoryCollection = NewObject<UListDataObject_Collection>();
			GraphicsCategoryCollection->SetDataID(FName("GraphicsCategoryCollection"));
			GraphicsCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Graphics")));

			VideoTabCollection->AddChildListData(GraphicsCategoryCollection);

			//Display Gamma
			{
				{
					UListDataObject_Scalar* DisplayGamma = NewObject<UListDataObject_Scalar>();
					DisplayGamma->SetDataID(FName("DisplayGamma"));
					DisplayGamma->SetDataDisplayName(FText::FromString(TEXT("Brightness")));
					DisplayGamma->SetDescriptionRichText(GET_DESCRIPTION("DisplayGammaDescKey"));
					DisplayGamma->SetDisplayValueRange(TRange<float>(0.f, 1.f));
					DisplayGamma->SetOutputValueRange(TRange<float>(1.7f, 2.7f)); //The default value Unreal has is: 2.2f
					DisplayGamma->SetSliderStepSize(0.01f);
					DisplayGamma->SetDisplayNumericType(ECommonNumericType::Percentage);
					DisplayGamma->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());
					DisplayGamma->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentDisplayGamma));
					DisplayGamma->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentDisplayGamma));
					DisplayGamma->SetDefaultValueFromString(LexToString(2.2f));

					GraphicsCategoryCollection->AddChildListData(DisplayGamma);
				}

				UListDataObject_StringInteger* CreatedOverallQuality = nullptr;

				//Overall Quality
				{
					UListDataObject_StringInteger* OverallQuality = NewObject<UListDataObject_StringInteger>();
					OverallQuality->SetDataID(FName("OverallQuality"));
					OverallQuality->SetDataDisplayName(FText::FromString(TEXT("Overall Quality")));
					OverallQuality->SetDescriptionRichText(GET_DESCRIPTION("OverallQualityDescKey"));
					OverallQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
					OverallQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
					OverallQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
					OverallQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
					OverallQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));
					OverallQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetOverallScalabilityLevel));
					OverallQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetOverallScalabilityLevel));
					OverallQuality->SetShouldApplyChangeImmediatly(true);

					GraphicsCategoryCollection->AddChildListData(OverallQuality);

					CreatedOverallQuality = OverallQuality;
				}
				//Resolution Scale
				{
					UListDataObject_Scalar* ResolutionScale = NewObject<UListDataObject_Scalar>();
					ResolutionScale->SetDataID(FName("ResolutionScale"));
					ResolutionScale->SetDataDisplayName(FText::FromString(TEXT("3D Resolution")));
					ResolutionScale->SetDescriptionRichText(GET_DESCRIPTION("ResolutionScaleDescKey"));
					ResolutionScale->SetDisplayValueRange(TRange<float>(0.f, 1.f));
					ResolutionScale->SetOutputValueRange(TRange<float>(0.f, 1.f));
					ResolutionScale->SetSliderStepSize(0.01f);
					ResolutionScale->SetDisplayNumericType(ECommonNumericType::Percentage);
					ResolutionScale->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());
					ResolutionScale->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetResolutionScaleNormalized));
					ResolutionScale->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetResolutionScaleNormalized));
					ResolutionScale->SetShouldApplyChangeImmediatly(true);

					ResolutionScale->AddEditDependencyData(CreatedOverallQuality);

					GraphicsCategoryCollection->AddChildListData(ResolutionScale);
				}

				//Global Illumination Quality
				{
					UListDataObject_StringInteger* GlobalIlluminationQuality = NewObject<UListDataObject_StringInteger>();
					GlobalIlluminationQuality->SetDataID(FName("GlobalIlluminationQuality"));
					GlobalIlluminationQuality->SetDataDisplayName(FText::FromString(TEXT("Global Illumination")));
					GlobalIlluminationQuality->SetDescriptionRichText(GET_DESCRIPTION("GlobalIlluminationQualityDescKey"));
					GlobalIlluminationQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
					GlobalIlluminationQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
					GlobalIlluminationQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
					GlobalIlluminationQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
					GlobalIlluminationQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));
					GlobalIlluminationQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetGlobalIlluminationQuality));
					GlobalIlluminationQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetGlobalIlluminationQuality));
					GlobalIlluminationQuality->SetShouldApplyChangeImmediatly(true);

					GlobalIlluminationQuality->AddEditDependencyData(CreatedOverallQuality);

					CreatedOverallQuality->AddEditDependencyData(GlobalIlluminationQuality);

					GraphicsCategoryCollection->AddChildListData(GlobalIlluminationQuality);
				}

				//Shadow Quality
				{
					UListDataObject_StringInteger* ShadowQuality = NewObject<UListDataObject_StringInteger>();
					ShadowQuality->SetDataID(FName("ShadowQuality"));
					ShadowQuality->SetDataDisplayName(FText::FromString(TEXT("Shadow Quality")));
					ShadowQuality->SetDescriptionRichText(GET_DESCRIPTION("ShadowQualityDescKey"));
					ShadowQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
					ShadowQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
					ShadowQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
					ShadowQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
					ShadowQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));
					ShadowQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetShadowQuality));
					ShadowQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetShadowQuality));
					ShadowQuality->SetShouldApplyChangeImmediatly(true);

					ShadowQuality->AddEditDependencyData(CreatedOverallQuality);

					CreatedOverallQuality->AddEditDependencyData(ShadowQuality);

					GraphicsCategoryCollection->AddChildListData(ShadowQuality);
				}

				//AntiAliasing Quality
				{
					UListDataObject_StringInteger* AntiAliasingQuality = NewObject<UListDataObject_StringInteger>();
					AntiAliasingQuality->SetDataID(FName("AntiAliasingQuality"));
					AntiAliasingQuality->SetDataDisplayName(FText::FromString(TEXT("Anti Aliasing")));
					AntiAliasingQuality->SetDescriptionRichText(GET_DESCRIPTION("AntiAliasingDescKey"));
					AntiAliasingQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
					AntiAliasingQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
					AntiAliasingQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
					AntiAliasingQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
					AntiAliasingQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));
					AntiAliasingQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetAntiAliasingQuality));
					AntiAliasingQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetAntiAliasingQuality));
					AntiAliasingQuality->SetShouldApplyChangeImmediatly(true);

					AntiAliasingQuality->AddEditDependencyData(CreatedOverallQuality);

					CreatedOverallQuality->AddEditDependencyData(AntiAliasingQuality);

					GraphicsCategoryCollection->AddChildListData(AntiAliasingQuality);
				}

				//View Distance Quality
				{
					UListDataObject_StringInteger* ViewDistanceQuality = NewObject<UListDataObject_StringInteger>();
					ViewDistanceQuality->SetDataID(FName("ViewDistanceQuality"));
					ViewDistanceQuality->SetDataDisplayName(FText::FromString(TEXT("View Distance")));
					ViewDistanceQuality->SetDescriptionRichText(GET_DESCRIPTION("ViewDistanceDescKey"));
					ViewDistanceQuality->AddIntegerOption(0, FText::FromString(TEXT("Near")));
					ViewDistanceQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
					ViewDistanceQuality->AddIntegerOption(2, FText::FromString(TEXT("Far")));
					ViewDistanceQuality->AddIntegerOption(3, FText::FromString(TEXT("Very Far")));
					ViewDistanceQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));
					ViewDistanceQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetViewDistanceQuality));
					ViewDistanceQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetViewDistanceQuality));
					ViewDistanceQuality->SetShouldApplyChangeImmediatly(true);

					ViewDistanceQuality->AddEditDependencyData(CreatedOverallQuality);

					CreatedOverallQuality->AddEditDependencyData(ViewDistanceQuality);

					GraphicsCategoryCollection->AddChildListData(ViewDistanceQuality);
				}

				//Texture Quality
				{
					UListDataObject_StringInteger* TextureQuality = NewObject<UListDataObject_StringInteger>();
					TextureQuality->SetDataID(FName("TextureQuality"));
					TextureQuality->SetDataDisplayName(FText::FromString(TEXT("Texture Quality")));
					TextureQuality->SetDescriptionRichText(GET_DESCRIPTION("TextureQualityDescKey"));
					TextureQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
					TextureQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
					TextureQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
					TextureQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
					TextureQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));
					TextureQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetTextureQuality));
					TextureQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetTextureQuality));
					TextureQuality->SetShouldApplyChangeImmediatly(true);

					TextureQuality->AddEditDependencyData(CreatedOverallQuality);

					CreatedOverallQuality->AddEditDependencyData(TextureQuality);

					GraphicsCategoryCollection->AddChildListData(TextureQuality);
				}

				//Visual Effects Quality
				{
					UListDataObject_StringInteger* VisualEffectQuality = NewObject<UListDataObject_StringInteger>();
					VisualEffectQuality->SetDataID(FName("VisualEffectQuality"));
					VisualEffectQuality->SetDataDisplayName(FText::FromString(TEXT("Visual Effect Quality")));
					VisualEffectQuality->SetDescriptionRichText(GET_DESCRIPTION("VisualEffectQualityDescKey"));
					VisualEffectQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
					VisualEffectQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
					VisualEffectQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
					VisualEffectQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
					VisualEffectQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));
					VisualEffectQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetVisualEffectQuality));
					VisualEffectQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetVisualEffectQuality));
					VisualEffectQuality->SetShouldApplyChangeImmediatly(true);

					VisualEffectQuality->AddEditDependencyData(CreatedOverallQuality);

					CreatedOverallQuality->AddEditDependencyData(VisualEffectQuality);

					GraphicsCategoryCollection->AddChildListData(VisualEffectQuality);
				}

				//Reflection Quality
				{
					UListDataObject_StringInteger* ReflectionQuality = NewObject<UListDataObject_StringInteger>();
					ReflectionQuality->SetDataID(FName("ReflectionQuality"));
					ReflectionQuality->SetDataDisplayName(FText::FromString(TEXT("Reflection Quality")));
					ReflectionQuality->SetDescriptionRichText(GET_DESCRIPTION("ReflectionQualityDescKey"));
					ReflectionQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
					ReflectionQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
					ReflectionQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
					ReflectionQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
					ReflectionQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));
					ReflectionQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetReflectionQuality));
					ReflectionQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetReflectionQuality));
					ReflectionQuality->SetShouldApplyChangeImmediatly(true);

					ReflectionQuality->AddEditDependencyData(CreatedOverallQuality);

					CreatedOverallQuality->AddEditDependencyData(ReflectionQuality);

					GraphicsCategoryCollection->AddChildListData(ReflectionQuality);
				}

				//Post Processing Quality
				{
					UListDataObject_StringInteger* PostProcessingQuality = NewObject<UListDataObject_StringInteger>();
					PostProcessingQuality->SetDataID(FName("PostProcessingQuality"));
					PostProcessingQuality->SetDataDisplayName(FText::FromString(TEXT("Post Processing Quality")));
					PostProcessingQuality->SetDescriptionRichText(GET_DESCRIPTION("PostProcessingQualityDescKey"));
					PostProcessingQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
					PostProcessingQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
					PostProcessingQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
					PostProcessingQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
					PostProcessingQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));
					PostProcessingQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetPostProcessingQuality));
					PostProcessingQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetPostProcessingQuality));
					PostProcessingQuality->SetShouldApplyChangeImmediatly(true);

					PostProcessingQuality->AddEditDependencyData(CreatedOverallQuality);

					CreatedOverallQuality->AddEditDependencyData(PostProcessingQuality);

					GraphicsCategoryCollection->AddChildListData(PostProcessingQuality);
				}
				//Advanced Graphics Category
				{
					UListDataObject_Collection* AdvancedGraphicsCategoryCollection = NewObject<UListDataObject_Collection>();
					AdvancedGraphicsCategoryCollection->SetDataID(FName("AdvancedGraphicsCategoryCollection"));
					AdvancedGraphicsCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Advanced Graphics")));

					VideoTabCollection->AddChildListData(AdvancedGraphicsCategoryCollection);

					//Vertical Sync
					{
						UListDataObject_StringBool* VerticalSync = NewObject<UListDataObject_StringBool>();
						VerticalSync->SetDataID(FName("VerticalSync"));
						VerticalSync->SetDataDisplayName(FText::FromString(TEXT("V-Sync")));
						VerticalSync->SetDescriptionRichText(GET_DESCRIPTION("VerticalSyncDescKey"));
						VerticalSync->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(IsVSyncEnabled));
						VerticalSync->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetVSyncEnabled));
						VerticalSync->SetFalseAsDefaultValue();
						VerticalSync->SetShouldApplyChangeImmediatly(true);

						FOptionsDataEditConditionDescriptor FullscreenOnlyCondition;
						FullscreenOnlyCondition.SetEditConditionFunc(
							[CreatedWindowMode]()->bool
							{
								return CreatedWindowMode->GetCurrentValueAsEnum<EWindowMode::Type>() == EWindowMode::Fullscreen;
							}
						);
						FullscreenOnlyCondition.SetDisabledRichReason(TEXT("\n\n<Disabled>This feature only works if the 'Window Mode' is set to 'Fullscreen'.</>"));
						FullscreenOnlyCondition.SetDisabledForcedStringValue(TEXT("false"));

						VerticalSync->AddEditCondition(FullscreenOnlyCondition);

						AdvancedGraphicsCategoryCollection->AddChildListData(VerticalSync);
					}

					//Frame Rate Limit
					{
						UListDataObject_String* FrameRateLimit = NewObject<UListDataObject_String>();
						FrameRateLimit->SetDataID(FName("FrameRateLimit"));
						FrameRateLimit->SetDataDisplayName(FText::FromString(TEXT("Frame Rate Limit")));
						FrameRateLimit->SetDescriptionRichText(GET_DESCRIPTION("FrameRateLimitDescKey"));
						FrameRateLimit->AddDynamicOption(LexToString(30.f), FText::FromString(TEXT("30 FPS")));
						FrameRateLimit->AddDynamicOption(LexToString(60.f), FText::FromString(TEXT("60 FPS")));
						FrameRateLimit->AddDynamicOption(LexToString(90.f), FText::FromString(TEXT("90 FPS")));
						FrameRateLimit->AddDynamicOption(LexToString(120.f), FText::FromString(TEXT("120 FPS")));
						FrameRateLimit->AddDynamicOption(LexToString(0.f), FText::FromString(TEXT("No Limit")));
						FrameRateLimit->SetDefaultValueFromString(LexToString(0.f));
						FrameRateLimit->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetFrameRateLimit));
						FrameRateLimit->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetFrameRateLimit));
						FrameRateLimit->SetShouldApplyChangeImmediatly(true);

						AdvancedGraphicsCategoryCollection->AddChildListData(FrameRateLimit);
					}
				}
			}
		}
	}

	RegisteredOptionsTabCollections.Add(VideoTabCollection);
}

void UOptionsDataRegistry::InitControlCollectionTab()
{
	UListDataObject_Collection* ControlTabCollection = NewObject<UListDataObject_Collection>();
	ControlTabCollection->SetDataID(FName("ControlTabCollection"));
	ControlTabCollection->SetDataDisplayName(FText::FromString(TEXT("Control")));

	RegisteredOptionsTabCollections.Add(ControlTabCollection);
}
