using System;
using System.Runtime.CompilerServices;

namespace Ellie
{
    public struct Vector3
    {
        public float X, Y, Z;

        public Vector3(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }
    }

    public static class InternalCalls
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Normalize_Vector3(ref Vector3 param, out Vector3 outParam);
    }

    public class Entity
    {
        public Entity()
        {
            Console.WriteLine("Hello from constructor");

            Vector3 test = new Vector3(5, 2.5f, 1);
            Log(test);
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

        private void Log(Vector3 param)
        {
            InternalCalls.Normalize_Vector3(ref param, out Vector3 outParam);
            Console.WriteLine($"{outParam.X}, {outParam.Y}, {outParam.Z}");
        }
    }
}