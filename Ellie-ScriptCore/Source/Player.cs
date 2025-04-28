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
        void OnCreate()
        {
            Console.WriteLine("Create");
        }

        void OnUpdate(float ts)
        {
            Console.WriteLine($"Update : {ts}");
        }
    }
}
