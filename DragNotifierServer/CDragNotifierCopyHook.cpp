#include "pch.h"
#include <exception>
#include <filesystem>
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
    wprintf(L"hwnd %p func 0x%x flags 0x%x srca 0x%lx dsta 0x%lx\n", hwnd, wFunc, wFlags, dwSrcAttribs, dwDestAttribs);
    wprintf(L"src %s dest %s\n", pszSrcFile, pszDestFile);
    if (wFunc == FO_COPY || wFunc == FO_MOVE) {
        std::filesystem::path srcpath(pszSrcFile);
        std::filesystem::path dstpath(pszDestFile);
        if (srcpath.stem() == DRAGNOTIFIER_PREFIX) {
            auto hwndString = srcpath.extension().wstring();
            if (hwndString.length() > 1) {
                auto hwndDest = reinterpret_cast<HWND>(std::stoull(hwndString.substr(1)));
                auto dest = dstpath.parent_path();
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
