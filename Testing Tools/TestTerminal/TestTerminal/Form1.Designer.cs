namespace TestTerminal {
    partial class Form1 {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing) {
            if (disposing && (components != null)) {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent() {
            this.panel1 = new System.Windows.Forms.Panel();
            this.button1 = new System.Windows.Forms.Button();
            this.cbb_baud_rate = new System.Windows.Forms.ComboBox();
            this.lbl_baud = new System.Windows.Forms.Label();
            this.btn_connect = new System.Windows.Forms.Button();
            this.tbx_send = new System.Windows.Forms.TextBox();
            this.rtb_terminal = new System.Windows.Forms.RichTextBox();
            this.cbb_com_port = new System.Windows.Forms.ComboBox();
            this.lbl_com_port = new System.Windows.Forms.Label();
            this.btn_answer = new System.Windows.Forms.Button();
            this.btn_ctrl_z = new System.Windows.Forms.Button();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.panel1.Controls.Add(this.btn_ctrl_z);
            this.panel1.Controls.Add(this.btn_answer);
            this.panel1.Controls.Add(this.button1);
            this.panel1.Controls.Add(this.cbb_baud_rate);
            this.panel1.Controls.Add(this.lbl_baud);
            this.panel1.Controls.Add(this.btn_connect);
            this.panel1.Controls.Add(this.tbx_send);
            this.panel1.Controls.Add(this.rtb_terminal);
            this.panel1.Controls.Add(this.cbb_com_port);
            this.panel1.Controls.Add(this.lbl_com_port);
            this.panel1.Location = new System.Drawing.Point(12, 12);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(552, 569);
            this.panel1.TabIndex = 0;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(104, 1);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(58, 23);
            this.button1.TabIndex = 7;
            this.button1.Text = "Refresh";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.btn_refresh_Click);
            // 
            // cbb_baud_rate
            // 
            this.cbb_baud_rate.FormattingEnabled = true;
            this.cbb_baud_rate.Location = new System.Drawing.Point(230, 3);
            this.cbb_baud_rate.Name = "cbb_baud_rate";
            this.cbb_baud_rate.Size = new System.Drawing.Size(71, 21);
            this.cbb_baud_rate.TabIndex = 6;
            // 
            // lbl_baud
            // 
            this.lbl_baud.AutoSize = true;
            this.lbl_baud.Location = new System.Drawing.Point(173, 6);
            this.lbl_baud.Name = "lbl_baud";
            this.lbl_baud.Size = new System.Drawing.Size(58, 13);
            this.lbl_baud.TabIndex = 5;
            this.lbl_baud.Text = "Baud Rate";
            // 
            // btn_connect
            // 
            this.btn_connect.Location = new System.Drawing.Point(308, 1);
            this.btn_connect.Name = "btn_connect";
            this.btn_connect.Size = new System.Drawing.Size(75, 23);
            this.btn_connect.TabIndex = 4;
            this.btn_connect.Text = "Connect";
            this.btn_connect.UseVisualStyleBackColor = true;
            this.btn_connect.Click += new System.EventHandler(this.btn_connect_Click);
            // 
            // tbx_send
            // 
            this.tbx_send.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tbx_send.Location = new System.Drawing.Point(6, 546);
            this.tbx_send.Name = "tbx_send";
            this.tbx_send.Size = new System.Drawing.Size(377, 20);
            this.tbx_send.TabIndex = 3;
            this.tbx_send.KeyDown += new System.Windows.Forms.KeyEventHandler(this.tbx_send_KeyDown);
            // 
            // rtb_terminal
            // 
            this.rtb_terminal.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.rtb_terminal.Location = new System.Drawing.Point(6, 30);
            this.rtb_terminal.Name = "rtb_terminal";
            this.rtb_terminal.Size = new System.Drawing.Size(377, 510);
            this.rtb_terminal.TabIndex = 2;
            this.rtb_terminal.Text = "";
            // 
            // cbb_com_port
            // 
            this.cbb_com_port.FormattingEnabled = true;
            this.cbb_com_port.Location = new System.Drawing.Point(27, 3);
            this.cbb_com_port.Name = "cbb_com_port";
            this.cbb_com_port.Size = new System.Drawing.Size(71, 21);
            this.cbb_com_port.TabIndex = 1;
            // 
            // lbl_com_port
            // 
            this.lbl_com_port.AutoSize = true;
            this.lbl_com_port.Location = new System.Drawing.Point(3, 6);
            this.lbl_com_port.Name = "lbl_com_port";
            this.lbl_com_port.Size = new System.Drawing.Size(26, 13);
            this.lbl_com_port.TabIndex = 0;
            this.lbl_com_port.Text = "Port";
            // 
            // btn_answer
            // 
            this.btn_answer.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btn_answer.Location = new System.Drawing.Point(389, 469);
            this.btn_answer.Name = "btn_answer";
            this.btn_answer.Size = new System.Drawing.Size(160, 71);
            this.btn_answer.TabIndex = 8;
            this.btn_answer.Text = "Answer";
            this.btn_answer.UseVisualStyleBackColor = true;
            this.btn_answer.Click += new System.EventHandler(this.btn_answer_Click);
            // 
            // btn_ctrl_z
            // 
            this.btn_ctrl_z.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btn_ctrl_z.Location = new System.Drawing.Point(389, 544);
            this.btn_ctrl_z.Name = "btn_ctrl_z";
            this.btn_ctrl_z.Size = new System.Drawing.Size(75, 23);
            this.btn_ctrl_z.TabIndex = 9;
            this.btn_ctrl_z.Text = "Ctrl+Z";
            this.btn_ctrl_z.UseVisualStyleBackColor = true;
            this.btn_ctrl_z.Click += new System.EventHandler(this.btn_ctrl_z_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(576, 593);
            this.Controls.Add(this.panel1);
            this.Name = "Form1";
            this.Text = "Form1";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Button btn_connect;
        private System.Windows.Forms.TextBox tbx_send;
        private System.Windows.Forms.RichTextBox rtb_terminal;
        private System.Windows.Forms.ComboBox cbb_com_port;
        private System.Windows.Forms.Label lbl_com_port;
        private System.Windows.Forms.ComboBox cbb_baud_rate;
        private System.Windows.Forms.Label lbl_baud;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button btn_answer;
        private System.Windows.Forms.Button btn_ctrl_z;

    }
}

