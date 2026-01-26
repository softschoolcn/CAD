using System;
using System.Diagnostics;
using System.Runtime.InteropServices;

namespace UiTranslateOverlay;

internal static class NativeMethods
{
    [DllImport("user32.dll")]
    public static extern IntPtr GetForegroundWindow();

    [DllImport("user32.dll", SetLastError = true)]
    private static extern uint GetWindowThreadProcessId(IntPtr hWnd, out uint lpdwProcessId);

    [DllImport("dwmapi.dll")]
    private static extern int DwmGetWindowAttribute(IntPtr hwnd, int dwAttribute, out RECT pvAttribute, int cbAttribute);

    private const int DWMWA_EXTENDED_FRAME_BOUNDS = 9;

    [StructLayout(LayoutKind.Sequential)]
    internal struct RECT
    {
        public int Left, Top, Right, Bottom;
    }

    public static bool TryGetProcessNameFromHwnd(IntPtr hwnd, out string? exeName)
    {
        exeName = null;
        try
        {
            GetWindowThreadProcessId(hwnd, out var pid);
            if (pid == 0) return false;
            using var p = Process.GetProcessById((int)pid);
            exeName = p.MainModule?.ModuleName;
            return !string.IsNullOrWhiteSpace(exeName);
        }
        catch
        {
            return false;
        }
    }

    public static bool TryGetWindowBounds(IntPtr hwnd, out RECT rect)
    {
        rect = default;
        try
        {
            int hr = DwmGetWindowAttribute(hwnd, DWMWA_EXTENDED_FRAME_BOUNDS, out rect, Marshal.SizeOf<RECT>());
            return hr == 0;
        }
        catch
        {
            return false;
        }
    }

    // Available on Windows 10+ (1607+). If unavailable, call will throw and we fall back.
    [DllImport("user32.dll")]
    private static extern uint GetDpiForWindow(IntPtr hWnd);

    public static int GetDpi(IntPtr hwnd)
    {
        try
        {
            var dpi = (int)GetDpiForWindow(hwnd);
            return dpi <= 0 ? 96 : dpi;
        }
        catch
        {
            return 96;
        }
    }

    [DllImport("user32.dll", SetLastError = true)]
    private static extern bool SetWindowPos(IntPtr hWnd, IntPtr hWndInsertAfter,
        int X, int Y, int cx, int cy, uint uFlags);

    private static readonly IntPtr HWND_TOPMOST = new IntPtr(-1);
    private static readonly IntPtr HWND_NOTOPMOST = new IntPtr(-2);
    private const uint SWP_NOMOVE = 0x0002;
    private const uint SWP_NOSIZE = 0x0001;
    private const uint SWP_NOACTIVATE = 0x0010;
    private const uint SWP_SHOWWINDOW = 0x0040;

    public static void EnsureTopmost(IntPtr hwnd)
    {
        _ = SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    }

    public static void ClearTopmost(IntPtr hwnd)
    {
        _ = SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    }

    /// <summary>
    /// Position overlay to exactly match target bounds and keep it just above the target in Z-order,
    /// without stealing focus.
    /// </summary>
    public static void FollowTarget(IntPtr overlayHwnd, IntPtr targetHwnd, int x, int y, int w, int h)
    {
        // Per Win32 docs: hWndInsertAfter is the window that the positioned window should be placed after in Z-order.
        // Using targetHwnd keeps overlay adjacent to target's Z position ("shadow window" behaviour).
        _ = SetWindowPos(overlayHwnd, targetHwnd, x, y, w, h, SWP_NOACTIVATE | SWP_SHOWWINDOW);
    }

    public const int GWL_EXSTYLE = -20;
    public const int WS_EX_TRANSPARENT = 0x20;
    public const int WS_EX_TOOLWINDOW = 0x80;
    public const int WS_EX_NOACTIVATE = 0x08000000;
    public const int WS_EX_LAYERED = 0x80000;

    [DllImport("user32.dll")]
    public static extern int GetWindowLong(IntPtr hWnd, int nIndex);

    [DllImport("user32.dll")]
    public static extern int SetWindowLong(IntPtr hWnd, int nIndex, int dwNewLong);
}
