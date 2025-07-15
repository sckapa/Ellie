using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Ellie;

namespace Sandbox
{
    class Camera : Entity
    {
        public float DistanceFromCamera = 5f;
        Entity box;
        void OnCreate()
        {
            box = FindEntityByName("Boxx");
        }

        void OnUpdate(float ts)
        {
            Translation = new Vector3(box.Translation.X, box.Translation.Y, DistanceFromCamera);
        }
    }
}
