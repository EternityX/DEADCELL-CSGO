#pragma once

// note - dex;  these are not complete structs, they only contain needed things for this project.
//              all of these structs are taken from x86-64 Windows 7 Home Premium ( Service Pack 1 ).

namespace pe {
	namespace types {
		enum EXCEPTION_DISPOSITION : size_t {
			ExceptionContinueExecution = 0,
			ExceptionContinueSearch,
			ExceptionNestedException,
			ExceptionCollidedUnwind
		};

		struct EXCEPTION_REGISTRATION_RECORD {
			EXCEPTION_REGISTRATION_RECORD *Next;
			EXCEPTION_DISPOSITION Handler;
		};

		struct NT_TIB {
			EXCEPTION_REGISTRATION_RECORD *ExceptionList;
			uintptr_t StackBase;
			uintptr_t StackLimit;
			uintptr_t SubSystemTib;

			union {
				uintptr_t FiberData;
				uint32_t Version;
			};

			uintptr_t ArbitraryUserPointer;
			NT_TIB *Self;
		};

		struct CLIENT_ID {
			uintptr_t UniqueProcess;
			uintptr_t UniqueThread;
		};

		struct LIST_ENTRY {
			LIST_ENTRY *Flink;
			LIST_ENTRY *Blink;
		};

		struct PEB_LDR_DATA {
			uint32_t Length;
			uint8_t Initialized;
			uintptr_t SsHandle;
			LIST_ENTRY InLoadOrderModuleList;
			LIST_ENTRY InMemoryOrderModuleList;
		};

		struct UNICODE_STRING {
			uint16_t Length;
			uint16_t MaximumLength;
			wchar_t *Buffer;
		};

		struct ACTIVATION_CONTEXT {
			// todo - dex; no symbols for this?
		};

		struct LDR_DATA_TABLE_ENTRY {
			LIST_ENTRY InLoadOrderLinks;
			LIST_ENTRY InMemoryOrderLinks;
			LIST_ENTRY InInitializationOrderLinks;
			uintptr_t DllBase;
			uintptr_t EntryPoint;
			uint32_t SizeOfImage;
			UNICODE_STRING FullDllName;
			UNICODE_STRING BaseDllName;
		};

		// Process Environment Block.
		struct PEB {
			uint8_t InheritedAddressSpace;
			uint8_t ReadImageFileExecOptions;
			uint8_t BeingDebugged;

			union {
				uint8_t BitField;

				struct {
					uint8_t ImageUsesLargePages : 1;
					uint8_t IsProtectedProcess : 1;
					uint8_t IsLegacyProcess : 1;
					uint8_t IsImageDynamicallyRelocated : 1;
					uint8_t SkipPatchingUser32Forwarders : 1;
					uint8_t SpareBits : 1 /* 3 */;
				};
			};

			uintptr_t Mutant;
			uintptr_t ImageBaseAddress;
			PEB_LDR_DATA *Ldr;
		};

		// Thread Environment Block ( aka Thread Information Block ).
		struct TEB {
			NT_TIB NtTib;
			uintptr_t EnvironmentPointer;
			CLIENT_ID ClientId;
			uintptr_t ActiveRpcHandle;
			uintptr_t ThreadLocalStoragePointer;
			PEB *ProcessEnvironmentBlock;
			uint32_t LastErrorValue;
			uint32_t CountOfOwnedCriticalSections;
			uintptr_t CsrClientThread;
			uintptr_t Win32ThreadInfo;
			uint32_t User32Reserved[ 26 ];
			uint32_t UserReserved[ 5 ];
			uintptr_t WOW32Reserved; // note - dex; some syscalls use this, but it depends on architecture.
		};
	}
}
