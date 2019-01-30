using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Knights_tour_WFA
{
    class CompareMriezky : IComparer<Mriezka>
    {
        public int Compare(Mriezka x, Mriezka y)
        {
            if (x.Ohodnotenie > y.Ohodnotenie)
                return -1;
            else if (x.Ohodnotenie < y.Ohodnotenie)
                return 1;
            else
                return 0;
        }
    }
}
