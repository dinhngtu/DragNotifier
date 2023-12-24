#include "pch.h"
#include <exception>
#include <filesystem>
#include <fmt/core.h>
#include <fmt/xchar.h>
#include "CDragNotifierCopyHook.hpp"

static const std::wstring DRAGNOTIFIER_PREFIX = L"{83120B49-8EFA-4CDE-8AFD-2954AE52B4A3}"s;
static constexpr ULONG_PTR DRAGNOTIFIER_COPY = 0x83120B49;

IFACEMETHODIMP_(UINT) CDragNotifierCopyHook::CopyCallback(
    _In_opt_ HWND hwnd,
    UINT wFunc,
    UINT wFlags,
    _In_ PCWSTR pszSrcFile,
    DWORD dwSrcAttribs,
    _In_opt_ PCWSTR pszDestFile,
    DWORD dwDestAttribs) {
    auto f = fmt::format(
        L"hwnd: {:#x}, func: {:#x}, flags: {:#x}, srcattr: {:#x}, destattr: {:#x}, src: {}, dest: {}",
        (uintptr_t)hwnd,
        wFunc,
        wFlags,
        dwSrcAttribs,
        dwDestAttribs,
        pszSrcFile,
        pszDestFile);
    OutputDebugStringW(f.c_str());
    try {
        if (wFunc == FO_COPY || wFunc == FO_MOVE) {
            std::filesystem::path srcpath(pszSrcFile);
            std::filesystem::path dstpath(pszDestFile);
            if (srcpath.stem() == DRAGNOTIFIER_PREFIX) {
                auto hwndString = srcpath.extension().wstring();
                if (hwndString.length() > 1) {
                    auto hwndDest = reinterpret_cast<HWND>(std::stoull(hwndString.substr(1)));
                    auto dest = dstpath.parent_path();
                    f = fmt::format(L"hwnd: {:#x}, path: {}", (uintptr_t)hwndDest, dest.wstring());
                    OutputDebugStringW(f.c_str());
                    DragNotifierCopyData data{};
                    if (SUCCEEDED(StringCchCopyW(&data.filename[0], ARRAYSIZE(data.filename), dest.c_str()))) {
                        COPYDATASTRUCT cds{ DRAGNOTIFIER_COPY, sizeof(DragNotifierCopyData), &data };
                        DWORD_PTR res;
                        if (SendMessageTimeoutW(hwndDest, WM_COPYDATA, reinterpret_cast<WPARAM>(hwnd), reinterpret_cast<LPARAM>(&cds), SMTO_ABORTIFHUNG, 1000, &res) && res)
                            return IDNO;
                    }
                }
            }
        }
        return IDYES;
    }
    catch (const std::exception& e) {
        OutputDebugStringA(e.what());
        return IDYES;
    }
}
