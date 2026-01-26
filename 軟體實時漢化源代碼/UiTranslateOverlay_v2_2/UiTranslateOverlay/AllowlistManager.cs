using System;
using System.Collections.Generic;
using System.IO;
using System.Threading;

namespace UiTranslateOverlay;

public sealed class AllowlistManager : IDisposable
{
    private readonly string _path;
    private readonly FileSystemWatcher _fsw;
    // Fully qualify to avoid ambiguity when WinForms is enabled (System.Windows.Forms.Timer vs System.Threading.Timer)
    private readonly System.Threading.Timer _debounce;
    private volatile int _pending;
    private HashSet<string> _set = new(StringComparer.OrdinalIgnoreCase);

    public event Action<HashSet<string>>? Changed;

    public AllowlistManager(string path)
    {
        _path = path;
        Reload();

        var dir = Path.GetDirectoryName(_path)!;
        var name = Path.GetFileName(_path);

        _debounce = new System.Threading.Timer(_ =>
        {
            if (Interlocked.Exchange(ref _pending, 0) == 1)
            {
                try
                {
                    Reload();
                    Changed?.Invoke(Snapshot());
                }
                catch { }
            }
        }, null, Timeout.Infinite, Timeout.Infinite);

        _fsw = new FileSystemWatcher(dir, name)
        {
            NotifyFilter = NotifyFilters.LastWrite | NotifyFilters.Size | NotifyFilters.FileName
        };
        _fsw.Changed += (_, __) => Schedule();
        _fsw.Created += (_, __) => Schedule();
        _fsw.Renamed += (_, __) => Schedule();
        _fsw.EnableRaisingEvents = true;
    }

    public HashSet<string> Snapshot() => new(_set, StringComparer.OrdinalIgnoreCase);

    public bool IsAllowed(string exeName) => _set.Contains(exeName);

    private void Schedule()
    {
        Interlocked.Exchange(ref _pending, 1);
        _debounce.Change(200, Timeout.Infinite);
    }

    private void Reload()
    {
        var next = new HashSet<string>(StringComparer.OrdinalIgnoreCase);
        if (File.Exists(_path))
        {
            foreach (var raw in File.ReadAllLines(_path))
            {
                var line = raw.Trim();
                if (line.Length == 0) continue;
                if (line.StartsWith("#")) continue;

                var exe = Path.GetFileName(line);
                if (!string.IsNullOrWhiteSpace(exe))
                    next.Add(exe);
            }
        }
        _set = next;
    }

    public void Dispose()
    {
        _fsw.Dispose();
        _debounce.Dispose();
    }
}
