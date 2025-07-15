using System;
using System.Runtime.CompilerServices;
using System.Runtime.Remoting;

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

        // Changes made in play mode are not accepted
        // It will always return the entity that matches the name in edit mode
        public Entity FindEntityByName(string name)
        {
            ulong uuid = InternalCalls.Entity_FindEntityByName(name);

            if (uuid == 0) 
            {
                return null;
            }

            return new Entity(uuid);
        }

        public T As<T>() where T: Entity, new()
        {
            object instance = InternalCalls.GetScriptInstance(ID);
            return instance as T;
        }
        
    }
}