#pragma once

#include "ModReturns.h"
#include <utility/DetoursFwd.h>

#include <vector>
#include <functional>

#if defined(HOOKLOADER_EXPORTS)
#if defined(SATISFACTORYMODLOADER_EXPORTS)
#define SML_HOOK_API __declspec(dllexport)
#else
#error Exports must only be done from ModLoader dll
#endif

// Make method available for hooking in mod launcher dll.
// Usage: EXPORT_METHOD(&Foo::Bar).
#define EXPORT_METHOD(PMF) \
template struct SML_HOOK_API HookInvoker<decltype(&PMF), &PMF>; \
template SML_HOOK_API void Subscribe<&PMF>(HookInvoker<decltype(&PMF), &PMF>::Handler handler);
#else
#if defined(SATISFACTORYMODLOADER_EXPORTS)
#define SML_HOOK_API
#else
#define SML_HOOK_API __declspec(dllimport)
#endif

// Import method from mod launcher dll.
// Usage: EXPORT_METHOD(&Foo::Bar).
#define EXPORT_METHOD(PMF) \
extern template struct SML_HOOK_API HookInvoker<decltype(&PMF), &PMF>; \
extern template SML_HOOK_API void Subscribe<&PMF>(HookInvoker<decltype(&PMF), &PMF>::Handler handler);
#endif

namespace SML {
	template <auto PMF>
	struct HookName;

	// Make method available for hooking in mod API.
	// Usage: DEFINE_METHOD(&Foo::Bar).
	#define DEFINE_METHOD(PMF) \
	template <> struct HookName<&PMF> { static constexpr const char Name[] = #PMF; }; \
	EXPORT_METHOD(PMF)

	template <typename T, T>
	struct HookInvoker;

	template <auto PMF, typename Handler>
	struct HandlerStorage {
		static std::vector<Handler> handlers;
	};

	template <auto PMF, typename Handler>
	std::vector<Handler> HandlerStorage<PMF, Handler>::handlers;

	template <typename R, typename... A, R(*PMF)(A...)>
	class HookInvoker<R(*)(A...), PMF> {
	public:
		// mod handler function
		typedef R HandlerSignature(ModReturns*, A...);

		typedef R ReturnType;

		// support arbitrary context for handlers
		typedef std::function<HandlerSignature> Handler;

		// if std::function seems like too much overhead, this typedef can be used for raw function pointers instead
		// typedef HandlerSignature* Handler;

	private:
		// detoured function
		typedef R __fastcall HookType(A...);

		static void* original;

		template <typename X>
		static HookType* GetApply(std::true_type) {
			return &ApplyVoid<X>;
		}

		template <typename X>
		static HookType* GetApply(std::false_type) {
			return &Apply<X>;
		}

		template <typename X>
		static R __fastcall Apply(A... args) {
			ModReturns returns;
			returns.UseOriginalFunction = true;

			R ret{};

			for(auto&& handler : HandlerStorage<PMF, Handler>::handlers)
				ret = handler(&returns, args...);

			if(returns.UseOriginalFunction)
				return ((HookType*)original)(args...);

			return ret;
		}

		template <typename X>
		static void __fastcall ApplyVoid(A... args) {
			ModReturns returns;
			returns.UseOriginalFunction = true;

			for(auto&& handler : HandlerStorage<PMF, Handler>::handlers)
				handler(&returns, args...);

			if(returns.UseOriginalFunction)
				((HookType*)original)(args...);
		}

		static constexpr const char _gameModule[] = "FactoryGame-Win64-Shipping.exe";

		static void InstallHook() {
			// only if not already installed
			if(!original) {
				Detours::DetourTransactionBegin();
				original = Detours::DetourFindFunction(_gameModule, HookName<PMF>::Name);
				Detours::DetourAttach(&original, (void*)GetApply<R>(std::is_same<R, void>{}));
				Detours::DetourTransactionCommit();
			}
		}

	public:
		static void Subscribe(Handler handler) {
			InstallHook();
			HandlerStorage<PMF, Handler>::handlers.push_back(handler);
		}

		static HookType* GetOriginal() { return (HookType*)original; }
	};

	template <typename R, typename... A, R(*PMF)(A...)>
	void* HookInvoker<R(*)(A...), PMF>::original;

	template <typename R, typename C, typename... A, R(C::*PMF)(A...)>
	class HookInvoker<R(C::*)(A...), PMF> {
	public:
		// mod handler function
		typedef R HandlerSignature(ModReturns*, C*, A...);

		typedef R ReturnType;

		// support arbitrary context for handlers
		typedef std::function<HandlerSignature> Handler;

		// if std::function seems like too much overhead, this typedef can be used for raw function pointers instead
		// typedef HandlerSignature* Handler;

	private:
		// detoured function
		typedef R __fastcall HookType(C*, A...);

		static void* original;

		template <typename X>
		static HookType* GetApply(std::true_type) {
			return &ApplyVoid<X>;
		}

		template <typename X>
		static HookType* GetApply(std::false_type) {
			return &Apply<X>;
		}

		template <typename X>
		static R __fastcall Apply(C* thiz, A... args) {
			ModReturns returns;
			returns.UseOriginalFunction = true;

			R ret{};

			for(auto&& handler : HandlerStorage<PMF, Handler>::handlers)
				ret = handler(&returns, thiz, args...);

			if(returns.UseOriginalFunction)
				return ((HookType*)original)(thiz, args...);

			return ret;
		}

		template <typename X>
		static void __fastcall ApplyVoid(C* thiz, A... args) {
			ModReturns returns;
			returns.UseOriginalFunction = true;

			for(auto&& handler : HandlerStorage<PMF, Handler>::handlers)
				handler(&returns, thiz, args...);

			if(returns.UseOriginalFunction)
				((HookType*)original)(thiz, args...);
		}

		static constexpr const char _gameModule[] = "FactoryGame-Win64-Shipping.exe";

		static void InstallHook() {
			// only if not already installed
			if(!original) {
				Detours::DetourTransactionBegin();
				original = Detours::DetourFindFunction(_gameModule, HookName<PMF>::Name);
				Detours::DetourAttach(&original, (void*)GetApply<R>(std::is_same<R, void>{}));
				Detours::DetourTransactionCommit();
			}
		}

	public:
		static void Subscribe(Handler handler) {
			InstallHook();
			HandlerStorage<PMF, Handler>::handlers.push_back(handler);
		}

		static HookType* GetOriginal() {
			InstallHook();
			return (HookType*)original;
		}
	};

	template <typename R, typename C, typename... A, R(C::*PMF)(A...)>
	void* HookInvoker<R(C::*)(A...), PMF>::original;

	template <auto HookableFunction>
	void Subscribe(typename HookInvoker<decltype(HookableFunction), HookableFunction>::Handler handler) {
		HookInvoker<decltype(HookableFunction), HookableFunction>::Subscribe(handler);
	}

	template <auto HookableFunction, typename... A>
	typename HookInvoker<decltype(HookableFunction), HookableFunction>::ReturnType
	Call(A... args) {
		return HookInvoker<decltype(HookableFunction), HookableFunction>::GetOriginal()(args...);
	}
}