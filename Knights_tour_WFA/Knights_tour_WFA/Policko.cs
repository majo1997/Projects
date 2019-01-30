using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Knights_tour_WFA
{
    class Policko
    {
        private bool _Navstivene;

        public bool Navstivene
        {
            get { return _Navstivene; }
            set { _Navstivene = value; }
        }

        public void Navstiv()
        {
            Navstivene = true;
        }

        public Policko()
        {
            Navstivene = false;
        }

        ~Policko()
        { }
    }
}
