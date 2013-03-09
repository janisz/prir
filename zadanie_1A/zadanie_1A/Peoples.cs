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
        private Random random = new Random();
        private Counter counter = new Counter();
        private const int MinPreparationTime = 1;
        private const int MaxPreparationTime = 500;

        private int prepareTime
        {
            get
            {
                return random.Next(MinPreparationTime, MaxPreparationTime);
            }
        }

        public T RandomEnum<T>()
        {
            T[] values = (T[])Enum.GetValues(typeof(T));
            return values[random.Next(0, values.Length)];
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

        public void NextPerson()
        {
            PersonType person = RandomEnum<PersonType>();
            switch (person)
            {
                case PersonType.Miner: Miner(); break;
                case PersonType.Child: Child(); break;
                case PersonType.Women: Woman(); break;
            }
        }

        private void Cook(Semaphore semaphoreFull, Semaphore semaphoreEmpty, CookType type)
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

        private void Miner()
        {
            System.Console.WriteLine("Miner [{0}]:\t come", Id);
            bool hasEaten = false;
            while (!hasEaten)
            {
                counter.SoupsFull.WaitOne();
                if (counter.getWomenCount() == 0)
                {
                    counter.MainCoursesFull.WaitOne();
                    if (counter.getWomenCount() == 0)
                    {
                        counter.DessertsFull.WaitOne();

                        System.Console.WriteLine("Miner [{0}]:\t served", Id);
                        hasEaten = true;

                        counter.DessertsEmpty.Release();
                    }
                    counter.MainCoursesEmpty.Release();
                }
                counter.SoupsEmpty.Release();

            }
            System.Console.WriteLine("Miner [{0}]:\t gone", Id);
        }

        private void Child()
        {
            System.Console.WriteLine("Child [{0}]:\t come", Id);
            bool hasEaten = false;
            while (!hasEaten)
            {
                counter.SoupsFull.WaitOne();
                if (counter.getWomenCount() == 0)
                {
                    counter.DessertsFull.WaitOne();

                    System.Console.WriteLine("Child [{0}]:\t served", Id);
                    hasEaten = true;

                    counter.DessertsEmpty.Release();
                }
                counter.SoupsEmpty.Release();
            }
            System.Console.WriteLine("Child [{0}]:\t gone", Id);
        }

        private void Woman()
        {
            int womenCount = counter.incWomenCount();
            System.Console.WriteLine("WOMAN [{0}]:\t come \n There is {1} women in queue", Id, womenCount);

            counter.MainCoursesFull.WaitOne();
            counter.DessertsFull.WaitOne();
            System.Console.WriteLine("WOMAN [{0}]:\t served", Id);
            counter.DessertsEmpty.Release();
            counter.MainCoursesEmpty.Release();

            counter.decWomenCount();

            System.Console.WriteLine("WOMAN [{0}]:\t gone", Id);
        }

    }
}
