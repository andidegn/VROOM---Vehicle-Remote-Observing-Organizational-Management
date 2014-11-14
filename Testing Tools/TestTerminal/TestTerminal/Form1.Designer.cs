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
            this.components = new System.ComponentModel.Container();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea1 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend1 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series1 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.pnl_main = new System.Windows.Forms.Panel();
            this.wb_maps = new System.Windows.Forms.WebBrowser();
            this.chart_signal = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.cms_signal_chart = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.dockUndockToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.resetToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.pnl_ctrls = new System.Windows.Forms.Panel();
            this.gb_connection = new System.Windows.Forms.GroupBox();
            this.lbl_operator_name = new System.Windows.Forms.Label();
            this.chk_graph = new System.Windows.Forms.CheckBox();
            this.chk_module_state = new System.Windows.Forms.CheckBox();
            this.prog_signal = new System.Windows.Forms.ProgressBar();
            this.chk_signal = new System.Windows.Forms.CheckBox();
            this.btn_network_reg_status = new System.Windows.Forms.Button();
            this.lbl_signal_strength = new System.Windows.Forms.Label();
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
            this.gb_text_sms = new System.Windows.Forms.GroupBox();
            this.btn_send_msg = new System.Windows.Forms.Button();
            this.btn_read_msg = new System.Windows.Forms.Button();
            this.nud_msg = new System.Windows.Forms.NumericUpDown();
            this.btn_delete_msg = new System.Windows.Forms.Button();
            this.gb_gps = new System.Windows.Forms.GroupBox();
            this.chk_map = new System.Windows.Forms.CheckBox();
            this.btn_map = new System.Windows.Forms.Button();
            this.tbx_lat = new System.Windows.Forms.TextBox();
            this.lbl_lat = new System.Windows.Forms.Label();
            this.tbx_long = new System.Windows.Forms.TextBox();
            this.lbl_long = new System.Windows.Forms.Label();
            this.btn_gps_get_location = new System.Windows.Forms.Button();
            this.btn_gps_status = new System.Windows.Forms.Button();
            this.btn_gps_hot_rst = new System.Windows.Forms.Button();
            this.btn_gps_pwr = new System.Windows.Forms.Button();
            this.btn_exit = new System.Windows.Forms.Button();
            this.btn_clear = new System.Windows.Forms.Button();
            this.btn_error_report = new System.Windows.Forms.Button();
            this.btn_ctrl_z = new System.Windows.Forms.Button();
            this.tbx_send = new System.Windows.Forms.TextBox();
            this.rtb_terminal = new System.Windows.Forms.RichTextBox();
            this.cms_rtb_terminal = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.saveAsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.colorToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.timestampToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.textToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.backgroundToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.fontToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.btn_refresh = new System.Windows.Forms.Button();
            this.cbb_baud_rate = new System.Windows.Forms.ComboBox();
            this.lbl_baud = new System.Windows.Forms.Label();
            this.btn_connect = new System.Windows.Forms.Button();
            this.cbb_com_port = new System.Windows.Forms.ComboBox();
            this.lbl_com_port = new System.Windows.Forms.Label();
            this.backgroundWorker1 = new System.ComponentModel.BackgroundWorker();
            this.pnl_main.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.chart_signal)).BeginInit();
            this.cms_signal_chart.SuspendLayout();
            this.pnl_ctrls.SuspendLayout();
            this.gb_connection.SuspendLayout();
            this.gb_call.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nud_ring_tone)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nud_speaker_volume)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nud_ringer_volume)).BeginInit();
            this.gb_text_sms.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nud_msg)).BeginInit();
            this.gb_gps.SuspendLayout();
            this.cms_rtb_terminal.SuspendLayout();
            this.SuspendLayout();
            // 
            // pnl_main
            // 
            this.pnl_main.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.pnl_main.Controls.Add(this.wb_maps);
            this.pnl_main.Controls.Add(this.chart_signal);
            this.pnl_main.Controls.Add(this.pnl_ctrls);
            this.pnl_main.Controls.Add(this.btn_exit);
            this.pnl_main.Controls.Add(this.btn_clear);
            this.pnl_main.Controls.Add(this.btn_error_report);
            this.pnl_main.Controls.Add(this.btn_ctrl_z);
            this.pnl_main.Controls.Add(this.tbx_send);
            this.pnl_main.Controls.Add(this.rtb_terminal);
            this.pnl_main.Controls.Add(this.btn_refresh);
            this.pnl_main.Controls.Add(this.cbb_baud_rate);
            this.pnl_main.Controls.Add(this.lbl_baud);
            this.pnl_main.Controls.Add(this.btn_connect);
            this.pnl_main.Controls.Add(this.cbb_com_port);
            this.pnl_main.Controls.Add(this.lbl_com_port);
            this.pnl_main.Location = new System.Drawing.Point(12, 12);
            this.pnl_main.Name = "pnl_main";
            this.pnl_main.Size = new System.Drawing.Size(760, 538);
            this.pnl_main.TabIndex = 0;
            // 
            // wb_maps
            // 
            this.wb_maps.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.wb_maps.CausesValidation = false;
            this.wb_maps.Location = new System.Drawing.Point(7, 31);
            this.wb_maps.MinimumSize = new System.Drawing.Size(20, 20);
            this.wb_maps.Name = "wb_maps";
            this.wb_maps.ScriptErrorsSuppressed = true;
            this.wb_maps.ScrollBarsEnabled = false;
            this.wb_maps.Size = new System.Drawing.Size(497, 476);
            this.wb_maps.TabIndex = 24;
            this.wb_maps.Visible = false;
            // 
            // chart_signal
            // 
            this.chart_signal.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.chart_signal.BackColor = System.Drawing.Color.WhiteSmoke;
            this.chart_signal.BorderlineColor = System.Drawing.Color.Black;
            this.chart_signal.BorderlineDashStyle = System.Windows.Forms.DataVisualization.Charting.ChartDashStyle.Solid;
            chartArea1.Name = "ChartArea1";
            this.chart_signal.ChartAreas.Add(chartArea1);
            this.chart_signal.ContextMenuStrip = this.cms_signal_chart;
            legend1.Name = "Legend1";
            this.chart_signal.Legends.Add(legend1);
            this.chart_signal.Location = new System.Drawing.Point(7, 31);
            this.chart_signal.Name = "chart_signal";
            series1.ChartArea = "ChartArea1";
            series1.Legend = "Legend1";
            series1.Name = "Series1";
            this.chart_signal.Series.Add(series1);
            this.chart_signal.Size = new System.Drawing.Size(499, 477);
            this.chart_signal.TabIndex = 22;
            this.chart_signal.Text = "chart_signal";
            this.chart_signal.Visible = false;
            this.chart_signal.DoubleClick += new System.EventHandler(this.chart_signal_DoubleClick);
            this.chart_signal.MouseEnter += new System.EventHandler(this.chart_signal_MouseEnter);
            // 
            // cms_signal_chart
            // 
            this.cms_signal_chart.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.dockUndockToolStripMenuItem,
            this.resetToolStripMenuItem});
            this.cms_signal_chart.Name = "cms_signal_chart";
            this.cms_signal_chart.Size = new System.Drawing.Size(148, 48);
            // 
            // dockUndockToolStripMenuItem
            // 
            this.dockUndockToolStripMenuItem.Name = "dockUndockToolStripMenuItem";
            this.dockUndockToolStripMenuItem.Size = new System.Drawing.Size(147, 22);
            this.dockUndockToolStripMenuItem.Text = "Dock/Undock";
            this.dockUndockToolStripMenuItem.Click += new System.EventHandler(this.dockUndockToolStripMenuItem_Click);
            // 
            // resetToolStripMenuItem
            // 
            this.resetToolStripMenuItem.Name = "resetToolStripMenuItem";
            this.resetToolStripMenuItem.Size = new System.Drawing.Size(147, 22);
            this.resetToolStripMenuItem.Text = "Reset";
            this.resetToolStripMenuItem.Click += new System.EventHandler(this.resetToolStripMenuItem_Click);
            // 
            // pnl_ctrls
            // 
            this.pnl_ctrls.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.pnl_ctrls.Controls.Add(this.gb_connection);
            this.pnl_ctrls.Controls.Add(this.gb_call);
            this.pnl_ctrls.Controls.Add(this.gb_text_sms);
            this.pnl_ctrls.Controls.Add(this.gb_gps);
            this.pnl_ctrls.Location = new System.Drawing.Point(505, 0);
            this.pnl_ctrls.Name = "pnl_ctrls";
            this.pnl_ctrls.Size = new System.Drawing.Size(255, 509);
            this.pnl_ctrls.TabIndex = 23;
            // 
            // gb_connection
            // 
            this.gb_connection.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.gb_connection.Controls.Add(this.lbl_operator_name);
            this.gb_connection.Controls.Add(this.chk_graph);
            this.gb_connection.Controls.Add(this.chk_module_state);
            this.gb_connection.Controls.Add(this.prog_signal);
            this.gb_connection.Controls.Add(this.chk_signal);
            this.gb_connection.Controls.Add(this.btn_network_reg_status);
            this.gb_connection.Controls.Add(this.lbl_signal_strength);
            this.gb_connection.Location = new System.Drawing.Point(3, 8);
            this.gb_connection.Name = "gb_connection";
            this.gb_connection.Size = new System.Drawing.Size(249, 93);
            this.gb_connection.TabIndex = 18;
            this.gb_connection.TabStop = false;
            this.gb_connection.Text = "Connection";
            // 
            // lbl_operator_name
            // 
            this.lbl_operator_name.AutoSize = true;
            this.lbl_operator_name.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbl_operator_name.Location = new System.Drawing.Point(90, 64);
            this.lbl_operator_name.Name = "lbl_operator_name";
            this.lbl_operator_name.Size = new System.Drawing.Size(0, 20);
            this.lbl_operator_name.TabIndex = 25;
            // 
            // chk_graph
            // 
            this.chk_graph.AutoSize = true;
            this.chk_graph.Location = new System.Drawing.Point(191, 33);
            this.chk_graph.Name = "chk_graph";
            this.chk_graph.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.chk_graph.Size = new System.Drawing.Size(55, 17);
            this.chk_graph.TabIndex = 24;
            this.chk_graph.Text = "Graph";
            this.chk_graph.UseVisualStyleBackColor = true;
            this.chk_graph.CheckedChanged += new System.EventHandler(this.chk_graph_CheckedChanged);
            // 
            // chk_module_state
            // 
            this.chk_module_state.Appearance = System.Windows.Forms.Appearance.Button;
            this.chk_module_state.Checked = true;
            this.chk_module_state.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chk_module_state.Location = new System.Drawing.Point(6, 30);
            this.chk_module_state.Name = "chk_module_state";
            this.chk_module_state.Size = new System.Drawing.Size(75, 23);
            this.chk_module_state.TabIndex = 23;
            this.chk_module_state.Text = "On";
            this.chk_module_state.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.chk_module_state.UseVisualStyleBackColor = true;
            this.chk_module_state.CheckedChanged += new System.EventHandler(this.chk_module_state_CheckedChanged);
            // 
            // prog_signal
            // 
            this.prog_signal.Location = new System.Drawing.Point(6, 16);
            this.prog_signal.Maximum = 310;
            this.prog_signal.Name = "prog_signal";
            this.prog_signal.Size = new System.Drawing.Size(237, 10);
            this.prog_signal.Step = 1;
            this.prog_signal.Style = System.Windows.Forms.ProgressBarStyle.Continuous;
            this.prog_signal.TabIndex = 22;
            // 
            // chk_signal
            // 
            this.chk_signal.AutoSize = true;
            this.chk_signal.CheckAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.chk_signal.Location = new System.Drawing.Point(87, 33);
            this.chk_signal.Name = "chk_signal";
            this.chk_signal.Size = new System.Drawing.Size(55, 17);
            this.chk_signal.TabIndex = 18;
            this.chk_signal.Text = "Signal";
            this.chk_signal.UseVisualStyleBackColor = true;
            this.chk_signal.CheckedChanged += new System.EventHandler(this.chk_signal_CheckedChanged);
            // 
            // btn_network_reg_status
            // 
            this.btn_network_reg_status.Location = new System.Drawing.Point(6, 59);
            this.btn_network_reg_status.Name = "btn_network_reg_status";
            this.btn_network_reg_status.Size = new System.Drawing.Size(75, 23);
            this.btn_network_reg_status.TabIndex = 17;
            this.btn_network_reg_status.Text = "Status";
            this.btn_network_reg_status.UseVisualStyleBackColor = true;
            this.btn_network_reg_status.Click += new System.EventHandler(this.btn_network_reg_status_Click);
            // 
            // lbl_signal_strength
            // 
            this.lbl_signal_strength.AutoSize = true;
            this.lbl_signal_strength.Location = new System.Drawing.Point(143, 35);
            this.lbl_signal_strength.Name = "lbl_signal_strength";
            this.lbl_signal_strength.Size = new System.Drawing.Size(16, 13);
            this.lbl_signal_strength.TabIndex = 16;
            this.lbl_signal_strength.Text = "---";
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
            this.gb_call.Location = new System.Drawing.Point(3, 250);
            this.gb_call.Name = "gb_call";
            this.gb_call.Size = new System.Drawing.Size(249, 101);
            this.gb_call.TabIndex = 13;
            this.gb_call.TabStop = false;
            this.gb_call.Text = "Call";
            this.gb_call.Visible = false;
            // 
            // btn_set_ring_tone
            // 
            this.btn_set_ring_tone.Location = new System.Drawing.Point(205, 72);
            this.btn_set_ring_tone.Name = "btn_set_ring_tone";
            this.btn_set_ring_tone.Size = new System.Drawing.Size(38, 23);
            this.btn_set_ring_tone.TabIndex = 17;
            this.btn_set_ring_tone.Text = "Set";
            this.btn_set_ring_tone.UseVisualStyleBackColor = true;
            this.btn_set_ring_tone.Click += new System.EventHandler(this.btn_set_ring_tone_Click);
            // 
            // nud_ring_tone
            // 
            this.nud_ring_tone.Location = new System.Drawing.Point(163, 75);
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
            this.lbl_ring_tone.Location = new System.Drawing.Point(95, 77);
            this.lbl_ring_tone.Name = "lbl_ring_tone";
            this.lbl_ring_tone.Size = new System.Drawing.Size(57, 13);
            this.lbl_ring_tone.TabIndex = 15;
            this.lbl_ring_tone.Text = "Ring Tone";
            // 
            // btn_set_speaker_volume
            // 
            this.btn_set_speaker_volume.Location = new System.Drawing.Point(205, 46);
            this.btn_set_speaker_volume.Name = "btn_set_speaker_volume";
            this.btn_set_speaker_volume.Size = new System.Drawing.Size(38, 23);
            this.btn_set_speaker_volume.TabIndex = 14;
            this.btn_set_speaker_volume.Text = "Set";
            this.btn_set_speaker_volume.UseVisualStyleBackColor = true;
            this.btn_set_speaker_volume.Click += new System.EventHandler(this.btn_set_speaker_volume_Click);
            // 
            // btn_set_ringer_volume
            // 
            this.btn_set_ringer_volume.Location = new System.Drawing.Point(205, 20);
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
            this.lbl_speaker_vol.Location = new System.Drawing.Point(95, 51);
            this.lbl_speaker_vol.Name = "lbl_speaker_vol";
            this.lbl_speaker_vol.Size = new System.Drawing.Size(68, 13);
            this.lbl_speaker_vol.TabIndex = 12;
            this.lbl_speaker_vol.Text = "Speaker Vol.";
            // 
            // lbl_ringer_volume
            // 
            this.lbl_ringer_volume.AutoSize = true;
            this.lbl_ringer_volume.Location = new System.Drawing.Point(95, 26);
            this.lbl_ringer_volume.Name = "lbl_ringer_volume";
            this.lbl_ringer_volume.Size = new System.Drawing.Size(59, 13);
            this.lbl_ringer_volume.TabIndex = 11;
            this.lbl_ringer_volume.Text = "Ringer Vol.";
            // 
            // nud_speaker_volume
            // 
            this.nud_speaker_volume.Location = new System.Drawing.Point(163, 49);
            this.nud_speaker_volume.Name = "nud_speaker_volume";
            this.nud_speaker_volume.Size = new System.Drawing.Size(36, 20);
            this.nud_speaker_volume.TabIndex = 10;
            this.nud_speaker_volume.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.nud_speaker_volume_MouseDoubleClick);
            // 
            // nud_ringer_volume
            // 
            this.nud_ringer_volume.Location = new System.Drawing.Point(163, 23);
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
            this.btn_answer.Location = new System.Drawing.Point(6, 20);
            this.btn_answer.Name = "btn_answer";
            this.btn_answer.Size = new System.Drawing.Size(75, 75);
            this.btn_answer.TabIndex = 8;
            this.btn_answer.Text = "Answer";
            this.btn_answer.UseVisualStyleBackColor = true;
            this.btn_answer.Click += new System.EventHandler(this.btn_answer_Click);
            // 
            // gb_text_sms
            // 
            this.gb_text_sms.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.gb_text_sms.Controls.Add(this.btn_send_msg);
            this.gb_text_sms.Controls.Add(this.btn_read_msg);
            this.gb_text_sms.Controls.Add(this.nud_msg);
            this.gb_text_sms.Controls.Add(this.btn_delete_msg);
            this.gb_text_sms.Location = new System.Drawing.Point(3, 357);
            this.gb_text_sms.Name = "gb_text_sms";
            this.gb_text_sms.Size = new System.Drawing.Size(249, 47);
            this.gb_text_sms.TabIndex = 14;
            this.gb_text_sms.TabStop = false;
            this.gb_text_sms.Text = "Text (SMS)";
            this.gb_text_sms.Visible = false;
            // 
            // btn_send_msg
            // 
            this.btn_send_msg.Location = new System.Drawing.Point(6, 47);
            this.btn_send_msg.Name = "btn_send_msg";
            this.btn_send_msg.Size = new System.Drawing.Size(75, 23);
            this.btn_send_msg.TabIndex = 13;
            this.btn_send_msg.Text = "Send MSG";
            this.btn_send_msg.UseVisualStyleBackColor = true;
            this.btn_send_msg.Click += new System.EventHandler(this.btn_send_msg_Click);
            // 
            // btn_read_msg
            // 
            this.btn_read_msg.Location = new System.Drawing.Point(6, 18);
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
            // btn_delete_msg
            // 
            this.btn_delete_msg.Location = new System.Drawing.Point(168, 18);
            this.btn_delete_msg.Name = "btn_delete_msg";
            this.btn_delete_msg.Size = new System.Drawing.Size(75, 23);
            this.btn_delete_msg.TabIndex = 12;
            this.btn_delete_msg.Text = "Delete";
            this.btn_delete_msg.UseVisualStyleBackColor = true;
            this.btn_delete_msg.Click += new System.EventHandler(this.btn_delete_msg_Click);
            // 
            // gb_gps
            // 
            this.gb_gps.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.gb_gps.Controls.Add(this.chk_map);
            this.gb_gps.Controls.Add(this.btn_map);
            this.gb_gps.Controls.Add(this.tbx_lat);
            this.gb_gps.Controls.Add(this.lbl_lat);
            this.gb_gps.Controls.Add(this.tbx_long);
            this.gb_gps.Controls.Add(this.lbl_long);
            this.gb_gps.Controls.Add(this.btn_gps_get_location);
            this.gb_gps.Controls.Add(this.btn_gps_status);
            this.gb_gps.Controls.Add(this.btn_gps_hot_rst);
            this.gb_gps.Controls.Add(this.btn_gps_pwr);
            this.gb_gps.Location = new System.Drawing.Point(3, 107);
            this.gb_gps.Name = "gb_gps";
            this.gb_gps.Size = new System.Drawing.Size(249, 137);
            this.gb_gps.TabIndex = 20;
            this.gb_gps.TabStop = false;
            this.gb_gps.Text = "GPS";
            // 
            // chk_map
            // 
            this.chk_map.AutoSize = true;
            this.chk_map.Location = new System.Drawing.Point(199, 53);
            this.chk_map.Name = "chk_map";
            this.chk_map.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.chk_map.Size = new System.Drawing.Size(47, 17);
            this.chk_map.TabIndex = 9;
            this.chk_map.Text = "Map";
            this.chk_map.UseVisualStyleBackColor = true;
            this.chk_map.CheckedChanged += new System.EventHandler(this.chk_map_CheckedChanged);
            // 
            // btn_map
            // 
            this.btn_map.Location = new System.Drawing.Point(87, 50);
            this.btn_map.Name = "btn_map";
            this.btn_map.Size = new System.Drawing.Size(75, 23);
            this.btn_map.TabIndex = 8;
            this.btn_map.Text = "Update Map";
            this.btn_map.UseVisualStyleBackColor = true;
            this.btn_map.Click += new System.EventHandler(this.btn_map_Click);
            // 
            // tbx_lat
            // 
            this.tbx_lat.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.tbx_lat.Location = new System.Drawing.Point(87, 105);
            this.tbx_lat.Name = "tbx_lat";
            this.tbx_lat.ReadOnly = true;
            this.tbx_lat.Size = new System.Drawing.Size(156, 20);
            this.tbx_lat.TabIndex = 7;
            // 
            // lbl_lat
            // 
            this.lbl_lat.AutoSize = true;
            this.lbl_lat.Location = new System.Drawing.Point(6, 108);
            this.lbl_lat.Name = "lbl_lat";
            this.lbl_lat.Size = new System.Drawing.Size(45, 13);
            this.lbl_lat.TabIndex = 6;
            this.lbl_lat.Text = "Latitude";
            // 
            // tbx_long
            // 
            this.tbx_long.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.tbx_long.Location = new System.Drawing.Point(87, 79);
            this.tbx_long.Name = "tbx_long";
            this.tbx_long.ReadOnly = true;
            this.tbx_long.Size = new System.Drawing.Size(156, 20);
            this.tbx_long.TabIndex = 5;
            // 
            // lbl_long
            // 
            this.lbl_long.AutoSize = true;
            this.lbl_long.Location = new System.Drawing.Point(6, 82);
            this.lbl_long.Name = "lbl_long";
            this.lbl_long.Size = new System.Drawing.Size(54, 13);
            this.lbl_long.TabIndex = 4;
            this.lbl_long.Text = "Longitude";
            // 
            // btn_gps_get_location
            // 
            this.btn_gps_get_location.Location = new System.Drawing.Point(6, 50);
            this.btn_gps_get_location.Name = "btn_gps_get_location";
            this.btn_gps_get_location.Size = new System.Drawing.Size(75, 23);
            this.btn_gps_get_location.TabIndex = 3;
            this.btn_gps_get_location.Text = "Location Fix";
            this.btn_gps_get_location.UseVisualStyleBackColor = true;
            this.btn_gps_get_location.Click += new System.EventHandler(this.btn_gps_get_location_Click);
            // 
            // btn_gps_status
            // 
            this.btn_gps_status.Location = new System.Drawing.Point(87, 19);
            this.btn_gps_status.Name = "btn_gps_status";
            this.btn_gps_status.Size = new System.Drawing.Size(75, 23);
            this.btn_gps_status.TabIndex = 2;
            this.btn_gps_status.Text = "Status";
            this.btn_gps_status.UseVisualStyleBackColor = true;
            this.btn_gps_status.Click += new System.EventHandler(this.btn_gps_status_Click);
            // 
            // btn_gps_hot_rst
            // 
            this.btn_gps_hot_rst.Location = new System.Drawing.Point(168, 19);
            this.btn_gps_hot_rst.Name = "btn_gps_hot_rst";
            this.btn_gps_hot_rst.Size = new System.Drawing.Size(75, 23);
            this.btn_gps_hot_rst.TabIndex = 1;
            this.btn_gps_hot_rst.Text = "Hot Reset";
            this.btn_gps_hot_rst.UseVisualStyleBackColor = true;
            this.btn_gps_hot_rst.Click += new System.EventHandler(this.btn_gps_hot_rst_Click);
            // 
            // btn_gps_pwr
            // 
            this.btn_gps_pwr.BackColor = System.Drawing.Color.Red;
            this.btn_gps_pwr.Location = new System.Drawing.Point(6, 19);
            this.btn_gps_pwr.Name = "btn_gps_pwr";
            this.btn_gps_pwr.Size = new System.Drawing.Size(75, 23);
            this.btn_gps_pwr.TabIndex = 0;
            this.btn_gps_pwr.Text = "Off";
            this.btn_gps_pwr.UseVisualStyleBackColor = false;
            this.btn_gps_pwr.Click += new System.EventHandler(this.btn_gps_pwr_Click);
            // 
            // btn_exit
            // 
            this.btn_exit.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btn_exit.Location = new System.Drawing.Point(676, 512);
            this.btn_exit.Name = "btn_exit";
            this.btn_exit.Size = new System.Drawing.Size(75, 23);
            this.btn_exit.TabIndex = 21;
            this.btn_exit.Text = "Exit";
            this.btn_exit.UseVisualStyleBackColor = true;
            this.btn_exit.Click += new System.EventHandler(this.btn_exit_Click);
            // 
            // btn_clear
            // 
            this.btn_clear.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btn_clear.Location = new System.Drawing.Point(433, 512);
            this.btn_clear.Name = "btn_clear";
            this.btn_clear.Size = new System.Drawing.Size(75, 23);
            this.btn_clear.TabIndex = 19;
            this.btn_clear.Text = "Clear";
            this.btn_clear.UseVisualStyleBackColor = true;
            this.btn_clear.Click += new System.EventHandler(this.btn_clear_Click);
            // 
            // btn_error_report
            // 
            this.btn_error_report.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btn_error_report.Location = new System.Drawing.Point(595, 512);
            this.btn_error_report.Name = "btn_error_report";
            this.btn_error_report.Size = new System.Drawing.Size(75, 23);
            this.btn_error_report.TabIndex = 17;
            this.btn_error_report.Text = "Error Report";
            this.btn_error_report.UseVisualStyleBackColor = true;
            this.btn_error_report.Click += new System.EventHandler(this.btn_error_report_Click);
            // 
            // btn_ctrl_z
            // 
            this.btn_ctrl_z.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btn_ctrl_z.Location = new System.Drawing.Point(514, 512);
            this.btn_ctrl_z.Name = "btn_ctrl_z";
            this.btn_ctrl_z.Size = new System.Drawing.Size(75, 23);
            this.btn_ctrl_z.TabIndex = 9;
            this.btn_ctrl_z.Text = "Ctrl+Z";
            this.btn_ctrl_z.UseVisualStyleBackColor = true;
            this.btn_ctrl_z.Click += new System.EventHandler(this.btn_ctrl_z_Click);
            // 
            // tbx_send
            // 
            this.tbx_send.AllowDrop = true;
            this.tbx_send.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tbx_send.Location = new System.Drawing.Point(6, 514);
            this.tbx_send.Name = "tbx_send";
            this.tbx_send.Size = new System.Drawing.Size(421, 20);
            this.tbx_send.TabIndex = 3;
            this.tbx_send.KeyDown += new System.Windows.Forms.KeyEventHandler(this.tbx_send_KeyDown);
            // 
            // rtb_terminal
            // 
            this.rtb_terminal.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.rtb_terminal.ContextMenuStrip = this.cms_rtb_terminal;
            this.rtb_terminal.Location = new System.Drawing.Point(6, 30);
            this.rtb_terminal.Name = "rtb_terminal";
            this.rtb_terminal.Size = new System.Drawing.Size(500, 479);
            this.rtb_terminal.TabIndex = 2;
            this.rtb_terminal.Text = "";
            // 
            // cms_rtb_terminal
            // 
            this.cms_rtb_terminal.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.saveAsToolStripMenuItem,
            this.colorToolStripMenuItem,
            this.fontToolStripMenuItem});
            this.cms_rtb_terminal.Name = "cms_rtb_terminal";
            this.cms_rtb_terminal.Size = new System.Drawing.Size(124, 70);
            // 
            // saveAsToolStripMenuItem
            // 
            this.saveAsToolStripMenuItem.Name = "saveAsToolStripMenuItem";
            this.saveAsToolStripMenuItem.Size = new System.Drawing.Size(123, 22);
            this.saveAsToolStripMenuItem.Text = "Save As...";
            this.saveAsToolStripMenuItem.Click += new System.EventHandler(this.saveAsToolStripMenuItem_Click);
            // 
            // colorToolStripMenuItem
            // 
            this.colorToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.timestampToolStripMenuItem,
            this.textToolStripMenuItem,
            this.backgroundToolStripMenuItem});
            this.colorToolStripMenuItem.Name = "colorToolStripMenuItem";
            this.colorToolStripMenuItem.Size = new System.Drawing.Size(123, 22);
            this.colorToolStripMenuItem.Text = "Color";
            // 
            // timestampToolStripMenuItem
            // 
            this.timestampToolStripMenuItem.Name = "timestampToolStripMenuItem";
            this.timestampToolStripMenuItem.Size = new System.Drawing.Size(138, 22);
            this.timestampToolStripMenuItem.Text = "Timestamp";
            this.timestampToolStripMenuItem.Click += new System.EventHandler(this.timestampToolStripMenuItem_Click);
            // 
            // textToolStripMenuItem
            // 
            this.textToolStripMenuItem.Name = "textToolStripMenuItem";
            this.textToolStripMenuItem.Size = new System.Drawing.Size(138, 22);
            this.textToolStripMenuItem.Text = "Text";
            this.textToolStripMenuItem.Click += new System.EventHandler(this.textToolStripMenuItem_Click);
            // 
            // backgroundToolStripMenuItem
            // 
            this.backgroundToolStripMenuItem.Name = "backgroundToolStripMenuItem";
            this.backgroundToolStripMenuItem.Size = new System.Drawing.Size(138, 22);
            this.backgroundToolStripMenuItem.Text = "Background";
            this.backgroundToolStripMenuItem.Click += new System.EventHandler(this.backgroundToolStripMenuItem_Click);
            // 
            // fontToolStripMenuItem
            // 
            this.fontToolStripMenuItem.Name = "fontToolStripMenuItem";
            this.fontToolStripMenuItem.Size = new System.Drawing.Size(123, 22);
            this.fontToolStripMenuItem.Text = "Font";
            this.fontToolStripMenuItem.Click += new System.EventHandler(this.fontToolStripMenuItem_Click);
            // 
            // btn_refresh
            // 
            this.btn_refresh.Location = new System.Drawing.Point(104, 1);
            this.btn_refresh.Name = "btn_refresh";
            this.btn_refresh.Size = new System.Drawing.Size(58, 23);
            this.btn_refresh.TabIndex = 7;
            this.btn_refresh.Text = "Refresh";
            this.btn_refresh.UseVisualStyleBackColor = true;
            this.btn_refresh.Click += new System.EventHandler(this.btn_refresh_Click);
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
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(784, 562);
            this.Controls.Add(this.pnl_main);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MinimumSize = new System.Drawing.Size(700, 390);
            this.Name = "Form1";
            this.Text = "AT Terminal";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.KeyUp += new System.Windows.Forms.KeyEventHandler(this.Form1_KeyUp);
            this.pnl_main.ResumeLayout(false);
            this.pnl_main.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.chart_signal)).EndInit();
            this.cms_signal_chart.ResumeLayout(false);
            this.pnl_ctrls.ResumeLayout(false);
            this.gb_connection.ResumeLayout(false);
            this.gb_connection.PerformLayout();
            this.gb_call.ResumeLayout(false);
            this.gb_call.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nud_ring_tone)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nud_speaker_volume)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nud_ringer_volume)).EndInit();
            this.gb_text_sms.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.nud_msg)).EndInit();
            this.gb_gps.ResumeLayout(false);
            this.gb_gps.PerformLayout();
            this.cms_rtb_terminal.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel pnl_main;
        private System.Windows.Forms.Button btn_connect;
        private System.Windows.Forms.TextBox tbx_send;
        private System.Windows.Forms.RichTextBox rtb_terminal;
        private System.Windows.Forms.ComboBox cbb_com_port;
        private System.Windows.Forms.Label lbl_com_port;
        private System.Windows.Forms.ComboBox cbb_baud_rate;
        private System.Windows.Forms.Label lbl_baud;
        private System.Windows.Forms.Button btn_refresh;
        private System.Windows.Forms.Button btn_answer;
        private System.Windows.Forms.Button btn_ctrl_z;
        private System.Windows.Forms.Button btn_read_msg;
        private System.Windows.Forms.NumericUpDown nud_msg;
        private System.Windows.Forms.Button btn_delete_msg;
        private System.Windows.Forms.GroupBox gb_text_sms;
        private System.Windows.Forms.GroupBox gb_call;
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
        private System.Windows.Forms.GroupBox gb_gps;
        private System.Windows.Forms.Button btn_gps_pwr;
        private System.Windows.Forms.Button btn_gps_hot_rst;
        private System.Windows.Forms.Button btn_gps_get_location;
        private System.Windows.Forms.Button btn_gps_status;
        private System.Windows.Forms.Button btn_exit;
        private System.Windows.Forms.CheckBox chk_signal;
        private System.Windows.Forms.ProgressBar prog_signal;
        private System.Windows.Forms.CheckBox chk_module_state;
        private System.Windows.Forms.DataVisualization.Charting.Chart chart_signal;
        private System.Windows.Forms.CheckBox chk_graph;
        private System.Windows.Forms.Panel pnl_ctrls;
        private System.Windows.Forms.ContextMenuStrip cms_rtb_terminal;
        private System.Windows.Forms.ToolStripMenuItem saveAsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem colorToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem timestampToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem textToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem backgroundToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem fontToolStripMenuItem;
        private System.Windows.Forms.ContextMenuStrip cms_signal_chart;
        private System.Windows.Forms.ToolStripMenuItem dockUndockToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem resetToolStripMenuItem;
        private System.Windows.Forms.Label lbl_operator_name;
        private System.Windows.Forms.TextBox tbx_lat;
        private System.Windows.Forms.Label lbl_lat;
        private System.Windows.Forms.TextBox tbx_long;
        private System.Windows.Forms.Label lbl_long;
        private System.Windows.Forms.Button btn_map;
        private System.Windows.Forms.WebBrowser wb_maps;
        private System.Windows.Forms.CheckBox chk_map;
        private System.ComponentModel.BackgroundWorker backgroundWorker1;

    }
}

