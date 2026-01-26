using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows;
using System.Windows.Automation;
using System.Windows.Threading;

namespace UiTranslateOverlay;

public sealed class UiaWatcher : IDisposable
{
    private readonly DispatcherTimer _scanTimer;
    private readonly DispatcherTimer _boundsTimer;

    private IntPtr _hwnd = IntPtr.Zero;
    public IntPtr CurrentHwnd => _hwnd;
    private bool _enabled = true;
    private bool _active;

    private HashSet<string> _allow = new(StringComparer.OrdinalIgnoreCase);

    private readonly ControlType[] _types = new[]
    {
        ControlType.MenuItem,
        ControlType.Button,
        ControlType.TabItem,
        ControlType.CheckBox,
        ControlType.RadioButton,
        ControlType.Text
    };

    public event Action<bool, string?>? ActiveChanged;
    public event Action<TargetWindowBounds>? TargetBounds;
    public event Action<IReadOnlyList<OverlayItem>>? OverlayUpdated;

    public UiaWatcher()
    {
        _scanTimer = new DispatcherTimer(DispatcherPriority.Background)
        {
            Interval = TimeSpan.FromMilliseconds(160)
        };
        _scanTimer.Tick += (_, __) => ScanOnce();

        _boundsTimer = new DispatcherTimer(DispatcherPriority.Background)
        {
            Interval = TimeSpan.FromMilliseconds(33)
        };
        _boundsTimer.Tick += (_, __) => PublishBounds();

        Automation.AddAutomationFocusChangedEventHandler(OnFocusChanged);
        OnFocusChanged(null, EventArgs.Empty);
    }

    public void SetAllowlist(HashSet<string> allow)
    {
        _allow = new HashSet<string>(allow, StringComparer.OrdinalIgnoreCase);
        OnFocusChanged(null, EventArgs.Empty);
    }

    public void SetEnabled(bool enabled)
    {
        _enabled = enabled;
        OnFocusChanged(null, EventArgs.Empty);
    }

    private void OnFocusChanged(object? sender, EventArgs e)
    {
        var hwnd = NativeMethods.GetForegroundWindow();
        if (hwnd == IntPtr.Zero) { Deactivate(null); return; }

        if (!NativeMethods.TryGetProcessNameFromHwnd(hwnd, out var exe) || string.IsNullOrWhiteSpace(exe))
        {
            Deactivate(null); return;
        }

        _hwnd = hwnd;

        bool allowed = _enabled && _allow.Contains(exe);
        if (!allowed) { Deactivate(exe); return; }

        Activate(exe);
    }

    private void Activate(string? exe)
    {
        if (_active) return;
        _active = true;
        _scanTimer.Start();
        _boundsTimer.Start();
        ActiveChanged?.Invoke(true, exe);
    }

    private void Deactivate(string? exe)
    {
        if (!_active)
        {
            ActiveChanged?.Invoke(false, exe);
            return;
        }
        _active = false;
        _scanTimer.Stop();
        _boundsTimer.Stop();
        OverlayUpdated?.Invoke(Array.Empty<OverlayItem>());
        ActiveChanged?.Invoke(false, exe);
    }

    private void PublishBounds()
    {
        if (!_active) return;
        if (_hwnd == IntPtr.Zero) return;

        if (NativeMethods.TryGetWindowBounds(_hwnd, out var r))
        {
            TargetBounds?.Invoke(new TargetWindowBounds(
                Left: r.Left,
                Top: r.Top,
                Width: r.Right - r.Left,
                Height: r.Bottom - r.Top
            ));
        }
    }

    private void ScanOnce()
    {
        if (!_active) return;

        try
        {
            var root = AutomationElement.FromHandle(_hwnd);
            if (root is null) return;

            var cond = new OrCondition(_types.Select(t => new PropertyCondition(AutomationElement.ControlTypeProperty, t)).ToArray());
            var elems = root.FindAll(TreeScope.Descendants, cond);

            // Avoid LINQ GroupBy on every tick; build a de-duplicated map in O(n).
            var map = new Dictionary<(string Text, (int x, int y, int w, int h) Q), OverlayItem>(
                capacity: Math.Min(elems.Count, 400));

            int step = GetQuantizeStepPx();

            for (int i = 0; i < elems.Count; i++)
            {
                var el = elems[i];
                var name = el.Current.Name;
                if (string.IsNullOrWhiteSpace(name)) continue;
                if (name.Length > 120) continue;

                var rect = el.Current.BoundingRectangle;
                if (rect.Width <= 2 || rect.Height <= 2) continue;
                if (double.IsInfinity(rect.X) || double.IsInfinity(rect.Y)) continue;

                var bounds = new Rect(rect.X, rect.Y, rect.Width, rect.Height);
                var key = (name, Quantize(bounds, step));
                if (!map.ContainsKey(key))
                {
                    map[key] = new OverlayItem(name, name, bounds);
                }
            }

            OverlayUpdated?.Invoke(map.Values.ToList());
        }
        catch { }
    }

    private int GetQuantizeStepPx()
    {
        // Make quantization scale with DPI to reduce "jitter" on 125%/150% etc.
        int dpi = _hwnd == IntPtr.Zero ? 96 : NativeMethods.GetDpi(_hwnd);
        double scale = dpi / 96.0;
        // Base step 2px @ 100% -> 3px @ 150% -> 4px @ 200% (rounded & clamped)
        int step = (int)Math.Round(2 * scale);
        return Math.Clamp(step, 2, 6);
    }

    private static (int x, int y, int w, int h) Quantize(Rect r, int step)
    {
        int q(double v) => (int)(Math.Round(v / step) * step);
        return (q(r.X), q(r.Y), q(r.Width), q(r.Height));
    }

    public void Dispose()
    {
        try { Automation.RemoveAutomationFocusChangedEventHandler(OnFocusChanged); } catch { }
        _scanTimer.Stop();
        _boundsTimer.Stop();
    }
}
