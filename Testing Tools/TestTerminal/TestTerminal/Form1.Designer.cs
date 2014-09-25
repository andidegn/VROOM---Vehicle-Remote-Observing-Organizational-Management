﻿namespace TestTerminal {
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.panel1 = new System.Windows.Forms.Panel();
            this.gb_connection = new System.Windows.Forms.GroupBox();
            this.btn_network_reg_status = new System.Windows.Forms.Button();
            this.btn_signal_strength = new System.Windows.Forms.Button();
            this.lbl_signal_strength = new System.Windows.Forms.Label();
            this.btn_error_report = new System.Windows.Forms.Button();
            this.gb_text_sms = new System.Windows.Forms.GroupBox();
            this.btn_send_msg = new System.Windows.Forms.Button();
            this.btn_read_msg = new System.Windows.Forms.Button();
            this.nud_msg = new System.Windows.Forms.NumericUpDown();
            this.btn_delete_all_msg = new System.Windows.Forms.Button();
            this.gb_call = new System.Windows.Forms.GroupBox();
            this.btn_set_ring_tone = new System.Windows.Forms.Button();
            this.nud_ring_tone = new System.Windows.Forms.NumericUpDown();
            this.lbl_ring_tone = new System.Windows.Forms.Label();
            this.btn_set_speaker_volume = new System.Windows.Forms.Button();
            this.btn_set_ringer_volume = new System.Windows.Forms.Button();
            this.lbl_speaker_vol = new System.Windows.Forms.Label();
            this.lbl_ringer_volume = new System.Windows.Forms.Label();
            this.nud_speaker_volume = new System.Windows.Forms.NumericUpDown();
            this.nud_ringer_volume = new System.Windows.Forms.NumericUpDown();
            this.btn_answer = new System.Windows.Forms.Button();
            this.btn_ctrl_z = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            this.cbb_baud_rate = new System.Windows.Forms.ComboBox();
            this.lbl_baud = new System.Windows.Forms.Label();
            this.btn_connect = new System.Windows.Forms.Button();
            this.tbx_send = new System.Windows.Forms.TextBox();
            this.rtb_terminal = new System.Windows.Forms.RichTextBox();
            this.cbb_com_port = new System.Windows.Forms.ComboBox();
            this.lbl_com_port = new System.Windows.Forms.Label();
            this.btn_clear = new System.Windows.Forms.Button();
            this.panel1.SuspendLayout();
            this.gb_connection.SuspendLayout();
            this.gb_text_sms.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nud_msg)).BeginInit();
            this.gb_call.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nud_ring_tone)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nud_speaker_volume)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nud_ringer_volume)).BeginInit();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.panel1.Controls.Add(this.btn_clear);
            this.panel1.Controls.Add(this.gb_connection);
            this.panel1.Controls.Add(this.btn_error_report);
            this.panel1.Controls.Add(this.gb_text_sms);
            this.panel1.Controls.Add(this.gb_call);
            this.panel1.Controls.Add(this.btn_ctrl_z);
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
            // gb_connection
            // 
            this.gb_connection.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.gb_connection.Controls.Add(this.btn_network_reg_status);
            this.gb_connection.Controls.Add(this.btn_signal_strength);
            this.gb_connection.Controls.Add(this.lbl_signal_strength);
            this.gb_connection.Location = new System.Drawing.Point(389, 6);
            this.gb_connection.Name = "gb_connection";
            this.gb_connection.Size = new System.Drawing.Size(160, 118);
            this.gb_connection.TabIndex = 18;
            this.gb_connection.TabStop = false;
            this.gb_connection.Text = "Connection";
            // 
            // btn_network_reg_status
            // 
            this.btn_network_reg_status.Location = new System.Drawing.Point(6, 48);
            this.btn_network_reg_status.Name = "btn_network_reg_status";
            this.btn_network_reg_status.Size = new System.Drawing.Size(75, 23);
            this.btn_network_reg_status.TabIndex = 17;
            this.btn_network_reg_status.Text = "Status";
            this.btn_network_reg_status.UseVisualStyleBackColor = true;
            this.btn_network_reg_status.Click += new System.EventHandler(this.btn_network_reg_status_Click);
            // 
            // btn_signal_strength
            // 
            this.btn_signal_strength.Location = new System.Drawing.Point(6, 19);
            this.btn_signal_strength.Name = "btn_signal_strength";
            this.btn_signal_strength.Size = new System.Drawing.Size(75, 23);
            this.btn_signal_strength.TabIndex = 15;
            this.btn_signal_strength.Text = "Signal Strength";
            this.btn_signal_strength.UseVisualStyleBackColor = true;
            this.btn_signal_strength.Click += new System.EventHandler(this.btn_signal_strength_Click);
            // 
            // lbl_signal_strength
            // 
            this.lbl_signal_strength.AutoSize = true;
            this.lbl_signal_strength.Location = new System.Drawing.Point(88, 24);
            this.lbl_signal_strength.Name = "lbl_signal_strength";
            this.lbl_signal_strength.Size = new System.Drawing.Size(13, 13);
            this.lbl_signal_strength.TabIndex = 16;
            this.lbl_signal_strength.Text = "0";
            // 
            // btn_error_report
            // 
            this.btn_error_report.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btn_error_report.Location = new System.Drawing.Point(470, 543);
            this.btn_error_report.Name = "btn_error_report";
            this.btn_error_report.Size = new System.Drawing.Size(75, 23);
            this.btn_error_report.TabIndex = 17;
            this.btn_error_report.Text = "Error Report";
            this.btn_error_report.UseVisualStyleBackColor = true;
            this.btn_error_report.Click += new System.EventHandler(this.btn_error_report_Click);
            // 
            // gb_text_sms
            // 
            this.gb_text_sms.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.gb_text_sms.Controls.Add(this.btn_send_msg);
            this.gb_text_sms.Controls.Add(this.btn_read_msg);
            this.gb_text_sms.Controls.Add(this.nud_msg);
            this.gb_text_sms.Controls.Add(this.btn_delete_all_msg);
            this.gb_text_sms.Location = new System.Drawing.Point(389, 268);
            this.gb_text_sms.Name = "gb_text_sms";
            this.gb_text_sms.Size = new System.Drawing.Size(160, 112);
            this.gb_text_sms.TabIndex = 14;
            this.gb_text_sms.TabStop = false;
            this.gb_text_sms.Text = "Text (SMS)";
            // 
            // btn_send_msg
            // 
            this.btn_send_msg.Location = new System.Drawing.Point(7, 78);
            this.btn_send_msg.Name = "btn_send_msg";
            this.btn_send_msg.Size = new System.Drawing.Size(75, 23);
            this.btn_send_msg.TabIndex = 13;
            this.btn_send_msg.Text = "Send MSG";
            this.btn_send_msg.UseVisualStyleBackColor = true;
            this.btn_send_msg.Click += new System.EventHandler(this.btn_send_msg_Click);
            // 
            // btn_read_msg
            // 
            this.btn_read_msg.Location = new System.Drawing.Point(6, 19);
            this.btn_read_msg.Name = "btn_read_msg";
            this.btn_read_msg.Size = new System.Drawing.Size(75, 23);
            this.btn_read_msg.TabIndex = 10;
            this.btn_read_msg.Text = "Read MSG";
            this.btn_read_msg.UseVisualStyleBackColor = true;
            this.btn_read_msg.Click += new System.EventHandler(this.btn_read_msg_Click);
            // 
            // nud_msg
            // 
            this.nud_msg.Location = new System.Drawing.Point(88, 21);
            this.nud_msg.Name = "nud_msg";
            this.nud_msg.Size = new System.Drawing.Size(67, 20);
            this.nud_msg.TabIndex = 11;
            // 
            // btn_delete_all_msg
            // 
            this.btn_delete_all_msg.Location = new System.Drawing.Point(6, 48);
            this.btn_delete_all_msg.Name = "btn_delete_all_msg";
            this.btn_delete_all_msg.Size = new System.Drawing.Size(75, 23);
            this.btn_delete_all_msg.TabIndex = 12;
            this.btn_delete_all_msg.Text = "Delete All";
            this.btn_delete_all_msg.UseVisualStyleBackColor = true;
            this.btn_delete_all_msg.Click += new System.EventHandler(this.btn_delete_all_msg_Click);
            // 
            // gb_call
            // 
            this.gb_call.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.gb_call.Controls.Add(this.btn_set_ring_tone);
            this.gb_call.Controls.Add(this.nud_ring_tone);
            this.gb_call.Controls.Add(this.lbl_ring_tone);
            this.gb_call.Controls.Add(this.btn_set_speaker_volume);
            this.gb_call.Controls.Add(this.btn_set_ringer_volume);
            this.gb_call.Controls.Add(this.lbl_speaker_vol);
            this.gb_call.Controls.Add(this.lbl_ringer_volume);
            this.gb_call.Controls.Add(this.nud_speaker_volume);
            this.gb_call.Controls.Add(this.nud_ringer_volume);
            this.gb_call.Controls.Add(this.btn_answer);
            this.gb_call.Location = new System.Drawing.Point(389, 130);
            this.gb_call.Name = "gb_call";
            this.gb_call.Size = new System.Drawing.Size(160, 132);
            this.gb_call.TabIndex = 13;
            this.gb_call.TabStop = false;
            this.gb_call.Text = "Call";
            // 
            // btn_set_ring_tone
            // 
            this.btn_set_ring_tone.Location = new System.Drawing.Point(116, 95);
            this.btn_set_ring_tone.Name = "btn_set_ring_tone";
            this.btn_set_ring_tone.Size = new System.Drawing.Size(38, 23);
            this.btn_set_ring_tone.TabIndex = 17;
            this.btn_set_ring_tone.Text = "Set";
            this.btn_set_ring_tone.UseVisualStyleBackColor = true;
            this.btn_set_ring_tone.Click += new System.EventHandler(this.btn_set_ring_tone_Click);
            // 
            // nud_ring_tone
            // 
            this.nud_ring_tone.Location = new System.Drawing.Point(74, 98);
            this.nud_ring_tone.Maximum = new decimal(new int[] {
            19,
            0,
            0,
            0});
            this.nud_ring_tone.Name = "nud_ring_tone";
            this.nud_ring_tone.Size = new System.Drawing.Size(36, 20);
            this.nud_ring_tone.TabIndex = 16;
            this.nud_ring_tone.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.nud_ring_tone_MouseDoubleClick);
            // 
            // lbl_ring_tone
            // 
            this.lbl_ring_tone.AutoSize = true;
            this.lbl_ring_tone.Location = new System.Drawing.Point(6, 100);
            this.lbl_ring_tone.Name = "lbl_ring_tone";
            this.lbl_ring_tone.Size = new System.Drawing.Size(57, 13);
            this.lbl_ring_tone.TabIndex = 15;
            this.lbl_ring_tone.Text = "Ring Tone";
            // 
            // btn_set_speaker_volume
            // 
            this.btn_set_speaker_volume.Location = new System.Drawing.Point(116, 69);
            this.btn_set_speaker_volume.Name = "btn_set_speaker_volume";
            this.btn_set_speaker_volume.Size = new System.Drawing.Size(38, 23);
            this.btn_set_speaker_volume.TabIndex = 14;
            this.btn_set_speaker_volume.Text = "Set";
            this.btn_set_speaker_volume.UseVisualStyleBackColor = true;
            this.btn_set_speaker_volume.Click += new System.EventHandler(this.btn_set_speaker_volume_Click);
            // 
            // btn_set_ringer_volume
            // 
            this.btn_set_ringer_volume.Location = new System.Drawing.Point(116, 43);
            this.btn_set_ringer_volume.Name = "btn_set_ringer_volume";
            this.btn_set_ringer_volume.Size = new System.Drawing.Size(38, 23);
            this.btn_set_ringer_volume.TabIndex = 13;
            this.btn_set_ringer_volume.Text = "Set";
            this.btn_set_ringer_volume.UseVisualStyleBackColor = true;
            this.btn_set_ringer_volume.Click += new System.EventHandler(this.btn_set_ringer_volume_Click);
            // 
            // lbl_speaker_vol
            // 
            this.lbl_speaker_vol.AutoSize = true;
            this.lbl_speaker_vol.Location = new System.Drawing.Point(6, 74);
            this.lbl_speaker_vol.Name = "lbl_speaker_vol";
            this.lbl_speaker_vol.Size = new System.Drawing.Size(68, 13);
            this.lbl_speaker_vol.TabIndex = 12;
            this.lbl_speaker_vol.Text = "Speaker Vol.";
            // 
            // lbl_ringer_volume
            // 
            this.lbl_ringer_volume.AutoSize = true;
            this.lbl_ringer_volume.Location = new System.Drawing.Point(6, 49);
            this.lbl_ringer_volume.Name = "lbl_ringer_volume";
            this.lbl_ringer_volume.Size = new System.Drawing.Size(59, 13);
            this.lbl_ringer_volume.TabIndex = 11;
            this.lbl_ringer_volume.Text = "Ringer Vol.";
            // 
            // nud_speaker_volume
            // 
            this.nud_speaker_volume.Location = new System.Drawing.Point(74, 72);
            this.nud_speaker_volume.Name = "nud_speaker_volume";
            this.nud_speaker_volume.Size = new System.Drawing.Size(36, 20);
            this.nud_speaker_volume.TabIndex = 10;
            this.nud_speaker_volume.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.nud_speaker_volume_MouseDoubleClick);
            // 
            // nud_ringer_volume
            // 
            this.nud_ringer_volume.Location = new System.Drawing.Point(74, 46);
            this.nud_ringer_volume.Maximum = new decimal(new int[] {
            4,
            0,
            0,
            0});
            this.nud_ringer_volume.Name = "nud_ringer_volume";
            this.nud_ringer_volume.Size = new System.Drawing.Size(36, 20);
            this.nud_ringer_volume.TabIndex = 9;
            this.nud_ringer_volume.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.nud_ringer_volume_MouseDoubleClick);
            // 
            // btn_answer
            // 
            this.btn_answer.Location = new System.Drawing.Point(6, 18);
            this.btn_answer.Name = "btn_answer";
            this.btn_answer.Size = new System.Drawing.Size(149, 23);
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
            this.cbb_baud_rate.SelectedIndexChanged += new System.EventHandler(this.cbb_baud_rate_SelectedIndexChanged);
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
            this.tbx_send.Size = new System.Drawing.Size(295, 20);
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
            // btn_clear
            // 
            this.btn_clear.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btn_clear.Location = new System.Drawing.Point(308, 543);
            this.btn_clear.Name = "btn_clear";
            this.btn_clear.Size = new System.Drawing.Size(75, 23);
            this.btn_clear.TabIndex = 19;
            this.btn_clear.Text = "Clear";
            this.btn_clear.UseVisualStyleBackColor = true;
            this.btn_clear.Click += new System.EventHandler(this.btn_clear_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(576, 593);
            this.Controls.Add(this.panel1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MinimumSize = new System.Drawing.Size(450, 310);
            this.Name = "Form1";
            this.Text = "Form1";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.gb_connection.ResumeLayout(false);
            this.gb_connection.PerformLayout();
            this.gb_text_sms.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.nud_msg)).EndInit();
            this.gb_call.ResumeLayout(false);
            this.gb_call.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nud_ring_tone)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nud_speaker_volume)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nud_ringer_volume)).EndInit();
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
        private System.Windows.Forms.Button btn_read_msg;
        private System.Windows.Forms.NumericUpDown nud_msg;
        private System.Windows.Forms.Button btn_delete_all_msg;
        private System.Windows.Forms.GroupBox gb_text_sms;
        private System.Windows.Forms.GroupBox gb_call;
        private System.Windows.Forms.Button btn_signal_strength;
        private System.Windows.Forms.Label lbl_signal_strength;
        private System.Windows.Forms.Button btn_send_msg;
        private System.Windows.Forms.NumericUpDown nud_speaker_volume;
        private System.Windows.Forms.NumericUpDown nud_ringer_volume;
        private System.Windows.Forms.Button btn_set_speaker_volume;
        private System.Windows.Forms.Button btn_set_ringer_volume;
        private System.Windows.Forms.Label lbl_speaker_vol;
        private System.Windows.Forms.Label lbl_ringer_volume;
        private System.Windows.Forms.NumericUpDown nud_ring_tone;
        private System.Windows.Forms.Label lbl_ring_tone;
        private System.Windows.Forms.Button btn_set_ring_tone;
        private System.Windows.Forms.Button btn_error_report;
        private System.Windows.Forms.GroupBox gb_connection;
        private System.Windows.Forms.Button btn_network_reg_status;
        private System.Windows.Forms.Button btn_clear;

    }
}

