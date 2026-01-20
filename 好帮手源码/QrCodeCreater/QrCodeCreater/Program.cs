using System;
using System.Collections.Generic;
using System.Text;
using ThoughtWorks.QRCode.Codec;
using System.Diagnostics;
using System.IO;
using System.Drawing;

namespace QrCodeCreater
{
    class Program
    {
        static void Main(string[] args)
        {
            if ((args != null) && (args.Length != 0))
            {
                string path = args[0];
                try
                {
                    Trace.WriteLine("生成{" + path + "}二维码");
                    QRCodeEncoder encoder = new QRCodeEncoder();
                    encoder.QRCodeEncodeMode = QRCodeEncoder.ENCODE_MODE.BYTE;
                    encoder.QRCodeErrorCorrect = QRCodeEncoder.ERROR_CORRECTION.H;
                    encoder.QRCodeScale = 2;
                    encoder.QRCodeVersion = 0x0;
                    string content = File.ReadAllText(path, Encoding.Default);
                    File.Delete(path);
                    Image original = encoder.Encode(content);
                    Bitmap bitmap = new Bitmap(original);
                    string contents = "";
                    int width = original.Width;
                    contents = contents + width.ToString() + ";";
                    int height = original.Height;
                    contents = contents + height.ToString() + ";";
                    for (int i = 0; i < width; i++)
                    {
                        for (int j = 0; j < height; j++)
                        {
                            Color pixel = bitmap.GetPixel(i, j);
                            if (((pixel.R == 0) && (pixel.G == 0)) && (pixel.B == 0))
                            {
                                contents = contents + "1";
                            }
                            else
                            {
                                contents = contents + "0";
                            }
                        }
                    }
                    File.WriteAllText(path, contents);
                }
                catch (Exception exception)
                {
                    Trace.Write("生成二维码时发生异常:" + exception.StackTrace);
                    File.Delete(path);
                }
            }

        }
    }
}
