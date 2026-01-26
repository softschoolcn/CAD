using System.Windows;

namespace UiTranslateOverlay;

public enum UiLanguage
{
    Source = 0,
    ZhHans = 1,
    ZhHant = 2
}

public readonly record struct TargetWindowBounds(double Left, double Top, double Width, double Height);

public sealed record OverlayItem(
    string SourceText,
    string RenderText,
    Rect Bounds
);
