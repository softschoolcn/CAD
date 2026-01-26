using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.IO.Compression;
using System.Linq;
using System.Xml;

namespace UiTranslateOverlay;

/// <summary>
/// Minimal .xlsx reader (no NuGet). Assumptions:
/// - Read the first worksheet (sheet1) only.
/// - Row format: A=Source, B=ZhHans, C=ZhHant
/// - Values are strings (sharedStrings or inlineStr).
/// </summary>
public static class XlsxGlossaryReader
{
    public static Dictionary<string, (string zhHans, string zhHant)> Load(string xlsxPath)
    {
        var dict = new Dictionary<string, (string, string)>(StringComparer.Ordinal);

        if (!File.Exists(xlsxPath))
            return dict;

        using var fs = File.OpenRead(xlsxPath);
        using var zip = new ZipArchive(fs, ZipArchiveMode.Read);

        var sharedStrings = ReadSharedStrings(zip);

        var sheetEntry = zip.GetEntry("xl/worksheets/sheet1.xml");
        if (sheetEntry is null) return dict;

        using var sheetStream = sheetEntry.Open();
        using var xr = XmlReader.Create(sheetStream, new XmlReaderSettings { IgnoreComments = true, IgnoreWhitespace = true });

        string? curCol = null;
        string? v = null;
        string? a = null, b = null, c = null;
        string? cellType = null;

        while (xr.Read())
        {
            if (xr.NodeType == XmlNodeType.Element && xr.Name == "row")
            {
                a = b = c = null;
            }

            if (xr.NodeType == XmlNodeType.Element && xr.Name == "c")
            {
                curCol = null;
                v = null;
                cellType = xr.GetAttribute("t"); // "s" for shared string
                var r = xr.GetAttribute("r"); // e.g., "A1"
                if (!string.IsNullOrEmpty(r))
                {
                    curCol = new string(r.TakeWhile(char.IsLetter).ToArray());
                }
            }

            if (xr.NodeType == XmlNodeType.Element && (xr.Name == "v" || xr.Name == "t"))
            {
                v = xr.ReadElementContentAsString();
                if (curCol == null) continue;

                var val = ResolveCellValue(v, cellType, sharedStrings);

                if (curCol.Equals("A", StringComparison.OrdinalIgnoreCase)) a = val;
                else if (curCol.Equals("B", StringComparison.OrdinalIgnoreCase)) b = val;
                else if (curCol.Equals("C", StringComparison.OrdinalIgnoreCase)) c = val;
            }

            if (xr.NodeType == XmlNodeType.EndElement && xr.Name == "row")
            {
                if (!string.IsNullOrWhiteSpace(a))
                {
                    dict[a] = (b ?? string.Empty, c ?? string.Empty);
                }
            }
        }

        return dict;
    }

    private static string ResolveCellValue(string raw, string? cellType, List<string> sharedStrings)
    {
        if (string.Equals(cellType, "s", StringComparison.OrdinalIgnoreCase))
        {
            if (int.TryParse(raw, NumberStyles.Integer, CultureInfo.InvariantCulture, out int idx))
            {
                if (idx >= 0 && idx < sharedStrings.Count) return sharedStrings[idx];
            }
        }
        return raw ?? string.Empty;
    }

    private static List<string> ReadSharedStrings(ZipArchive zip)
    {
        var list = new List<string>();
        var entry = zip.GetEntry("xl/sharedStrings.xml");
        if (entry is null) return list;

        using var s = entry.Open();
        using var xr = XmlReader.Create(s, new XmlReaderSettings { IgnoreComments = true, IgnoreWhitespace = true });
        string? current = null;
        while (xr.Read())
        {
            if (xr.NodeType == XmlNodeType.Element && xr.Name == "si")
            {
                current = "";
            }
            else if (xr.NodeType == XmlNodeType.Element && xr.Name == "t")
            {
                var t = xr.ReadElementContentAsString();
                current += t;
            }
            else if (xr.NodeType == XmlNodeType.EndElement && xr.Name == "si")
            {
                list.Add(current ?? "");
                current = null;
            }
        }
        return list;
    }
}
