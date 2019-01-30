using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Threading;

namespace Knights_tour_WFA
{
    public partial class Form1 : Form
    {
        PictureBox[,] _Pictures;

        public Form1()
        {
            InitializeComponent();
        }

        private void NajdiRiesenie_Click(object sender, EventArgs e)
        {
            int RozmerStrany ;//= Convert.ToInt32(textBox1.Text);
            int ZaciatokX ;//= Convert.ToInt32(textBox2.Text);
            int ZaciatokY ;//= Convert.ToInt32(textBox3.Text);
            int PredchadzajuceX = 0;
            int PredchadzajuceY = 0;
            //test intervalu rozmerstrany...
            //indexoutofbounds excpt.

            try
            {
                RozmerStrany = Convert.ToInt32(textBox1.Text);
                ZaciatokX = Convert.ToInt32(textBox2.Text);
                ZaciatokY = Convert.ToInt32(textBox3.Text);
            }
            catch (FormatException fEx)
            {
                MessageBox.Show("Nezadali ste vsetky udaje!");
                //pouziva sa fEx.toString(); vypis
                return;
            }
            catch (Exception Ex)
            {
                return;
            }

            //toto asi zmenit
            /*if (ZaciatokX < 0 || ZaciatokY < 0 || ZaciatokX >= RozmerStrany || ZaciatokY >= RozmerStrany)
            {
                MessageBox.Show("Zadajte suradnice v intervale <0, Rozmer strany) !");
                return;
            }*/
            SkryElements();

            //logika tu nebude
            //tu na tento konstr. by mohla byt trycatch
            Mriezka m = new Mriezka(RozmerStrany, ZaciatokX, ZaciatokY);

            HashSet<Mriezka> hash = new HashSet<Mriezka>();

            Stack<Mriezka> fronta = new Stack<Mriezka>();

            while (!m.JeHotova())
            {
                if (hash.Add(m))
                {

                    foreach (var vm in m.VygenerujTahy(m))
                    {
                        fronta.Push(vm);
                    }

                }

                if (fronta.Count != 0)
                    m = fronta.Pop();
                else
                {
                    MessageBox.Show("Nenaslo sa riesenie!");
                    return;
                }
            }

            List<int> suradniceX = m.SuradniceX();
            List<int> suradniceY = m.SuradniceY();

            suradniceX.Reverse();
            suradniceY.Reverse();

            VykresliSachovnicu(RozmerStrany);

            Thread t = new Thread(() =>//keby je parametrized tak by tam slo p namiesto()
            {
                for (int i = 0; i < suradniceX.Count; i++)
                {
                    if (i != 0)
                        _Pictures[PredchadzajuceX, PredchadzajuceY].Image = Image.FromFile(@"C:\Users\Mário\Desktop\Git repositories\Knights_tour_WFA\Knights_tour_WFA\Resources\imgs\red.jpg");
 
                    _Pictures[suradniceX[i], suradniceY[i]].Image = Image.FromFile(@"C:\Users\Mário\Documents\visual studio 2012\Projects\Knights_tour_WFA\Knights_tour_WFA\Resources\imgs\knight.jpg");
                    PredchadzajuceX = suradniceX[i];
                    PredchadzajuceY = suradniceY[i];
                    Thread.Sleep(500);

                }
                _Pictures[PredchadzajuceX, PredchadzajuceY].Image = Image.FromFile(@"C:\Users\Mário\Desktop\Git repositories\Knights_tour_WFA\Knights_tour_WFA\Resources\imgs\red.jpg");
            });
            t.Start();

            //OdkryElements();
        }

        public void SkryElements()
        {
            textBox1.Hide();
            textBox2.Hide();
            textBox3.Hide();
            NajdiRiesenie.Hide();
            label1.Hide();
            label2.Hide();
            label3.Hide();
        }

        public void OdkryElements()//bude v evente tlacidla
        {
            this.Width = 652;
            this.Height = 428;

            textBox1.Show();
            textBox2.Show();
            textBox3.Show();
            NajdiRiesenie.Show();
            label1.Show();
            label2.Show();
            label3.Show();
        }

        public void VykresliSachovnicu(int RozmerStrany)
        {
            _Pictures = new PictureBox[RozmerStrany, RozmerStrany];
            for (int i = 0; i < _Pictures.GetLength(0); i++)
            {
                for (int j = 0; j < _Pictures.GetLength(1); j++)
                {
                    _Pictures[i, j] = new PictureBox();
                    _Pictures[i, j].Size = new Size(50, 50);
                    _Pictures[i, j].Location = new Point(50 + (i * 50), 50 + (j * 50));
                    if ((i % 2 == 0 && j % 2 == 0) || (i % 2 == 1 && j % 2 == 1))
                        _Pictures[i, j].Image = Image.FromFile(@"C:\Users\Mário\Documents\visual studio 2012\Projects\Knights_tour_WFA\Knights_tour_WFA\Resources\imgs\black.jpg");
                    else
                        _Pictures[i, j].Image = Image.FromFile(@"C:\Users\Mário\Documents\visual studio 2012\Projects\Knights_tour_WFA\Knights_tour_WFA\Resources\imgs\white.jpg");

                    this.Controls.Add(_Pictures[i, j]);
                }
            }

            Button btnNew = new Button();
            btnNew.Click += new EventHandler(btnNew_Click);
            btnNew.Size = NajdiRiesenie.Size;
            btnNew.Text = " Znova";
            btnNew.Location = new Point(50, 15);
            this.Controls.Add(btnNew);

            this.Width = 100 + RozmerStrany * 50;
            this.Height = 100 + RozmerStrany * 50;
        }

        private void btnNew_Click(object sender, EventArgs e)
        {
            MessageBox.Show("tu to skonci odkryje hlavne okno");
            OdkryElements();
        }

    }
}
