namespace Accelerometer_Analyzer {
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
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea1 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend1 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series1 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.chart1 = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.btn_start = new System.Windows.Forms.Button();
            this.rtb_data = new System.Windows.Forms.RichTextBox();
            this.chkFullScale = new System.Windows.Forms.CheckBox();
            this.tkb_scale = new System.Windows.Forms.TrackBar();
            this.tbx_send = new System.Windows.Forms.TextBox();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveAsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.printToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.pnl_chart = new System.Windows.Forms.Panel();
            this.grp_axis = new System.Windows.Forms.GroupBox();
            this.chk_v_len = new System.Windows.Forms.CheckBox();
            this.chk_z = new System.Windows.Forms.CheckBox();
            this.chk_y = new System.Windows.Forms.CheckBox();
            this.chk_x = new System.Windows.Forms.CheckBox();
            this.tkb_length_offset = new System.Windows.Forms.TrackBar();
            this.btn_raw = new System.Windows.Forms.Button();
            this.lbl_send = new System.Windows.Forms.Label();
            this.btn_rst_buf = new System.Windows.Forms.Button();
            this.backgroundWorker1 = new System.ComponentModel.BackgroundWorker();
            this.chk_raw_data = new System.Windows.Forms.CheckBox();
            this.cbb_comport = new System.Windows.Forms.ComboBox();
            this.btn_refresh = new System.Windows.Forms.Button();
            this.cbb_baud_rate = new System.Windows.Forms.ComboBox();
            this.lbl_baud_rate = new System.Windows.Forms.Label();
            this.btn_stop = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.chart1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.tkb_scale)).BeginInit();
            this.menuStrip1.SuspendLayout();
            this.pnl_chart.SuspendLayout();
            this.grp_axis.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.tkb_length_offset)).BeginInit();
            this.SuspendLayout();
            // 
            // chart1
            // 
            this.chart1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.chart1.BackSecondaryColor = System.Drawing.Color.FromArgb(((int)(((byte)(224)))), ((int)(((byte)(224)))), ((int)(((byte)(224)))));
            this.chart1.BorderlineColor = System.Drawing.Color.Black;
            this.chart1.BorderlineDashStyle = System.Windows.Forms.DataVisualization.Charting.ChartDashStyle.Solid;
            this.chart1.BorderlineWidth = 2;
            chartArea1.Name = "ChartArea1";
            this.chart1.ChartAreas.Add(chartArea1);
            this.chart1.Cursor = System.Windows.Forms.Cursors.Default;
            legend1.Name = "Legend1";
            this.chart1.Legends.Add(legend1);
            this.chart1.Location = new System.Drawing.Point(12, 0);
            this.chart1.Name = "chart1";
            this.chart1.Palette = System.Windows.Forms.DataVisualization.Charting.ChartColorPalette.Bright;
            series1.ChartArea = "ChartArea1";
            series1.Legend = "Legend1";
            series1.Name = "Series1";
            this.chart1.Series.Add(series1);
            this.chart1.Size = new System.Drawing.Size(1041, 469);
            this.chart1.SuppressExceptions = true;
            this.chart1.TabIndex = 0;
            this.chart1.Text = "chart1";
            this.chart1.TextAntiAliasingQuality = System.Windows.Forms.DataVisualization.Charting.TextAntiAliasingQuality.SystemDefault;
            this.chart1.MouseEnter += new System.EventHandler(this.chart1_MouseEnter);
            // 
            // btn_start
            // 
            this.btn_start.Location = new System.Drawing.Point(54, 2);
            this.btn_start.Name = "btn_start";
            this.btn_start.Size = new System.Drawing.Size(75, 23);
            this.btn_start.TabIndex = 1;
            this.btn_start.Text = "Start";
            this.btn_start.UseVisualStyleBackColor = true;
            this.btn_start.Click += new System.EventHandler(this.btn_start_Click);
            // 
            // rtb_data
            // 
            this.rtb_data.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.rtb_data.Location = new System.Drawing.Point(869, 27);
            this.rtb_data.Name = "rtb_data";
            this.rtb_data.Size = new System.Drawing.Size(193, 469);
            this.rtb_data.TabIndex = 3;
            this.rtb_data.Text = "";
            this.rtb_data.Visible = false;
            // 
            // chkFullScale
            // 
            this.chkFullScale.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.chkFullScale.AutoSize = true;
            this.chkFullScale.Enabled = false;
            this.chkFullScale.Location = new System.Drawing.Point(976, 488);
            this.chkFullScale.Name = "chkFullScale";
            this.chkFullScale.Size = new System.Drawing.Size(67, 17);
            this.chkFullScale.TabIndex = 4;
            this.chkFullScale.Text = "Fullscale";
            this.chkFullScale.UseVisualStyleBackColor = true;
            // 
            // tkb_scale
            // 
            this.tkb_scale.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tkb_scale.Location = new System.Drawing.Point(0, 475);
            this.tkb_scale.Maximum = 10000;
            this.tkb_scale.Name = "tkb_scale";
            this.tkb_scale.Size = new System.Drawing.Size(970, 45);
            this.tkb_scale.TabIndex = 5;
            this.tkb_scale.Value = 400;
            this.tkb_scale.Scroll += new System.EventHandler(this.tkb_scale_Scroll);
            this.tkb_scale.MouseEnter += new System.EventHandler(this.tkb_scale_MouseEnter);
            // 
            // tbx_send
            // 
            this.tbx_send.Location = new System.Drawing.Point(700, 3);
            this.tbx_send.Name = "tbx_send";
            this.tbx_send.Size = new System.Drawing.Size(154, 20);
            this.tbx_send.TabIndex = 6;
            this.tbx_send.KeyDown += new System.Windows.Forms.KeyEventHandler(this.tbx_send_KeyDown);
            // 
            // menuStrip1
            // 
            this.menuStrip1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.menuStrip1.Dock = System.Windows.Forms.DockStyle.None;
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(45, 24);
            this.menuStrip1.TabIndex = 8;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.openToolStripMenuItem,
            this.saveAsToolStripMenuItem,
            this.printToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // openToolStripMenuItem
            // 
            this.openToolStripMenuItem.Name = "openToolStripMenuItem";
            this.openToolStripMenuItem.Size = new System.Drawing.Size(123, 22);
            this.openToolStripMenuItem.Text = "Open";
            this.openToolStripMenuItem.Click += new System.EventHandler(this.openToolStripMenuItem_Click);
            // 
            // saveAsToolStripMenuItem
            // 
            this.saveAsToolStripMenuItem.Name = "saveAsToolStripMenuItem";
            this.saveAsToolStripMenuItem.Size = new System.Drawing.Size(123, 22);
            this.saveAsToolStripMenuItem.Text = "Save As...";
            this.saveAsToolStripMenuItem.Click += new System.EventHandler(this.saveAsToolStripMenuItem_Click);
            // 
            // printToolStripMenuItem
            // 
            this.printToolStripMenuItem.Name = "printToolStripMenuItem";
            this.printToolStripMenuItem.Size = new System.Drawing.Size(123, 22);
            this.printToolStripMenuItem.Text = "Print";
            this.printToolStripMenuItem.Click += new System.EventHandler(this.printToolStripMenuItem_Click);
            // 
            // pnl_chart
            // 
            this.pnl_chart.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.pnl_chart.Controls.Add(this.grp_axis);
            this.pnl_chart.Controls.Add(this.tkb_length_offset);
            this.pnl_chart.Controls.Add(this.chkFullScale);
            this.pnl_chart.Controls.Add(this.tkb_scale);
            this.pnl_chart.Controls.Add(this.chart1);
            this.pnl_chart.Location = new System.Drawing.Point(0, 27);
            this.pnl_chart.Name = "pnl_chart";
            this.pnl_chart.Size = new System.Drawing.Size(1065, 523);
            this.pnl_chart.TabIndex = 10;
            // 
            // grp_axis
            // 
            this.grp_axis.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.grp_axis.BackColor = System.Drawing.SystemColors.Window;
            this.grp_axis.Controls.Add(this.chk_v_len);
            this.grp_axis.Controls.Add(this.chk_z);
            this.grp_axis.Controls.Add(this.chk_y);
            this.grp_axis.Controls.Add(this.chk_x);
            this.grp_axis.Location = new System.Drawing.Point(936, 181);
            this.grp_axis.Name = "grp_axis";
            this.grp_axis.Size = new System.Drawing.Size(103, 120);
            this.grp_axis.TabIndex = 7;
            this.grp_axis.TabStop = false;
            this.grp_axis.Text = "Axis";
            // 
            // chk_v_len
            // 
            this.chk_v_len.AutoSize = true;
            this.chk_v_len.Location = new System.Drawing.Point(16, 88);
            this.chk_v_len.Name = "chk_v_len";
            this.chk_v_len.Size = new System.Drawing.Size(49, 17);
            this.chk_v_len.TabIndex = 3;
            this.chk_v_len.Text = "v-len";
            this.chk_v_len.UseVisualStyleBackColor = true;
            this.chk_v_len.CheckedChanged += new System.EventHandler(this.chk_v_len_CheckedChanged);
            // 
            // chk_z
            // 
            this.chk_z.AutoSize = true;
            this.chk_z.Location = new System.Drawing.Point(16, 65);
            this.chk_z.Name = "chk_z";
            this.chk_z.Size = new System.Drawing.Size(52, 17);
            this.chk_z.TabIndex = 2;
            this.chk_z.Text = "z-axis";
            this.chk_z.UseVisualStyleBackColor = true;
            this.chk_z.CheckedChanged += new System.EventHandler(this.chk_z_CheckedChanged);
            // 
            // chk_y
            // 
            this.chk_y.AutoSize = true;
            this.chk_y.Location = new System.Drawing.Point(16, 42);
            this.chk_y.Name = "chk_y";
            this.chk_y.Size = new System.Drawing.Size(52, 17);
            this.chk_y.TabIndex = 1;
            this.chk_y.Text = "y-axis";
            this.chk_y.UseVisualStyleBackColor = true;
            this.chk_y.CheckedChanged += new System.EventHandler(this.chk_y_CheckedChanged);
            // 
            // chk_x
            // 
            this.chk_x.AutoSize = true;
            this.chk_x.Location = new System.Drawing.Point(16, 19);
            this.chk_x.Name = "chk_x";
            this.chk_x.Size = new System.Drawing.Size(52, 17);
            this.chk_x.TabIndex = 0;
            this.chk_x.Text = "x-axis";
            this.chk_x.UseVisualStyleBackColor = true;
            this.chk_x.CheckedChanged += new System.EventHandler(this.chk_x_CheckedChanged);
            // 
            // tkb_length_offset
            // 
            this.tkb_length_offset.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.tkb_length_offset.BackColor = System.Drawing.SystemColors.Window;
            this.tkb_length_offset.Location = new System.Drawing.Point(14, 4);
            this.tkb_length_offset.Maximum = 2000;
            this.tkb_length_offset.Minimum = -2000;
            this.tkb_length_offset.Name = "tkb_length_offset";
            this.tkb_length_offset.Orientation = System.Windows.Forms.Orientation.Vertical;
            this.tkb_length_offset.Size = new System.Drawing.Size(45, 461);
            this.tkb_length_offset.TabIndex = 6;
            this.tkb_length_offset.TickStyle = System.Windows.Forms.TickStyle.Both;
            this.tkb_length_offset.Scroll += new System.EventHandler(this.tkb_length_offset_Scroll);
            this.tkb_length_offset.KeyDown += new System.Windows.Forms.KeyEventHandler(this.tkb_length_offset_KeyDown);
            this.tkb_length_offset.MouseEnter += new System.EventHandler(this.tkb_length_offset_MouseEnter);
            // 
            // btn_raw
            // 
            this.btn_raw.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btn_raw.Location = new System.Drawing.Point(990, 2);
            this.btn_raw.Name = "btn_raw";
            this.btn_raw.Size = new System.Drawing.Size(75, 23);
            this.btn_raw.TabIndex = 11;
            this.btn_raw.Text = "Show";
            this.btn_raw.UseVisualStyleBackColor = true;
            this.btn_raw.Click += new System.EventHandler(this.btn_raw_Click);
            // 
            // lbl_send
            // 
            this.lbl_send.AutoSize = true;
            this.lbl_send.Location = new System.Drawing.Point(662, 6);
            this.lbl_send.Name = "lbl_send";
            this.lbl_send.Size = new System.Drawing.Size(32, 13);
            this.lbl_send.TabIndex = 12;
            this.lbl_send.Text = "Send";
            // 
            // btn_rst_buf
            // 
            this.btn_rst_buf.Location = new System.Drawing.Point(135, 2);
            this.btn_rst_buf.Name = "btn_rst_buf";
            this.btn_rst_buf.Size = new System.Drawing.Size(75, 23);
            this.btn_rst_buf.TabIndex = 13;
            this.btn_rst_buf.Text = "Reset Buffer";
            this.btn_rst_buf.UseVisualStyleBackColor = true;
            this.btn_rst_buf.Click += new System.EventHandler(this.btn_rst_buf_Click);
            // 
            // chk_raw_data
            // 
            this.chk_raw_data.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.chk_raw_data.AutoSize = true;
            this.chk_raw_data.Location = new System.Drawing.Point(876, 5);
            this.chk_raw_data.Name = "chk_raw_data";
            this.chk_raw_data.Size = new System.Drawing.Size(108, 17);
            this.chk_raw_data.TabIndex = 15;
            this.chk_raw_data.Text = "Raw Data collect";
            this.chk_raw_data.UseVisualStyleBackColor = true;
            // 
            // cbb_comport
            // 
            this.cbb_comport.FormattingEnabled = true;
            this.cbb_comport.Location = new System.Drawing.Point(457, 3);
            this.cbb_comport.Name = "cbb_comport";
            this.cbb_comport.Size = new System.Drawing.Size(60, 21);
            this.cbb_comport.TabIndex = 16;
            // 
            // btn_refresh
            // 
            this.btn_refresh.Location = new System.Drawing.Point(399, 2);
            this.btn_refresh.Name = "btn_refresh";
            this.btn_refresh.Size = new System.Drawing.Size(52, 23);
            this.btn_refresh.TabIndex = 17;
            this.btn_refresh.Text = "Refresh";
            this.btn_refresh.UseVisualStyleBackColor = true;
            this.btn_refresh.Click += new System.EventHandler(this.btn_refresh_Click);
            // 
            // cbb_baud_rate
            // 
            this.cbb_baud_rate.FormattingEnabled = true;
            this.cbb_baud_rate.Location = new System.Drawing.Point(588, 2);
            this.cbb_baud_rate.Name = "cbb_baud_rate";
            this.cbb_baud_rate.Size = new System.Drawing.Size(59, 21);
            this.cbb_baud_rate.TabIndex = 18;
            this.cbb_baud_rate.SelectedIndexChanged += new System.EventHandler(this.cbb_baud_rate_SelectedIndexChanged);
            // 
            // lbl_baud_rate
            // 
            this.lbl_baud_rate.AutoSize = true;
            this.lbl_baud_rate.Location = new System.Drawing.Point(524, 6);
            this.lbl_baud_rate.Name = "lbl_baud_rate";
            this.lbl_baud_rate.Size = new System.Drawing.Size(58, 13);
            this.lbl_baud_rate.TabIndex = 19;
            this.lbl_baud_rate.Text = "Baud Rate";
            // 
            // btn_stop
            // 
            this.btn_stop.Location = new System.Drawing.Point(216, 1);
            this.btn_stop.Name = "btn_stop";
            this.btn_stop.Size = new System.Drawing.Size(75, 23);
            this.btn_stop.TabIndex = 20;
            this.btn_stop.Text = "Stop";
            this.btn_stop.UseVisualStyleBackColor = true;
            this.btn_stop.Click += new System.EventHandler(this.btn_stop_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1077, 544);
            this.Controls.Add(this.btn_stop);
            this.Controls.Add(this.lbl_baud_rate);
            this.Controls.Add(this.cbb_baud_rate);
            this.Controls.Add(this.btn_refresh);
            this.Controls.Add(this.cbb_comport);
            this.Controls.Add(this.chk_raw_data);
            this.Controls.Add(this.btn_raw);
            this.Controls.Add(this.lbl_send);
            this.Controls.Add(this.btn_rst_buf);
            this.Controls.Add(this.tbx_send);
            this.Controls.Add(this.btn_start);
            this.Controls.Add(this.pnl_chart);
            this.Controls.Add(this.menuStrip1);
            this.Controls.Add(this.rtb_data);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "Form1";
            this.Text = "Form1";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            ((System.ComponentModel.ISupportInitialize)(this.chart1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.tkb_scale)).EndInit();
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.pnl_chart.ResumeLayout(false);
            this.pnl_chart.PerformLayout();
            this.grp_axis.ResumeLayout(false);
            this.grp_axis.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.tkb_length_offset)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.DataVisualization.Charting.Chart chart1;
        private System.Windows.Forms.Button btn_start;
        private System.Windows.Forms.RichTextBox rtb_data;
        private System.Windows.Forms.CheckBox chkFullScale;
        private System.Windows.Forms.TrackBar tkb_scale;
        private System.Windows.Forms.TextBox tbx_send;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem;
        private System.Windows.Forms.Panel pnl_chart;
        private System.Windows.Forms.Button btn_raw;
        private System.Windows.Forms.Label lbl_send;
        private System.Windows.Forms.ToolStripMenuItem printToolStripMenuItem;
        private System.Windows.Forms.Button btn_rst_buf;
        private System.ComponentModel.BackgroundWorker backgroundWorker1;
        private System.Windows.Forms.CheckBox chk_raw_data;
        private System.Windows.Forms.TrackBar tkb_length_offset;
        private System.Windows.Forms.ComboBox cbb_comport;
        private System.Windows.Forms.Button btn_refresh;
        private System.Windows.Forms.GroupBox grp_axis;
        private System.Windows.Forms.CheckBox chk_v_len;
        private System.Windows.Forms.CheckBox chk_z;
        private System.Windows.Forms.CheckBox chk_y;
        private System.Windows.Forms.CheckBox chk_x;
        private System.Windows.Forms.ComboBox cbb_baud_rate;
        private System.Windows.Forms.Label lbl_baud_rate;
        private System.Windows.Forms.Button btn_stop;
        private System.Windows.Forms.ToolStripMenuItem saveAsToolStripMenuItem;
    }
}

