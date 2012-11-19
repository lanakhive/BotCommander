namespace serial
{
    partial class OptionsWin
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.label1 = new System.Windows.Forms.Label();
            this.portcombo = new System.Windows.Forms.ComboBox();
            this.settingsok = new System.Windows.Forms.Button();
            this.settings_cancel = new System.Windows.Forms.Button();
            this.checkBox1 = new System.Windows.Forms.CheckBox();
            this.saddr = new System.Windows.Forms.NumericUpDown();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.daddr = new System.Windows.Forms.NumericUpDown();
            ((System.ComponentModel.ISupportInitialize)(this.saddr)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.daddr)).BeginInit();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(13, 13);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(56, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "COM Port:";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.label1.Click += new System.EventHandler(this.label1_Click);
            // 
            // portcombo
            // 
            this.portcombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.portcombo.FormattingEnabled = true;
            this.portcombo.Location = new System.Drawing.Point(75, 10);
            this.portcombo.Name = "portcombo";
            this.portcombo.Size = new System.Drawing.Size(121, 21);
            this.portcombo.TabIndex = 1;
            // 
            // settingsok
            // 
            this.settingsok.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.settingsok.Location = new System.Drawing.Point(161, 124);
            this.settingsok.Name = "settingsok";
            this.settingsok.Size = new System.Drawing.Size(75, 23);
            this.settingsok.TabIndex = 2;
            this.settingsok.Text = "OK";
            this.settingsok.UseVisualStyleBackColor = true;
            this.settingsok.Click += new System.EventHandler(this.settingsok_Click);
            // 
            // settings_cancel
            // 
            this.settings_cancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.settings_cancel.Location = new System.Drawing.Point(80, 124);
            this.settings_cancel.Name = "settings_cancel";
            this.settings_cancel.Size = new System.Drawing.Size(75, 23);
            this.settings_cancel.TabIndex = 3;
            this.settings_cancel.Text = "Cancel";
            this.settings_cancel.UseVisualStyleBackColor = true;
            // 
            // checkBox1
            // 
            this.checkBox1.AutoSize = true;
            this.checkBox1.Location = new System.Drawing.Point(16, 89);
            this.checkBox1.Name = "checkBox1";
            this.checkBox1.Size = new System.Drawing.Size(114, 17);
            this.checkBox1.TabIndex = 5;
            this.checkBox1.Text = "Show Frame Bytes";
            this.checkBox1.UseVisualStyleBackColor = true;
            this.checkBox1.CheckedChanged += new System.EventHandler(this.checkBox1_CheckedChanged);
            // 
            // saddr
            // 
            this.saddr.Hexadecimal = true;
            this.saddr.Location = new System.Drawing.Point(126, 37);
            this.saddr.Maximum = new decimal(new int[] {
            255,
            0,
            0,
            0});
            this.saddr.Name = "saddr";
            this.saddr.Size = new System.Drawing.Size(40, 20);
            this.saddr.TabIndex = 6;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 39);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(85, 13);
            this.label2.TabIndex = 7;
            this.label2.Text = "Source Address:";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 65);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(104, 13);
            this.label3.TabIndex = 8;
            this.label3.Text = "Destination Address:";
            // 
            // daddr
            // 
            this.daddr.Hexadecimal = true;
            this.daddr.Location = new System.Drawing.Point(126, 63);
            this.daddr.Maximum = new decimal(new int[] {
            255,
            0,
            0,
            0});
            this.daddr.Name = "daddr";
            this.daddr.Size = new System.Drawing.Size(40, 20);
            this.daddr.TabIndex = 9;
            // 
            // OptionsWin
            // 
            this.AcceptButton = this.settingsok;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.settings_cancel;
            this.ClientSize = new System.Drawing.Size(248, 159);
            this.Controls.Add(this.daddr);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.saddr);
            this.Controls.Add(this.checkBox1);
            this.Controls.Add(this.settings_cancel);
            this.Controls.Add(this.settingsok);
            this.Controls.Add(this.portcombo);
            this.Controls.Add(this.label1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "OptionsWin";
            this.Text = "Setup";
            this.Load += new System.EventHandler(this.OptionsWin_Load);
            ((System.ComponentModel.ISupportInitialize)(this.saddr)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.daddr)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox portcombo;
        private System.Windows.Forms.Button settingsok;
        private System.Windows.Forms.Button settings_cancel;
        private System.Windows.Forms.CheckBox checkBox1;
        private System.Windows.Forms.NumericUpDown saddr;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.NumericUpDown daddr;
    }
}