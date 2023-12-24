# Drag Notifier Shell Extension

To use the extension, your origin program must create a directory with the following path:

    <something>\{83120B49-8EFA-4CDE-8AFD-2954AE52B4A3}.<HWND>
    
where `<HWND>` is the destination program's HWND.
The actual path doesn't matter as long as it points to an existing directory. (Explorer won't run the shell copy hook otherwise)

The extension will send back a `WM_COPYDATA` message containing the destination path.
