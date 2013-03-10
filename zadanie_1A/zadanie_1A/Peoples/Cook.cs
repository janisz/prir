using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace zadanie_1A
{
    class Cook : People, IPeople
    {
        Semaphore semaphoreEmpty;
        Semaphore semaphoreFull;
        CookType type;

        public Cook(Semaphore semaphoreFull, Semaphore semaphoreEmpty, CookType type)
        {
            this.semaphoreEmpty = semaphoreEmpty;
            this.semaphoreFull = semaphoreFull;
            this.type = type;
        }

        public void Run()
        {
            System.Console.WriteLine("Cook [{0}]: \t Created", Id, type);
            while (true)
            {
                semaphoreEmpty.WaitOne();
                Thread.Sleep(prepareTime);

                System.Console.WriteLine("Cook [{0}]: \t {1}", Id, type);

                semaphoreFull.Release();
            }
        }

        private int prepareTime
        {
            get
            {
                return random.Next(MinPreparationTime, MaxPreparationTime);
            }
        }
    }
}

