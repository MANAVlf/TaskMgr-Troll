# TaskMgr-Troll 🎯

**⚠️ Old source code from ~2 years ago. Educational only.**

Windows DLL that trolls Task Manager by showing "TROLLED" and "EVILBYTECODE ON GITHUB!" instead of processes.

## How It Works 🔧

### The Method
- **Window Detection**: Finds Task Manager by title
- **Hook Installation**: `SetWindowsHookEx(WH_CALLWNDPROC)` 
- **Procedure Replacement**: Swaps window procedure with custom one
- **Message Blocking**: Blocks mouse, scroll, drag messages
- **Timer Persistence**: Repaints every 100ms

### What It Actually Does
```cpp
// Detects Task Manager
if (strstr(title, "Task Manager")) return TRUE;

// Hooks window creation
g_hook = SetWindowsHookEx(WH_CALLWNDPROC, CallWndProc, ...);

// Replaces window procedure  
SetWindowLongPtr(hwnd, GWLP_WNDPROC, WindowProc);

// Blocks user interaction
if (uMsg == WM_MOUSEMOVE || uMsg == WM_HSCROLL...) return 0;

// Paints "TROLLED"
DrawTextA(hdc, "TROLLED", -1, &rect, DT_CENTER | DT_VCENTER);
```

## Features ✨

- ✅ **Message Blocking** - Blocks mouse/scroll events  
- ✅ **Timer-Based** - 100ms repaint loop

## PoC:
<img width="669" height="563" alt="image" src="https://github.com/user-attachments/assets/dbbabdc9-53a8-4d48-b719-600194fb7fed" />


## Usage 🚀

1. Compile as DLL
2. Inject into process  (taskmgr, i used processhacker to inject: left click on process, press misc and chose inject dll)
3. Open Task Manager
4. See "TROLLED" instead of processes

## Limitations ⚠️

- Only affects Task Manager
- Obvious to users
- Task Manager specific

---

**Old experimental code - don't use in production!** 🎭
