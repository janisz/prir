using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace zadanie_1A
{

    public enum CookType
    {
        Desserts,
        Soups,
        MainCourses
    }

    class Peoples
    {
        private static Random random = new Random();
        private static Counter counter = new Counter();
        private const int MinPreparationTime = 1;
        private const int MaxPreparationTime = 500;

        private static int prepareTime
        {
            get 
            { 
                return random.Next(MinPreparationTime, MaxPreparationTime); 
            }
        }

        private static int Id 
        {
            get 
            {
                return Thread.CurrentThread.ManagedThreadId;
            }
        }

        public void Cook(object type)
        {
            CookType t = (CookType)type;
            switch ((CookType)t)
            {
                case CookType.Desserts:
                    Cook(counter.DessertsFull, counter.DessertsEmpty, t);
                    break;
                case CookType.Soups:
                    Cook(counter.SoupsFull, counter.SoupsEmpty, t);
                    break;
                case CookType.MainCourses:
                    Cook(counter.MainCoursesFull, counter.MainCoursesEmpty, t);
                    break;
            }
        }

        private void Cook(Semaphore semaphoreFull, Semaphore semaphoreEmpty, CookType type)
        {
            while (true)
            {
                semaphoreEmpty.WaitOne();
                Thread.Sleep(prepareTime);
                System.Console.WriteLine("Cook [{0}]: \t {1}", Id, type);
                semaphoreFull.Release();
            }

        }

        public void Miner()
        {

        }

        public void Child()
        {

        }

        public void Woman()
        {

        }

    }

    class Counter
    {
        private const int counterSize = 4;

        public Semaphore women = new Semaphore(0, 1);

        public Semaphore DessertsFull = new Semaphore(0, counterSize);
        public Semaphore SoupsFull = new Semaphore(0, counterSize);
        public Semaphore MainCoursesFull = new Semaphore(0, counterSize);

        public Semaphore DessertsEmpty = new Semaphore(counterSize, counterSize);
        public Semaphore SoupsEmpty = new Semaphore(counterSize, counterSize);
        public Semaphore MainCoursesEmpty = new Semaphore(counterSize, counterSize);
        
    }

    class Program
    {
        static void Main(string[] args)
        {
            int n = 3;
            int m = 4;
            int k = 5;

            var peoples = new Peoples();

            for (int i = 0; i < n; ++i)
            {
                Thread tc = new Thread(new ParameterizedThreadStart(peoples.Cook));
                tc.Start(CookType.Soups);
            }
            for (int i = 0; i < m; ++i)
            {
                Thread tc = new Thread(new ParameterizedThreadStart(peoples.Cook));
                tc.Start(CookType.MainCourses);
            }
            for (int i = 0; i < k; ++i)
            {
                Thread tc = new Thread(new ParameterizedThreadStart(peoples.Cook));
                tc.Start(CookType.Desserts);
            }
        }
    }
}
