using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace zadanie_1A
{
    class Counter
    {
        private static Counter instance;
        private Counter() { }

        public static Counter Instance
        {
            get
            {
                if (instance == null)
                {
                    instance = new Counter();
                }
                return instance;
            }
        }

        private const int counterSize = 4;

        private int womenCount = 0;

        public Semaphore Women = new Semaphore(1, 1);

        public Semaphore DessertsFull = new Semaphore(0, counterSize);
        public Semaphore SoupsFull = new Semaphore(0, counterSize);
        public Semaphore MainCoursesFull = new Semaphore(0, counterSize);

        public Semaphore DessertsEmpty = new Semaphore(counterSize, counterSize);
        public Semaphore SoupsEmpty = new Semaphore(counterSize, counterSize);
        public Semaphore MainCoursesEmpty = new Semaphore(counterSize, counterSize);

        public int getWomenCount()
        {
            Women.WaitOne();
            int ret = womenCount;
            Women.Release();
            return ret;
        }

        public int incWomenCount()
        {
            Women.WaitOne();
            int ret = ++womenCount;
            Women.Release();
            return ret;
        }

        public void decWomenCount()
        {
            Women.WaitOne();
            womenCount--;
            Women.Release();
        }

    }
}
