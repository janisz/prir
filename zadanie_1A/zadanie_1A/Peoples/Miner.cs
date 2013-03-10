using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace zadanie_1A
{
    class Miner : People, IPeople
    {
        public void Run()
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
    }
}
