#pragma once

#include "pch.h"

struct DECLSPEC_UUID("{7F4FD2EA-8CC8-43C4-8440-CD76805B4E95}") CDragNotifierCopyHook : winrt::implements<CDragNotifierCopyHook, ICopyHookW> {
    STDMETHOD_(UINT, CopyCallback)(
        _In_opt_ HWND hwnd,
        UINT wFunc,
        UINT wFlags,
        _In_ PCWSTR pszSrcFile,
        DWORD dwSrcAttribs,
        _In_opt_ PCWSTR pszDestFile,
        DWORD dwDestAttribs);
};

struct DragNotifierCopyData {
    wchar_t filename[MAX_PATH];
};
