using System;
using System.IO.Ports;

namespace EnumerateSerial
{
    class Program
    {
        static void Main(string[] args)
        {
            foreach (string s in SerialPort.GetPortNames())
            {
                Console.WriteLine("{0}", s);
            }
       }
    }
}
