using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Knights_tour_WFA
{
    class Mriezka
    {
        private Policko[,] _Policka;
        private int _PoziciaX;
        private int _PoziciaY;
        private int _RozmerStrany;
        private int _PocetNavstivenych = 0;
        private int _Ohodnotenie;
        private Mriezka _Predchadzajuca;

        public Policko this[int index1, int index2]
        {
            get { return _Policka[index1, index2]; }
            set { _Policka[index1, index2] = value; }
        }

        public int PoziciaX 
        {
            get { return _PoziciaX; }
            set { _PoziciaX = value; }
        }

        public int PoziciaY
        {
            get { return _PoziciaY; }
            set { _PoziciaY = value; }
        }

        public int RozmerStrany 
        {
            get { return _RozmerStrany; }
            set { _RozmerStrany = value; }
        }

        public int PocetNavstivenych
        {
            get { return _PocetNavstivenych; }
            set { _PocetNavstivenych = value; }
        }

        public int Ohodnotenie
        {
            get { return _Ohodnotenie; }
            set { _Ohodnotenie = value; }
        }

        public Mriezka Predchadzajuca
        {
            get { return _Predchadzajuca; }
            set { _Predchadzajuca = value; }
        }

        public bool JeHotova()
        {          
            if (PocetNavstivenych == Math.Pow(RozmerStrany, 2))
                return true;

            return false;
        }

        public List<Mriezka> VygenerujTahy(Mriezka m)
        { 
            List<Mriezka> list = new List<Mriezka>();

            for (int i = 0; i < 8; i++)//tu by mohol byt multi-threading
            {
                var n = m.GenerujTah(i);
                if (n != null)
                {
                    n.Ohodnot();
                    list.Add(n);
                }
            }

            CompareMriezky cm = new CompareMriezky();
            list.Sort(cm);

            return list;
        }

        public Mriezka GenerujTah(int i)
        {
            Mriezka n = null;
            switch (i)
            {
                case 0:
                    n = this.SkusVytvoritMriezku(2, 1);
                    break;
                case 1:
                    n = this.SkusVytvoritMriezku(2, -1);
                    break;
                case 2:
                    n = this.SkusVytvoritMriezku(-2, 1);
                    break;
                case 3:
                    n = this.SkusVytvoritMriezku(-2, -1);
                    break;
                case 4:
                    n = this.SkusVytvoritMriezku(1, 2);
                    break;
                case 5:
                    n = this.SkusVytvoritMriezku(1, -2);
                    break;
                case 6:
                    n = this.SkusVytvoritMriezku(-1, 2);
                    break;
                case 7:
                    n = this.SkusVytvoritMriezku(-1, -2);
                    break;
            }
            return n;
        }

        public void Navstiv(Mriezka m, int NoveX, int NoveY)
        {
            this[NoveX, NoveY].Navstiv();
            this.PocetNavstivenych++;
        }

        public Mriezka SkusVytvoritMriezku(int x, int y)
        {
            if (this.PoziciaX + x < RozmerStrany && this.PoziciaX + x >= 0 && this.PoziciaY + y < RozmerStrany && this.PoziciaY + y >= 0 && this[this.PoziciaX + x, this.PoziciaY + y].Navstivene == false)
            {
                return new Mriezka(this, this.PoziciaX + x, this.PoziciaY + y);
            }
            else
                return null;
        }

        public void Ohodnot()
        { 
            int ohodnotenie = 0;

            /*toto zmenit!!!!!!!!!!*/
            if(this.TestNenavstivenehoPolicka(2, 1))
                ohodnotenie++;
            if(this.TestNenavstivenehoPolicka(2, -1))
                ohodnotenie++;
            if(this.TestNenavstivenehoPolicka(-2, 1))
                ohodnotenie++;
            if(this.TestNenavstivenehoPolicka(-2, -1))
                ohodnotenie++;
            if(this.TestNenavstivenehoPolicka(1, 2))
                ohodnotenie++;
            if(this.TestNenavstivenehoPolicka(1, -2))
                ohodnotenie++;
            if(this.TestNenavstivenehoPolicka(-1, 2))
                ohodnotenie++;
            if(this.TestNenavstivenehoPolicka(-1, -2))
                ohodnotenie++;

            this.Ohodnotenie = ohodnotenie;
        }

        public bool TestNenavstivenehoPolicka(int x, int y)
        {
            if (this.PoziciaX + x >= 0 && this.PoziciaX + x < RozmerStrany && this.PoziciaY + y >= 0 && this.PoziciaY + y < RozmerStrany && this[this.PoziciaX + x, this.PoziciaY + y].Navstivene == false)
                return true;
            return false;
        }

        public List<int> SuradniceX()
        { 
            List<int> suradniceX = new List<int>();
            Mriezka m = this;
            while (m.Predchadzajuca != null)
            {
                suradniceX.Add(m.PoziciaX);
                m = m.Predchadzajuca;
            }
            suradniceX.Add(m.PoziciaX);

            return suradniceX;
        }

        public List<int> SuradniceY()
        {
            List<int> suradniceY = new List<int>();
            Mriezka m = this;
            while (m.Predchadzajuca != null)
            {
                suradniceY.Add(m.PoziciaY);
                m = m.Predchadzajuca;
            }
            suradniceY.Add(m.PoziciaY);

            return suradniceY;
        }

        public override int GetHashCode()
        {
            return 0;
        }

        public override bool Equals(object obj)
        {
            var m = (Mriezka)obj;

            bool b = false;
            int i;

            for (i = 0; i < this._Policka.GetLength(0); i++ )
            {
                for (int j = 0; j < this._Policka.GetLength(1); j++ )
                {
                    if (this[i, j].Navstivene != m[i, j].Navstivene)
                    {
                        b = true;
                        break;
                    }
                }
                if (b)
                    break;
            }
            if (b)
                return false;

            return true;
        }

        public Mriezka(int RozmerStrany, int ZaciatokX, int ZaciatokY)
        {
            this.RozmerStrany = RozmerStrany;
            _Policka = new Policko[RozmerStrany, RozmerStrany];
            for (int i = 0; i < _Policka.GetLength(1); i++)
            {
                for (int j = 0; j < _Policka.GetLength(0); j++)
                {
                    _Policka[j, i] = new Policko();
                }
            }
            Navstiv(this, ZaciatokX, ZaciatokY);
            PoziciaX = ZaciatokX;
            PoziciaY = ZaciatokY;
            Predchadzajuca = null;
        }

        public Mriezka(Mriezka m, int NoveX, int NoveY)
        {
            this.RozmerStrany = m.RozmerStrany;

            _Policka = new Policko[RozmerStrany, RozmerStrany];
            for (int i = 0; i < _Policka.GetLength(0); i++)
            {
                for (int j = 0; j < _Policka.GetLength(1); j++)
                {
                    _Policka[i, j] = new Policko();
                    _Policka[i, j].Navstivene = m[i, j].Navstivene; 
                }
            }

            this.PocetNavstivenych = m.PocetNavstivenych;
            this.PoziciaX = NoveX;
            this.PoziciaY = NoveY;

            Navstiv(this, NoveX, NoveY);

            this.Predchadzajuca = m;
        }

        ~Mriezka(){ }
    }
}
