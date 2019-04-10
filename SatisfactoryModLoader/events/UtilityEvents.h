#pragma once

#include "HookLoader.h"

#pragma warning (push)
#pragma warning (disable: 4667)

namespace SML {
	struct FString;

	class FText {
	public:
		// ; FString *__fastcall FText::ToString(FText *this)
		FString* ToString();
	};

	DEFINE_METHOD(FText::ToString);

	class BuildSettings {
	public:
		// ; const wchar_t *__fastcall BuildSettings::GetBuildVersion()
		static const wchar_t* GetBuildVersion();

	};

	DEFINE_METHOD(BuildSettings::GetBuildVersion);
}

#pragma warning (pop)
