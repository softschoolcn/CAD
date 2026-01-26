using System;
using System.Runtime.InteropServices;
using System.Windows;
using System.Windows.Interop;

namespace UiTranslateOverlay;

public sealed class HotkeyManager : IDisposable
{
    private readonly HwndSource _source;

    public event Action<int>? HotkeyPressed;

    public HotkeyManager(Window window)
    {
        var helper = new WindowInteropHelper(window);
        _source = HwndSource.FromHwnd(helper.Handle);
        _source.AddHook(WndProc);

        Register(1000, MOD_CONTROL | MOD_ALT, (uint)Keys.D0);
        Register(1001, MOD_CONTROL | MOD_ALT, (uint)Keys.D1);
        Register(1002, MOD_CONTROL | MOD_ALT, (uint)Keys.D2);
        Register(1003, MOD_CONTROL | MOD_ALT, (uint)Keys.D3);
    }

    private void Register(int id, uint modifiers, uint key)
    {
        try { RegisterHotKey(_source.Handle, id, modifiers, key); } catch { }
    }

    private IntPtr WndProc(IntPtr hwnd, int msg, IntPtr wParam, IntPtr lParam, ref bool handled)
    {
        if (msg == WM_HOTKEY)
        {
            int id = wParam.ToInt32();
            HotkeyPressed?.Invoke(id);
            handled = true;
        }
        return IntPtr.Zero;
    }

    public void Dispose()
    {
        UnregisterHotKey(_source.Handle, 1000);
        UnregisterHotKey(_source.Handle, 1001);
        UnregisterHotKey(_source.Handle, 1002);
        UnregisterHotKey(_source.Handle, 1003);
        _source.RemoveHook(WndProc);
    }

    private const int WM_HOTKEY = 0x0312;
    private const uint MOD_ALT = 0x0001;
    private const uint MOD_CONTROL = 0x0002;

    private enum Keys : uint
    {
        D0 = 0x30,
        D1 = 0x31,
        D2 = 0x32,
        D3 = 0x33,
    }

    [DllImport("user32.dll", SetLastError = true)]
    private static extern bool RegisterHotKey(IntPtr hWnd, int id, uint fsModifiers, uint vk);

    [DllImport("user32.dll", SetLastError = true)]
    private static extern bool UnregisterHotKey(IntPtr hWnd, int id);
}
