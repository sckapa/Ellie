using System;
using System.Runtime.CompilerServices;

namespace Ellie
{
    public struct Vector3
    {
        public float X, Y, Z;

        public static Vector3 Zero => new Vector3(0.0f);

        public Vector3(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }

        public Vector3(float scalar)
        {
            X = scalar;
            Y = scalar;
            Z = scalar;
        }

        public static Vector3 operator*(Vector3 vector, float scalar)
        {
            return new Vector3(vector.X * scalar, vector.Y * scalar, vector.Z * scalar);
        }

        public static Vector3 operator+(Vector3 a, Vector3 b)
        {
            return new Vector3(a.X + b.X, a.Y + b.Y, a.Z + b.Z);
        }
    }

    public static class InternalCalls
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Normalize_Vector3(ref Vector3 param, out Vector3 outParam);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Entity_GetTranslation(ulong entityID, out Vector3 outTranslation);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Entity_SetTranslation(ulong entityID, ref Vector3 translation);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsKeyDown(KeyCode keycode);
    }

    public class Entity
    {
        public Entity()
        {
            ID = 0;
        }

        public readonly ulong ID;

        public Entity(ulong id)
        {
            ID = id;
        }

        public Vector3 Translation
        {
            get
            {
                InternalCalls.Entity_GetTranslation(ID, out Vector3 outTranslation);
                return outTranslation;
            }
            set
            {
                InternalCalls.Entity_SetTranslation(ID, ref value);
            }
        }
        
    }
}