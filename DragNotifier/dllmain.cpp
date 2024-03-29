// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
/*
#include <wil/registry.h>
#include <wil/win32_helpers.h>
#include <wil/stl.h>
*/
#include "CDragNotifierCopyHook.hpp"

HINSTANCE g_hInstance = NULL;

BOOL WINAPI DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        g_hInstance = hModule;
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

struct DECLSPEC_UUID("{D8D7AC18-961F-4CE0-99AF-CF471B87ECA0}") CDragNotifierFactory : winrt::implements<CDragNotifierFactory, IClassFactory> {
    IFACEMETHODIMP CreateInstance(IUnknown * pUnkOuter, REFIID riid, void** ppvObject) WIN_NOEXCEPT {
        try {
            return winrt::make<CDragNotifierCopyHook>()->QueryInterface(riid, ppvObject);
        }
        catch (...) {
            return winrt::to_hresult();
        }
    }

    IFACEMETHODIMP LockServer(BOOL fLock) WIN_NOEXCEPT {
        if (fLock)
            ++winrt::get_module_lock();
        else
            --winrt::get_module_lock();
        return S_OK;
    }
};

_Use_decl_annotations_ STDAPI DllCanUnloadNow() {
    if (winrt::get_module_lock())
        return S_FALSE;
    winrt::clear_factory_cache();
    return S_OK;
}

_Use_decl_annotations_ STDAPI DllGetClassObject(_In_ REFCLSID rclsid, _In_ REFIID riid, _Outptr_ LPVOID* ppv) {
    try
    {
        *ppv = NULL;
        if (rclsid == __uuidof(CDragNotifierFactory))
            return winrt::make<CDragNotifierFactory>()->QueryInterface(riid, ppv);
        return E_INVALIDARG;
    }
    catch (...) {
        return winrt::to_hresult();
    }
}

/*
STDAPI DllRegisterServer() {
    try
    {
        wchar_t factoryGuidStr[wil::guid_string_buffer_length];
        if (!StringFromGUID2(__uuidof(CDragNotifierFactory), factoryGuidStr, ARRAYSIZE(factoryGuidStr)))
            winrt::throw_hresult(E_OUTOFMEMORY);

        {
            auto clsidKeyName = L"CLSID\\"s + factoryGuidStr;
            auto clsidKey = wil::reg::create_unique_key(HKEY_CLASSES_ROOT, clsidKeyName.c_str(), wil::reg::key_access::readwrite);
            {
                auto server = wil::reg::create_unique_key(clsidKey.get(), L"InprocServer32", wil::reg::key_access::readwrite);
                auto path = wil::GetModuleFileNameW<std::wstring>(g_hInstance);
                wil::reg::set_value_string(server.get(), NULL, path.c_str());
                wil::reg::set_value_string(server.get(), L"ThreadingModel", L"Apartment");
            }
        }
        {
            auto handlerKeyName = L"Directory\\shellex\\CopyHookHandlers\\"s + factoryGuidStr;
            auto handler = wil::reg::create_unique_key(HKEY_CLASSES_ROOT, handlerKeyName.c_str(), wil::reg::key_access::readwrite);
            wil::reg::set_value_string(handler.get(), NULL, factoryGuidStr);
        }

        return S_OK;
    }
    catch (...) {
        return winrt::to_hresult();
    }
}

STDAPI DllUnregisterServer() {
    try {
        LSTATUS status;

        wchar_t factoryGuidStr[wil::guid_string_buffer_length];
        if (!StringFromGUID2(__uuidof(CDragNotifierFactory), factoryGuidStr, ARRAYSIZE(factoryGuidStr)))
            winrt::throw_hresult(E_OUTOFMEMORY);

        auto handlerKeyName = L"Directory\\shellex\\CopyHookHandlers\\"s + factoryGuidStr;
        status = RegDeleteKeyExW(HKEY_CLASSES_ROOT, handlerKeyName.c_str(), 0, 0);
        if (status != ERROR_FILE_NOT_FOUND)
            winrt::check_win32(status);

        auto serverKeyName = L"CLSID\\"s + factoryGuidStr + L"\\InprocServer32";
        status = RegDeleteKeyExW(HKEY_CLASSES_ROOT, serverKeyName.c_str(), 0, 0);
        if (status != ERROR_FILE_NOT_FOUND)
            winrt::check_win32(status);

        auto clsidKeyName = L"CLSID\\"s + factoryGuidStr;
        status = RegDeleteKeyExW(HKEY_CLASSES_ROOT, clsidKeyName.c_str(), 0, 0);
        if (status != ERROR_FILE_NOT_FOUND)
            winrt::check_win32(status);

        return S_OK;
    }
    catch (...) {
        return winrt::to_hresult();
    }
}
*/