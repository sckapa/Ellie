using System;
using System.Runtime.CompilerServices;

namespace Ellie
{
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
                InternalCalls.TransformComponent_GetTranslation(ID, out Vector3 result);
                return result;
            }
            set
            {
                InternalCalls.TransformComponent_SetTranslation(ID, ref value);
            }
        }

        public bool HasComponent<T>() where T:Component, new()
        {
            Type type = typeof(T);
            return InternalCalls.Entity_HasComponent(ID, type);
        }

        public T GetComponent<T>() where T : Component, new()
        {
            if(!HasComponent<T>())
            {
                return null;
            }

            T component = new T() { Entity = this };
            return component;
        }
        
    }
}