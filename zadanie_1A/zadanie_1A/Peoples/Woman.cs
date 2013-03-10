using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace zadanie_1A
{
    class Woman : People, IPeople
    {
        public void Run()
        {
            int womenCount = counter.incWomenCount();
            if (womenCount == 1)
                System.Console.WriteLine("WOMAN [{0}]:\t come \n There is {1} women in queue", Id, womenCount);
            else
                System.Console.WriteLine("WOMAN [{0}]:\t come \n There are {1} woman in queue", Id, womenCount);

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
