using System;
using System.Windows.Forms;
using System.Text.Json;
using Webclient.client;

namespace Webclient
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }


        private void comboBox1_SelectionChangeCommitted(object sender, EventArgs e)
        {
            Httpreq Httpreq = new Httpreq();
            Httpreq.HttpReq(comboBox1.Text);
            textBox3.Text = Httpreq.ReturnJson();
        }

        
    }

}
