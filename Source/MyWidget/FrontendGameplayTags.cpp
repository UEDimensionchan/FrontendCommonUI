// Fill out your copyright notice in the Description page of Project Settings.


#include "FrontendGameplayTags.h"

namespace FrontendGameplayTags
{
	//Frontend Widget Stack
	UE_DEFINE_GAMEPLAY_TAG(Frontend_WidgetStack_Modal, "Frontend.WidgetStack.Modal");
	UE_DEFINE_GAMEPLAY_TAG(Frontend_WidgetStack_GameMenul, "Frontend.WidgetStack.GameMenul");
	UE_DEFINE_GAMEPLAY_TAG(Frontend_WidgetStack_GameHub, "Frontend.WidgetStack.GameHub");
	UE_DEFINE_GAMEPLAY_TAG(Frontend_WidgetStack_Frontend, "Frontend.WidgetStack.Frontend");

	//Frontend Widgets
	UE_DEFINE_GAMEPLAY_TAG(Frontend_Widget_PressAnyKeyScreen, "Frontend.Widget.PressAnyKeyScreen");
	UE_DEFINE_GAMEPLAY_TAG(Frontend_Widget_MainMenuScreen, "Frontend.Widget.MainMenuScreen");
	UE_DEFINE_GAMEPLAY_TAG(Frontend_Widget_OptionsScreen, "Frontend.Widget.OptionsScreen");

	UE_DEFINE_GAMEPLAY_TAG(Frontend_Widget_ConfirmScreen, "Frontend.Widget.ConfirmScreen");

	UE_DEFINE_GAMEPLAY_TAG(Frontend_Widget_KeyRemapScreen, "Frontend.Widget.KeyRemapScreen");

	UE_DEFINE_GAMEPLAY_TAG(Frontend_Image_TestImage, "Frontend.Image.TestImage");
}