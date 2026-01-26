using System;
using System.IO;
using System.Windows;
using System.Windows.Forms;

namespace UiTranslateOverlay;

public partial class MainWindow : Window
{
    private NotifyIcon? _tray;
    private Icon _iconOn = System.Drawing.SystemIcons.Shield;
    private Icon _iconOff = System.Drawing.SystemIcons.Application;

    private OverlayWindow? _overlay;
    private UiaWatcher? _watcher;
    private HotkeyManager? _hotkeys;
    private AllowlistManager? _allowlistMgr;
    private TranslationManager? _tm;

    private bool _enabled = true;

    public MainWindow()
    {
        InitializeComponent();

        Loaded += (_, __) => InitializeRuntime();
        Closing += (_, e) =>
        {
            e.Cancel = true;
            Hide();
        };
    }

    private void InitializeRuntime()
    {
        BuildTray();

        _overlay = new OverlayWindow();
        _overlay.Hide();

        var baseDir = AppDomain.CurrentDomain.BaseDirectory;

        var allowPath = Path.Combine(baseDir, "allowlist.txt");
        _allowlistMgr = new AllowlistManager(allowPath);

        var xlsxPath = Path.Combine(baseDir, "glossary", "global.xlsx");
        _tm = new TranslationManager(xlsxPath);

        _watcher = new UiaWatcher();
        _watcher.SetAllowlist(_allowlistMgr.Snapshot());

        _allowlistMgr.Changed += set => Dispatcher.Invoke(() => _watcher?.SetAllowlist(set));

        _watcher.ActiveChanged += (active, exe) =>
        {
            Dispatcher.Invoke(() =>
            {
                if (_tray != null)
                {
                    _tray.Icon = active ? _iconOn : _iconOff;
                    _tray.Text = active ? $"UI 同位翻譯覆蓋（已啟用：{exe}）" : "UI 同位翻譯覆蓋（待命）";
                }

                if (_overlay != null)
                {
                    if (active) _overlay.Show();
                    else _overlay.Hide();
                }
            });
        };

        _watcher.TargetBounds += b => Dispatcher.Invoke(() =>
        {
            if (_overlay == null || _watcher == null) return;
            _overlay.SetTargetHwnd(_watcher.CurrentHwnd);
            _overlay.SetBounds(b);
        });

        _watcher.OverlayUpdated += items =>
        {
            Dispatcher.Invoke(() =>
            {
                if (_overlay == null || _tm == null) return;
                _overlay.Render(items, _tm.Translate, show: _enabled);
            });
        };

        _hotkeys = new HotkeyManager(this);
        _hotkeys.HotkeyPressed += id =>
        {
            if (_watcher == null || _tm == null) return;

            if (id == 1000)
            {
                _enabled = !_enabled;
                _watcher.SetEnabled(_enabled);
                if (!_enabled) _overlay?.Render(Array.Empty<OverlayItem>(), s => s, show: false);
            }
            else if (id == 1001) _tm.SetLanguage(UiLanguage.Source);
            else if (id == 1002) _tm.SetLanguage(UiLanguage.ZhHans);
            else if (id == 1003) _tm.SetLanguage(UiLanguage.ZhHant);
        };

        Hide();
    }

    private void BuildTray()
    {
        _tray = new NotifyIcon
        {
            Icon = _iconOff,
            Visible = true,
            Text = "UI 同位翻譯覆蓋（待命）"
        };

        var menu = new ContextMenuStrip();

        var showItem = new ToolStripMenuItem("顯示說明視窗");
        showItem.Click += (_, __) => Dispatcher.Invoke(() =>
        {
            Show();
            Activate();
        });

        var exitItem = new ToolStripMenuItem("退出");
        exitItem.Click += (_, __) => Dispatcher.Invoke(() =>
        {
            Cleanup();
            System.Windows.Application.Current.Shutdown();
        });

        menu.Items.Add(showItem);
        menu.Items.Add(new ToolStripSeparator());
        menu.Items.Add(exitItem);

        _tray.ContextMenuStrip = menu;
        _tray.DoubleClick += (_, __) => Dispatcher.Invoke(() =>
        {
            Show();
            Activate();
        });
    }

    private void Cleanup()
    {
        _tray?.Dispose();
        _hotkeys?.Dispose();
        _watcher?.Dispose();
        _allowlistMgr?.Dispose();
        _tm?.Dispose();

        _overlay?.Close();
        _overlay = null;
    }
}
