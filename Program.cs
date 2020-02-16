using System;
using System.IO.Ports;
using System.Runtime.InteropServices;
using System.Threading;
using Bool = System.Int32;

namespace UserInputToArduino
{

    public static class Winuser
    {
        public const string DynamicLinkLibrary = "User32.dll";

        [StructLayout(LayoutKind.Sequential)]
        public struct Point
        {
            public int X;
            public int Y;
        }

        [DllImport(DynamicLinkLibrary, EntryPoint = "GetCursorPos")]
        public static extern Bool GetCursorPosition(ref Point point);
    }

    public static class Program
    {
        public static void Main(string[] args)
        {
            SerialPort serial = new SerialPort("COM6", 9600);

            serial.Open();

            Winuser.Point point = new Winuser.Point();
            byte[] buffer = new byte[2];

            while (true)
            {
                Winuser.GetCursorPosition(ref point);

                buffer[0] = (byte)(point.X / (1920 / 8));
                buffer[1] = (byte)(point.Y / (1080 / 8));

                serial.Write(buffer, 0, 2);

                Thread.Sleep(5);
            }
        }
    }
}
