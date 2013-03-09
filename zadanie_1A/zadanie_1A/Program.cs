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
        Desserts, Soups, MainCourses
    }

    public enum PersonType
    {
        Miner, Child, Women
    }

    class Program
    {
        static void Main(string[] args)
        {
            int n = 3;
            int m = 4;
            int k = 5;

            int nextPersonComeTime = 100;

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

            for (int i = 0; i < 100; i++)
            {
                Thread tc = new Thread(peoples.NextPerson);
                tc.Start();
                Thread.Sleep(new Random().Next(nextPersonComeTime));
            }
        }
    }
}
