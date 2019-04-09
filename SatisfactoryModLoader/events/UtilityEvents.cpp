#include <stdafx.h>
#include <other/BaseMod.h>
#include <events/UtilityEvents.h>
#include <utility/Logger.h>

namespace SML {
	FString* UtilityEvents::FTextToStringHook(void* text) {
		auto returns = Run(event::FTextToStringGlobal, text);
		if (!returns.UseOriginalFunction) {
			return (FString*)returns.ReturnValue;
		}
		return ((FString*(WINAPI*)(void*))globals.functions[event::FTextToString])(text);
	}

	FString* UtilityEvents::FTextToStringGlobal(void* text) {
		return ((FString*(WINAPI*)(void*))globals.functions[event::FTextToString])(text);
	}

	const wchar_t* UtilityEvents::BuildSettingsGetBuildVersionHook() {
		return ((const wchar_t*(WINAPI*)())globals.functions[event::BuildSettingsGetBuildVersion])();
	}

	std::string UtilityEvents::BuildSettingsGetBuildVersionGlobal() {
		auto pointer = (const wchar_t*(WINAPI*)())globals.functions[event::BuildSettingsGetBuildVersion];
		std::wstring version(pointer());
		std::string str(version.begin(), version.end());
		return str;
	}

	void UtilityEvents::Setup(HookLoader* hookLoader) {
		SML::info("UtilityEvents::Setup");

		// FText
		hookLoader->HookEvent(event::FTextToString, "FText::ToString", this->FTextToStringHook);
		hookLoader->HookEvent(event::FTextToStringGlobal, "", this->FTextToStringGlobal);

		// BuildSettings
		hookLoader->HookEvent(event::BuildSettingsGetBuildVersion, "BuildSettings::GetBuildVersion", this->BuildSettingsGetBuildVersionHook);
		hookLoader->HookEvent(event::BuildSettingsGetBuildVersionGlobal, "", this->BuildSettingsGetBuildVersionGlobal);

		hookLoader->CheckVersion(this->BuildSettingsGetBuildVersionGlobal());
	}
}