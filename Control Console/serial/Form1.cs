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
    public partial class Form1 : Form
    {
        private string port;
        private string command;
        private string rtext;
        private bool showbytes = false;
        private int stopcount = 0;
        private byte daddr = 0x01;
        private byte saddr = 0x02;
        frameDecode dec;

        public Form1()
        {
            InitializeComponent();
            ActiveControl = cmdbox;
            dec = new frameDecode(0x02);

        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (serialPort1.IsOpen)
                serialPort1.Close();
        }

        private void quitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void clearBufferToolStripMenuItem_Click(object sender, EventArgs e)
        {
            maintext.Clear();
            rtext = "";
        }

        private void setupToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OptionsWin options = new OptionsWin(showbytes,daddr,saddr);
            options.ShowDialog();
            if (options.DialogResult == DialogResult.OK)
            {
                port = options.getSelectedPort();
                showbytes = options.getShowRaw();
                dec.setAddr(options.getsaddr());
                daddr = options.getdaddr();
                if (!serialPort1.IsOpen)
                {
                    if (port.Count() > 0)
                    {
                        maintext.AppendText("Opening port: " + port + "\r\n");
                        serialPort1.PortName = port;
                        try
                        {
                            serialPort1.Open();
                            serialPort1.DtrEnable = true;
                            maintext.AppendText("Port Opened\r\n");
                        }
                        catch
                        {
                            maintext.AppendText("Failed to open port.\r\n");
                        }
                    }
                }
            }
        }

        private void submitb_Click(object sender, EventArgs e)
        {
            command = cmdbox.Text.Trim().ToUpper();
            if (command.Count() > 0 && serialPort1.IsOpen)
            {
                maintext.AppendText("> " + command + "\r\n");
                string encoded = encodeFrame(command);
                if (showbytes)
                    maintext.AppendText("TX Frame: " + BitConverter.ToString(Encoding.ASCII.GetBytes(encoded)).Replace('-', ' ') + "\r\n");
                maintext.AppendText("\r\n");
                serialPort1.Write(encoded);
                cmdbox.Clear();
                
            }
        }

        private void maintext_TextChanged(object sender, EventArgs e)
        {
            //autoscroll
            maintext.SelectionStart = maintext.Text.Length;
            maintext.ScrollToCaret();
        }

        private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            //rtext = serialPort1.ReadExisting();
            byte b;
            while (serialPort1.BytesToRead > 0)
            {
                b = (byte)serialPort1.ReadByte();
                rtext += (char)b;
                if (dec.frameDecoder(b))
                {
                    this.BeginInvoke(new Action(() =>
                    {
                        this.showRXText();
                    }));
                }
            }
        }

        private void showRXText()
        {
            string decoded = dec.getDecoded();
            if (rtext != null && decoded != null)
            {
                if (showbytes)
                    maintext.AppendText("RX Frame: " + BitConverter.ToString(Encoding.ASCII.GetBytes(rtext)).Replace('-', ' ') + "\r\n");
                maintext.AppendText("< " + decoded);
                rtext = "";
                maintext.AppendText("\r\n\r\n");
            }
            else
            {
                maintext.AppendText("RX Frame: " + BitConverter.ToString(Encoding.ASCII.GetBytes(rtext)).Replace('-', ' ') + "\r\n");
                maintext.AppendText("Bad Transmission Error\r\n");
            }
        }

        private string encodeFrame(string src)
        {
            byte checksum = 0;
            string dest = "";
            foreach (char c in src) checksum ^= (byte)c;
            return dest = "~" + (char)(byte)daddr + (char)(byte)src.Length + src + (char)checksum;
        }
    }
}
