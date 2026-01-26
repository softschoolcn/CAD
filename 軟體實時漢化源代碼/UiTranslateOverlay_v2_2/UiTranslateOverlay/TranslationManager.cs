using System;
using System.Collections.Generic;
using System.IO;

namespace UiTranslateOverlay;

public sealed class TranslationManager : IDisposable
{
    private readonly string _xlsxPath;
    private readonly FileSystemWatcher _fsw;
    private readonly System.Threading.Timer _debounce;
    private volatile int _pending;

    private Dictionary<string, (string zhHans, string zhHant)> _map = new(StringComparer.Ordinal);

    public UiLanguage CurrentLanguage { get; private set; } = UiLanguage.Source;

    public event Action? Changed;

    public TranslationManager(string xlsxPath)
    {
        _xlsxPath = xlsxPath;
        Reload();

        var dir = Path.GetDirectoryName(_xlsxPath)!;
        var name = Path.GetFileName(_xlsxPath);

        _debounce = new System.Threading.Timer(_ =>
        {
            if (System.Threading.Interlocked.Exchange(ref _pending, 0) == 1)
            {
                try { Reload(); Changed?.Invoke(); } catch { }
            }
        }, null, System.Threading.Timeout.Infinite, System.Threading.Timeout.Infinite);

        _fsw = new FileSystemWatcher(dir, name)
        {
            NotifyFilter = NotifyFilters.LastWrite | NotifyFilters.Size | NotifyFilters.FileName
        };
        _fsw.Changed += (_, __) => Schedule();
        _fsw.Created += (_, __) => Schedule();
        _fsw.Renamed += (_, __) => Schedule();
        _fsw.EnableRaisingEvents = true;
    }

    public void SetLanguage(UiLanguage lang)
    {
        if (CurrentLanguage == lang) return;
        CurrentLanguage = lang;
        Changed?.Invoke();
    }

    public string Translate(string source)
    {
        if (CurrentLanguage == UiLanguage.Source) return source;

        if (_map.TryGetValue(source, out var v))
        {
            var t = CurrentLanguage == UiLanguage.ZhHans ? v.zhHans : v.zhHant;
            return string.IsNullOrWhiteSpace(t) ? source : t;
        }

        return source;
    }

    private void Schedule()
    {
        System.Threading.Interlocked.Exchange(ref _pending, 1);
        _debounce.Change(200, System.Threading.Timeout.Infinite);
    }

    private void Reload()
    {
        _map = XlsxGlossaryReader.Load(_xlsxPath);
    }

    public void Dispose()
    {
        _fsw.Dispose();
        _debounce.Dispose();
    }
}
