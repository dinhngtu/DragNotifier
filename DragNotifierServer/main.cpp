#include "pch.h"
#include <atlbase.h>
#include "CDragNotifierCopyHook.hpp"

static DWORD registered;
static HANDLE evt;

struct DECLSPEC_UUID("{3FDC627C-9839-4BA8-923B-AA1E3F2D9A3F}") CDragNotifierFactory : winrt::implements<CDragNotifierFactory, IClassFactory> {
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

static BOOL WINAPI CtrlHandler(_In_ DWORD CtrlType) {
    return SetEvent(evt);
}

int wmain(int argc, wchar_t** argv) {
    winrt::init_apartment();
    HRESULT hr = CoRegisterClassObject(
        _uuidof(CDragNotifierFactory),
        winrt::make<CDragNotifierFactory>().get(),
        CLSCTX_LOCAL_SERVER,
        REGCLS_MULTIPLEUSE,
        &registered);
    winrt::check_hresult(hr);
    evt = winrt::check_pointer(CreateEvent(NULL, FALSE, FALSE, NULL));
    SetConsoleCtrlHandler(CtrlHandler, TRUE);
    wprintf(L"booted, waiting for event\n");
    wprintf(L"DRAGNOTIFIER_PREFIX = {83120B49-8EFA-4CDE-8AFD-2954AE52B4A3}\n");
    wprintf(L"CLSID = {3FDC627C-9839-4BA8-923B-AA1E3F2D9A3F}\n");
    WaitForSingleObject(evt, INFINITE);
    CoRevokeClassObject(registered);
    return 0;
}
