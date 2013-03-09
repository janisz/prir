using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace zadanie_1A
{
    class Peoples
    {
        protected Random random = new Random();
        protected Counter counter = Counter.Instance;
        protected const int MinPreparationTime = 1;
        protected const int MaxPreparationTime = 500;

        protected static int Id
        {
            get
            {
                return Thread.CurrentThread.ManagedThreadId;
            }
        }
    }
}
