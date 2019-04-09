#pragma once

#include <Windows.h>
#include <events/BaseEvent.h>

class FString;
class Globals;

namespace SML {
	class HookLoader;

	class UtilityEvents : BaseEvent {
	public:
		// ; FString *__fastcall FText::ToString(FText *this)
		static FString* FTextToStringHook(void* text);

		// ; FString *__fastcall FText::ToString(FText *this)
		static FString* FTextToStringGlobal(void* text);

		// ; const wchar_t *__fastcall BuildSettings::GetBuildVersion()
		static const wchar_t* BuildSettingsGetBuildVersionHook();

		// ; const wchar_t *__fastcall BuildSettings::GetBuildVersion()
		static std::string BuildSettingsGetBuildVersionGlobal();

		void Setup(HookLoader* hookLoader);
	};
}