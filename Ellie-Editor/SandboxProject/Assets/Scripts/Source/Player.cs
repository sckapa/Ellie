using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Ellie;

namespace Sandbox
{
    class Player : Entity
    {
        public float Speed;
        Entity camEntity;

        void OnCreate()
        {
            camEntity = FindEntityByName("Camera");
            Console.WriteLine($"Create - {ID}");
        }

        void OnUpdate(float ts)
        {
            Camera camera = camEntity.As<Camera>();

            if (camera != null)
            {
                camera.DistanceFromCamera += 0.001f * ts;
            }

            Vector3 velocity = Vector3.Zero;

            if (Input.IsKeyDown(KeyCode.W))
            {
                velocity.Y = 1.0f;
            }
            else if(Input.IsKeyDown(KeyCode.S))
            {
                velocity.Y = -1.0f;
            }
            
            if(Input.IsKeyDown(KeyCode.A))
            {
                velocity.X = -1.0f;
            }
            else if(Input.IsKeyDown(KeyCode.D))
            {
                velocity.X = 1.0f;
            }

            velocity *= Speed;

            Vector3 translation = Translation; 
            translation += velocity * ts;

            Translation = translation;
        }
    }
}
