using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
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
        private const int MinPreparationTime = 1;
        private const int MaxPreparationTime = 500;

        private static int prepareTime
        {
            get 
            { 
                return random.Next(MinPreparationTime, MaxPreparationTime); 
            }
        }

        public static void Cook(CookType type)
        {

        }

        public static void Miner()
        {

        }

        public static void Child()
        {

        }

        public static void Woman()
        {

        }


    }

    class Line
    {

    }

    class Program
    {
        static void Main(string[] args)
        {
        }
    }
}
