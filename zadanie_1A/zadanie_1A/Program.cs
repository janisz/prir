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

            for (int i = 0; i < n; ++i)
            {
                var cook = PeopleFactory.getCook(CookType.Soups);
                Thread tc = new Thread(cook.Run);
                tc.Start();
            }
            for (int i = 0; i < m; ++i)
            {
                var cook = PeopleFactory.getCook(CookType.MainCourses);
                Thread tc = new Thread(cook.Run);
                tc.Start();
            }
            for (int i = 0; i < k; ++i)
            {
                var cook = PeopleFactory.getCook(CookType.Desserts);
                Thread tc = new Thread(cook.Run);
                tc.Start();
            }

            for (int i = 0; i < 100; i++)
            {
                var person = PeopleFactory.getNextClient();
                Thread tc = new Thread(person.Run);
                tc.Start();
                Thread.Sleep(new Random().Next(nextPersonComeTime));
            }
        }
    }
}
