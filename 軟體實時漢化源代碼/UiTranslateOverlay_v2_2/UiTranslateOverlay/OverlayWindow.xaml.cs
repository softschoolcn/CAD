using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Interop;
using System.Windows.Media;

namespace UiTranslateOverlay;

public partial class OverlayWindow : Window
{
    private IntPtr _targetHwnd = IntPtr.Zero;

    public OverlayWindow()
    {
        InitializeComponent();
        Loaded += (_, __) => MakeClickThrough();
    }

    private void MakeClickThrough()
    {
        var hwnd = new WindowInteropHelper(this).Handle;
        int ex = NativeMethods.GetWindowLong(hwnd, NativeMethods.GWL_EXSTYLE);
        ex |= NativeMethods.WS_EX_TRANSPARENT | NativeMethods.WS_EX_TOOLWINDOW | NativeMethods.WS_EX_NOACTIVATE | NativeMethods.WS_EX_LAYERED;
        NativeMethods.SetWindowLong(hwnd, NativeMethods.GWL_EXSTYLE, ex);
        // Don't force TOPMOST forever; we keep Z-order adjacent to the target window instead.
    }

    public void SetTargetHwnd(IntPtr hwnd)
    {
        _targetHwnd = hwnd;
    }

    public void SetBounds(TargetWindowBounds b)
    {
        Left = b.Left;
        Top = b.Top;
        Width = b.Width;
        Height = b.Height;

        var hwnd = new WindowInteropHelper(this).Handle;
        if (_targetHwnd != IntPtr.Zero)
        {
            // Use Win32 to keep overlay glued and Z-adjacent (above the target) without focus stealing.
            NativeMethods.FollowTarget(hwnd, _targetHwnd, (int)b.Left, (int)b.Top, (int)b.Width, (int)b.Height);
        }
    }

    public void Render(IReadOnlyList<OverlayItem> items, Func<string, string> translate, bool show)
    {
        if (!show)
        {
            RootCanvas.Children.Clear();
            return;
        }

        RootCanvas.Children.Clear();

        foreach (var it in items)
        {
            var text = translate(it.SourceText);
            if (string.IsNullOrWhiteSpace(text)) continue;

            var tb = CreateTextBlock(text, it.Bounds);
            RootCanvas.Children.Add(tb);

            Canvas.SetLeft(tb, it.Bounds.X - Left);
            Canvas.SetTop(tb, it.Bounds.Y - Top);
        }
    }

    private TextBlock CreateTextBlock(string text, Rect bounds)
    {
        var tb = new TextBlock
        {
            Text = text,
            FontSize = Math.Max(10, bounds.Height * 0.55),
            Foreground = System.Windows.Media.Brushes.White,
            FontWeight = FontWeights.SemiBold
        };

        tb.Effect = new System.Windows.Media.Effects.DropShadowEffect
        {
            BlurRadius = 4,
            Opacity = 0.9,
            ShadowDepth = 0
        };

        tb.Measure(new System.Windows.Size(double.PositiveInfinity, double.PositiveInfinity));
        var w = tb.DesiredSize.Width;
        if (w > bounds.Width && w > 0)
        {
            var scale = bounds.Width / w;
            tb.LayoutTransform = new ScaleTransform(scale, scale);
        }

        return tb;
    }
}