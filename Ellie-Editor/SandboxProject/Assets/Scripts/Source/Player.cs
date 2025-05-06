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

        void OnCreate()
        {
            Console.WriteLine($"Create - {ID}");
            Speed = 0.025f;
        }

        void OnUpdate(float ts)
        {
            //Console.WriteLine($"Update : {ts} - {Translation.X}");

            float speed = Speed;
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

            velocity *= speed;

            Vector3 translation = Translation;
            translation += velocity * ts;

            Translation = translation;
        }
    }
}
