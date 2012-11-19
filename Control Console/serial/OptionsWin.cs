using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;

namespace serial
{
    public partial class OptionsWin : Form
    {
        private StringBuilder recievedData = new StringBuilder();

        public OptionsWin(bool raw, byte dadr, byte sadr)
        {
            InitializeComponent();
            checkBox1.Checked = raw;
            saddr.Value = sadr;
            daddr.Value = dadr;
        }

        public string getSelectedPort()
        {
            return portcombo.Text;
        }

        public bool getShowRaw()
        {
            return checkBox1.Checked;
        }

        public byte getsaddr()
        {
            return (byte)saddr.Value;
        }

        public byte getdaddr()
        {
            return (byte)daddr.Value;
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void OptionsWin_Load(object sender, EventArgs e)
        {
            foreach (string portname in SerialPort.GetPortNames())
            {
                portcombo.Items.Add(portname);
            }
            if (portcombo.Items.Count == 0)
            {
                portcombo.Text = "None Found";
                portcombo.Enabled = false;
            }
            else
            {
                portcombo.SelectedIndex = 0;
            }
        }

        private void settingsok_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            
        }

    }
}
