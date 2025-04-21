using System;

namespace Ellie
{
    public class Main
    {
        public Main()
        {
            Console.WriteLine("Hello from constructor");
        }

        public void PrintCustomMessage()
        {
            Console.WriteLine("Hello from function");
        }

        public void PrintInt(int param)
        {
            Console.WriteLine(param);
        }

        public void PrintInts(int param1, int param2)
        {
            Console.WriteLine($"{param1}, {param2}");
        }

        public void PrintCustomMessageWithParams(string param)
        {
            Console.WriteLine(param);
        }
    }
}