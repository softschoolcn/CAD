using System.Windows;

namespace UiTranslateOverlay;

// Fully qualify to avoid ambiguity when WinForms is enabled (System.Windows.Forms.Application vs System.Windows.Application)
public partial class App : System.Windows.Application
{
}
