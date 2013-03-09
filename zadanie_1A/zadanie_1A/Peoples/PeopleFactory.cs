using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace zadanie_1A
{
    class PeopleFactory
    {
        private static Random random = new Random();
        private static Counter counter = Counter.Instance;

        public static IPeople getNextClient()
        {
            PersonType person = RandomEnum<PersonType>();
            switch (person)
            {
                case PersonType.Miner: return new Miner();
                case PersonType.Child: return new Child();
                case PersonType.Women: return new Woman();
            }

            throw new IndexOutOfRangeException();
        }

        public static IPeople getCook(object type)
        {
            CookType t = (CookType)type;
            switch ((CookType)t)
            {
                case CookType.Desserts:
                    return new Cook(counter.DessertsFull, counter.DessertsEmpty, t);
                case CookType.Soups:
                    return new Cook(counter.SoupsFull, counter.SoupsEmpty, t);
                case CookType.MainCourses:
                    return new Cook(counter.MainCoursesFull, counter.MainCoursesEmpty, t);
            }

            throw new IndexOutOfRangeException();
        }

        public static T RandomEnum<T>()
        {
            T[] values = (T[])Enum.GetValues(typeof(T));
            return values[random.Next(0, values.Length)];
        }
    }
}
