using AMD.Util;
using AMD.Util.Serial.BaudRates;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using TestTerminal.Properties;
using AMD.Util.terminal;
using System.Threading;
using System.Windows.Forms.DataVisualization.Charting;
using Accelerometer_Analyzer.AMD.util;
using AMD.Util.Extensions;
using System.IO;
using System.Globalization;

namespace TestTerminal {
    public partial class Form1 : Form {
        private SerialPort _port;
        private bool _port_open = false;
        private BaudRate _baud_rate;
        private bool _wait_for_signal;
        private bool _checking_signal;
        private int _signal_ctr;

        private Color _color_timestamp;
        private Color _color_text;
        private Color _color_background;

        private const String CHART_SERIES = "series";
        private const int SIGNAL_INTERVAL = 2500;
        private bool _chart_docked;
        private int _zoom_value = 20;
        System.Windows.Forms.Timer _signal_timer = new System.Windows.Forms.Timer();
        private int _skip_no_of_receptions;
        private bool _map_docked;

        #region setup
        public Form1() {
            InitializeComponent();
            _init_rest();
            _init_chart();
            _init_signal_timer();
        }

        private void _init_rest() {
            _com_update();
            cbb_baud_rate.DataSource = BaudRateList.getInstance().getArray();
            tbx_send.Multiline = false;
            _settings_load();
            rtb_terminal.Font = new Font("Courier New", 11);
            rtb_terminal.BackColor = _color_background;
            this.ActiveControl = tbx_send;
        }
        private void _init_signal_timer() {
            _signal_timer.Interval = SIGNAL_INTERVAL;
            _signal_timer.Tick += (s, o) => {
                if (_checking_signal) {
                    _wait_for_signal = true;
                    this.BeginInvoke((Action)(() => { _com_write(AT_commands.AT_CONN_SIGNAL_STRENGTH); }));
                    _skip_no_of_receptions = 3;
                    Thread t = new Thread(_signal_timeout);
                    t.Start();
                } else {
                    _wait_for_signal = false;
                    this.BeginInvoke((Action)(() => { chk_signal.Checked = false; }));
                    _signal_timer.Stop();
                }
            };
        }

        private void _signal_timeout() {
            Thread.Sleep(450);
            //_wait_for_signal = false;
        }

        private void _settings_load() {
            cbb_baud_rate.SelectedIndex = Settings.Default.cbb_baud_rate;
            if (Settings.Default.cbb_com_port < cbb_com_port.Items.Count) {
                cbb_com_port.SelectedIndex = Settings.Default.cbb_com_port;
            }

            this.Size = Settings.Default.window_size;
            this.WindowState = Settings.Default.window_state;

            chk_graph.Checked = Settings.Default.chart_enabled;

            _chart_docked = Settings.Default.chart_dock_state;

            _color_timestamp = Settings.Default.color_timestamp;
            _color_text = Settings.Default.color_text;
            _color_background = Settings.Default.color_background;

            rtb_terminal.Font = Settings.Default.font_rtb;
        }

        private void _settings_save() {
            Settings.Default.cbb_baud_rate = cbb_baud_rate.SelectedIndex;
            Settings.Default.cbb_com_port = cbb_com_port.SelectedIndex;

            Settings.Default.window_state = this.WindowState != FormWindowState.Minimized ? this.WindowState : FormWindowState.Normal;
            if (this.WindowState == FormWindowState.Maximized) {
                this.WindowState = FormWindowState.Normal;
            }
            Settings.Default.window_size = this.Size;

            Settings.Default.chart_enabled = chk_graph.Checked;
            Settings.Default.chart_dock_state = !_chart_docked;

            Settings.Default.color_timestamp = _color_timestamp;
            Settings.Default.color_text = _color_text;
            Settings.Default.color_background = _color_background;
            Settings.Default.font_rtb = rtb_terminal.Font;

            Settings.Default.Save();
        }
        #endregion

        #region communication
        private void _com_update() {
            cbb_com_port.DataSource = SerialPortConnector.getAvalComPort();
        }

        private void _com_write(String s) {
            if (_port == null || !_port.IsOpen) {
                _com_connect();
            }
            try {
                _port.WriteLine(s + Environment.NewLine);
            } catch { }
        }

        private bool _com_connect() {
            try {
                if (_port_open) {
                    _port.Close();
                }
                _port = new SerialPort();
                _baud_rate = cbb_baud_rate.SelectedItem as BaudRate;
                SerialPortConnector.SerialSetup(_port, cbb_com_port.SelectedValue.ToString(), (cbb_baud_rate.SelectedValue as BaudRate));
                _port.DataReceived += new SerialDataReceivedEventHandler(uart_data_received_handler);
                _port_open = _port.IsOpen;
                cbb_com_port.Enabled = false;
                btn_connect.Text = "Disconnect";
            } catch (Exception) {
                _port_open = false;
                cbb_com_port.Enabled = true;
                btn_connect.Text = "Connect";
                _checking_signal = false;
                MessageBox.Show("Error opening port", "Connection error");
                return false;
            }
            return true;
        }
        #endregion

        #region gui
        delegate void _rtb_append_eh(String p);
        private void _rtb_append(String p) {
            if (InvokeRequired) {
                this.BeginInvoke(new _rtb_append_eh(_rtb_append), p);
                return;
            }
            if (p.Length < 1) {
                return;
            }

            if (p.Contains("COPS")) {
                if (p.Contains("\"")) {
                    int first = p.IndexOf('"');
                    int last = p.LastIndexOf('"');
                    lbl_operator_name.Text = p.Substring(first + 1, last - first - 1);
                } else if (p.EndsWith("0\r")) {
                    lbl_operator_name.Text = "No Operator";
                }
            }
            int cnt = (from c in p
                      where c == ','
                      select c).Count();

            if (cnt == 8 && p[3] != '.') {
                string[] sloc_arr = p.Split(',');

                String longitude_deg_st = sloc_arr[2].Substring(0, sloc_arr[2].IndexOf('.') - 2);
                String latitude_deg_st = sloc_arr[1].Substring(0, sloc_arr[1].IndexOf('.') - 2);

                double longitude_min_dec = double.Parse(sloc_arr[2].Substring(sloc_arr[2].IndexOf('.') - 2), CultureInfo.CreateSpecificCulture("en-US"));
                double latitude_min_dec = double.Parse(sloc_arr[1].Substring(sloc_arr[1].IndexOf('.') - 2), CultureInfo.CreateSpecificCulture("en-US"));
                
                int long_deg = int.Parse(longitude_deg_st);
                int lat_deg = int.Parse(latitude_deg_st);

                double long_rl = long_deg + longitude_min_dec / 60;
                double lat_rl = lat_deg + latitude_min_dec / 60;

                tbx_long.Text = long_rl.ToString();
                tbx_lat.Text = lat_rl.ToString();

                if (chk_map.Checked) {
                    _update_map();
                }
            }

            
            
            if (p.Contains("CSQ")) {
                if (p.Contains("CSQ:")) {
                    double raw_signal_value = double.Parse(p.Substring(p.IndexOf(' ') + 1, 4));
                    double.TryParse(p.Substring(p.IndexOf(' ') + 1, 4), out raw_signal_value);
                    prog_signal.Value = (int)(raw_signal_value * 10);
                    double dbm = _calc_dbm(raw_signal_value);
                    lbl_signal_strength.Text = dbm + "dbm";
                    _chart_add_point(CHART_SERIES, (_signal_ctr += SIGNAL_INTERVAL) / 1000, dbm);
                    if (raw_signal_value == 0) {
                        lbl_signal_strength.ForeColor = Color.Red;
                        prog_signal.Style = ProgressBarStyle.Marquee;
                    } else {
                        lbl_signal_strength.ForeColor = Color.Black;
                        prog_signal.Style = ProgressBarStyle.Continuous;
                    }
                    if (lbl_operator_name.Text.Length == 0) {
                        _com_write(AT_commands.AT_CONN_OPERATOR_NAME);
                    }
                }

            } else if (_skip_no_of_receptions <= 0) {
                rtb_terminal.AppendText((p != "\r" ? DateTime.Now.ToString("[dd-MM-yyyy HH:mm:ss] ") : ""), _color_timestamp);
                rtb_terminal.AppendText(p, _color_text);
                rtb_terminal.ScrollToCaret();

                if (p.Contains("RING")) {
                    btn_answer.BackColor = Color.Green;
                } else if (p.Contains("+CMTI:")) {
                    btn_read_msg.BackColor = Color.Green;
                } else if (p == "OK\r" || p == "ERROR\r") {
                    rtb_terminal.AppendText("=========================\n");
                }
            } else if (_skip_no_of_receptions > 0) {
                _skip_no_of_receptions--;
            }
        }

        private double _calc_dbm(double csq_value) {
            return -113 + (csq_value * 2);
        }

        private void _color_select(ref Color to_be_changed) {
            ColorDialog cd = new ColorDialog();
            if (cd.ShowDialog() == DialogResult.OK) {
                to_be_changed = cd.Color;
            }
        }

        private void _init_chart() {
            chart_signal.MouseWheel += chart_signal_MouseWheel;
            chart_signal.Series.Clear();
            chart_signal.ChartAreas[0].AxisX.Minimum = 0;
            chart_signal.ChartAreas[0].AxisY.Minimum = -113;
            chart_signal.ChartAreas[0].AxisY.Maximum = -30;
            chart_signal.ChartAreas[0].AxisX.LabelStyle.Enabled = false; ;
            chart_signal.ChartAreas[0].AxisX.MajorGrid.LineWidth = 0;
            chart_signal.ChartAreas[0].AxisY.MajorGrid.LineWidth = 1;
            //chart_signal.ChartAreas[0].Area3DStyle.Enable3D = true;
            //chart_signal.ChartAreas[0].Area3DStyle.Inclination = 10;
            //chart_signal.ChartAreas[0].Area3DStyle.Rotation = 10;
            ChartControl.ChartSetup(chart_signal, CHART_SERIES, 1, Color.DarkRed, SeriesChartType.Line, ChartValueType.Int32);
            chart_signal.Series[CHART_SERIES].IsVisibleInLegend = false;
            _chart_set_dock_state();
        }

        delegate void _chart_add_point_eh(String seriesName, double x, double y);
        private void _chart_add_point(String seriesName, double x, double y) {
            if (this.InvokeRequired) {
                this.BeginInvoke(new _chart_add_point_eh(_chart_add_point), seriesName, x, y);
                return;
            }
            try {
                chart_signal.Series[seriesName].Points.AddXY((double)x, (double)y);
            } catch { }
            _chart_zoom_trigger();
        }

        delegate void _chart_clear_data_eh();
        private void _chart_clear_data() {
            if (InvokeRequired) {
                this.BeginInvoke(new _chart_clear_data_eh(_chart_clear_data));
                return;
            }
            foreach (var series in chart_signal.Series) {
                series.Points.Clear();
            }
            _signal_ctr = 0;
        }

        delegate void _chart_zoom_trigger_eh();
        private void _chart_zoom_trigger() {
            if (InvokeRequired) {
                this.BeginInvoke(new _chart_zoom_trigger_eh(_chart_zoom_trigger));
                return;
            }
            chart_signal.ChartAreas[0].CursorX.AutoScroll = true;
            chart_signal.ChartAreas[0].AxisX.ScaleView.Zoomable = true;
            chart_signal.ChartAreas[0].AxisX.ScaleView.SizeType = DateTimeIntervalType.Number;
            chart_signal.ChartAreas[0].AxisX.ScaleView.Zoom(_signal_ctr / 1000 - _zoom_value, _signal_ctr / 1000);
        }

        private void _chart_set_dock_state() {
            if (_chart_docked) {
                //this.pnl_ctrls.Controls.Remove(chart_signal);
                this.pnl_main.Controls.Add(chart_signal);
                chart_signal.BringToFront();
                this.chart_signal.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                | System.Windows.Forms.AnchorStyles.Left)
                | System.Windows.Forms.AnchorStyles.Right));
                this.chart_signal.Location = new System.Drawing.Point(7, 31);
                this.chart_signal.Size = new System.Drawing.Size(rtb_terminal.Width - 3, rtb_terminal.Height - 3);
                _chart_docked = false;
            } else {
                this.pnl_ctrls.Controls.Add(chart_signal);
                this.chart_signal.Anchor = ((System.Windows.Forms.AnchorStyles)(System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right));
                this.chart_signal.Size = new System.Drawing.Size(245, 120);
                //this.chart_signal.Location = new System.Drawing.Point(this.Width - chart_signal.Width - 45, 350);
                this.chart_signal.Location = new System.Drawing.Point(5, gb_connection.Height + gb_call.Height + gb_text_sms.Height + gb_gps.Height + 25);
                _chart_docked = true;
            }
        }
        #endregion

        #region file
        private bool _save_as() {
            String path = String.Empty;
            SaveFileDialog sfd = new SaveFileDialog();
            sfd.Filter = "Rich Text Files (.rtf)|*.rtf";
            sfd.Title = "Save As...";
            sfd.DefaultExt = "*.rtf";
            sfd.RestoreDirectory = true;
            if (sfd.ShowDialog() == DialogResult.OK) {
                path = sfd.FileName;
            } else {
                return false;
            }
            rtb_terminal.SaveFile(path);
            return true;
        }
        #endregion

        #region event handlers
        #region Serial
        private void uart_data_received_handler(object sender, SerialDataReceivedEventArgs e) {
            try {
                _rtb_append(_port.ReadLine());
            } catch (Exception) {
                try {
                    _com_connect();
                } catch { }
            }
        }
        #endregion

        #region Button
        private void btn_connect_Click(object sender, EventArgs e) {
            if ((sender as Button).Text == "Connect") {
                cbb_com_port.Enabled = !_com_connect();
            } else if ((sender as Button).Text == "Disconnect") {
                cbb_com_port.Enabled = true;
                _checking_signal = false;
                _port.Dispose();
                (sender as Button).Text = "Connect";
            }
        }

        private void btn_refresh_Click(object sender, EventArgs e) {
            _com_update();
        }

        private void btn_answer_Click(object sender, EventArgs e) {
            if (btn_answer.Text == "Answer") {
                _com_write(AT_commands.AT_CALL_ANSWER);
                btn_answer.Text = "Hang Up";
                btn_answer.BackColor = Color.Red;
            } else if (btn_answer.Text == "Hang Up") {
                _com_write(AT_commands.AT_CALL_HANG_UP);
                btn_answer.Text = "Answer";
                btn_answer.BackColor = default(Color);
            }

        }

        private void btn_read_msg_Click(object sender, EventArgs e) {
            _com_write(AT_commands.AT_MSG_READ + nud_msg.Value);
            btn_read_msg.BackColor = default(Color);
        }

        private void btn_delete_msg_Click(object sender, EventArgs e) {
            _com_write(AT_commands.AT_MSG_DEL + nud_msg.Value + AT_commands.AT_MSG_DEL_END);
        }

        private void btn_ctrl_z_Click(object sender, EventArgs e) {
            if (_port == null || !_port.IsOpen) {
                _com_connect();
            }
            _port.WriteLine(((char)26).ToString());
        }

        private void btn_send_msg_Click(object sender, EventArgs e) {
            _com_write(AT_commands.AT_MSG_FORMAT + "1");
            _com_write(AT_commands.AT_MSG_SEND + tbx_send.Text + "\"");
        }

        private void btn_set_ringer_volume_Click(object sender, EventArgs e) {
            _com_write(AT_commands.AT_AUDIO_SET_RINGER_VOLUME + nud_ringer_volume.Value);
        }

        private void btn_set_speaker_volume_Click(object sender, EventArgs e) {
            _com_write(AT_commands.AT_AUDIO_SET_SPEAKER_VOLUME + nud_speaker_volume.Value);
        }

        private void btn_set_ring_tone_Click(object sender, EventArgs e) {
            _com_write(AT_commands.AT_AUDIO_SET_CURRENT_ALERT_SOUND + nud_ring_tone.Value);
        }

        private void btn_error_report_Click(object sender, EventArgs e) {
            _com_write(AT_commands.AT_DIAG_GET_ERROR_REPORT);
        }

        private void btn_network_reg_status_Click(object sender, EventArgs e) {
            _com_write(AT_commands.AT_CONN_NETWORK_REGISTRATION_STATUS);
        }

        private void btn_clear_Click(object sender, EventArgs e) {
            rtb_terminal.Clear();
        }

        private void btn_gps_pwr_Click(object sender, EventArgs e) {
            Button b = sender as Button;
            if (b.Text == "Off") {
                _com_write(AT_commands.AT_GPS_POWER_ON);
                b.BackColor = Color.DarkSeaGreen;
                b.Text = "On";
            } else if ((sender as Button).Text == "On") {
                _com_write(AT_commands.AT_GPS_POWER_OFF);
                b.BackColor = Color.Red;
                b.Text = "Off";
            }
        }

        private void btn_gps_status_Click(object sender, EventArgs e) {
            _com_write(AT_commands.AT_GPS_GET_STATUS);
        }

        private void btn_gps_hot_rst_Click(object sender, EventArgs e) {
            _com_write(AT_commands.AT_GPS_RST_AUTONOMY);
        }

        private void btn_gps_get_location_Click(object sender, EventArgs e) {
            _com_write(AT_commands.AT_GPS_GET_LOCATION);
        }

        private void btn_exit_Click(object sender, EventArgs e) {
            Application.Exit();
        }
        #endregion

        #region ContextMenuStrip
        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e) {
            _save_as();
        }

        private void timestampToolStripMenuItem_Click(object sender, EventArgs e) {
            _color_select(ref _color_timestamp);
        }

        private void textToolStripMenuItem_Click(object sender, EventArgs e) {
            _color_select(ref _color_text);
        }

        private void backgroundToolStripMenuItem_Click(object sender, EventArgs e) {
            _color_select(ref _color_background);
            rtb_terminal.BackColor = _color_background;
        }

        private void dockUndockToolStripMenuItem_Click(object sender, EventArgs e) {
            _chart_set_dock_state();
        }

        private void resetToolStripMenuItem_Click(object sender, EventArgs e) {
            _chart_clear_data();
        }
        #endregion

        #region NumberUpDown
        private void nud_ringer_volume_MouseDoubleClick(object sender, MouseEventArgs e) {
            _com_write(AT_commands.AT_AUDIO_GET_RINGER_VOLUME);
        }

        private void nud_speaker_volume_MouseDoubleClick(object sender, MouseEventArgs e) {
            _com_write(AT_commands.AT_AUDIO_GET_SPEAKER_VOLUME);
        }

        private void nud_ring_tone_MouseDoubleClick(object sender, MouseEventArgs e) {
            _com_write(AT_commands.AT_AUDIO_GET_CURRENT_ALERT_SOUND);
        }
        #endregion

        #region TextBox
        private void tbx_send_KeyDown(object sender, KeyEventArgs e) {
            if (e.KeyCode == Keys.Enter) {
                e.Handled = true;
                e.SuppressKeyPress = true;
                tbx_send.SelectAll();
                if (tbx_send.Text == "exit") {
                    Application.Exit();
                } else {
                    _com_write(tbx_send.Text);
                }
            }
        }
        #endregion

        #region CheckBox
        private void chk_signal_CheckedChanged(object sender, EventArgs e) {
            if ((sender as CheckBox).Checked) {
                prog_signal.Style = ProgressBarStyle.Continuous;
                _checking_signal = true;
                _signal_timer.Start();
            } else {
                prog_signal.Style = ProgressBarStyle.Marquee;
                _checking_signal = false;
                _wait_for_signal = false;
                lbl_signal_strength.Text = "---";
                lbl_signal_strength.ForeColor = Color.Black;
            }
        }

        private void chk_module_state_CheckedChanged(object sender, EventArgs e) {
            CheckBox c = sender as CheckBox;
            if (c.Text == "On") {
                _com_write(AT_commands.AT_STATE_OFF);
                c.Checked = false;
                c.Text = "Off";
            } else {
                _com_write(AT_commands.AT_STATE_ON);
                c.Checked = true;
                c.Text = "On";
            }
        }

        private void chk_graph_CheckedChanged(object sender, EventArgs e) {
            chart_signal.Visible = (sender as CheckBox).Checked;
        }
        #endregion

        #region ComboBox
        private void cbb_baud_rate_SelectedIndexChanged(object sender, EventArgs e) {
            if (_port != null) {
                _port.Dispose();
            }
            cbb_com_port.Enabled = true;
            btn_connect.Text = "Connect";
        }
        #endregion

        #region Chart
        private void chart_signal_DoubleClick(object sender, EventArgs e) {
            _chart_set_dock_state();
        }

        private void chart_signal_MouseEnter(object sender, EventArgs e) {
            if (!tbx_send.Focused) {
                chart_signal.Focus();
            }
        }

        private void chart_signal_MouseWheel(object sender, MouseEventArgs e) {
            try {
                double xMin = chart_signal.ChartAreas[0].AxisX.ScaleView.ViewMinimum;
                double xMax = chart_signal.ChartAreas[0].AxisX.ScaleView.ViewMaximum;
                double yMin = chart_signal.ChartAreas[0].AxisY.ScaleView.ViewMinimum;
                double yMax = chart_signal.ChartAreas[0].AxisY.ScaleView.ViewMaximum;
                double posXStart = 0;
                double posXFinish = 0;
                double posYStart = 0;
                double posYFinish = 0;

                if (e.Delta < 0) {
                    if (Control.ModifierKeys == Keys.Control) {
                        posYStart = chart_signal.ChartAreas[0].AxisY.PixelPositionToValue(e.Location.Y) - (yMax - yMin) * 1.1;
                        posYFinish = chart_signal.ChartAreas[0].AxisY.PixelPositionToValue(e.Location.Y) + (yMax - yMin) * 1.1;
                    } else {
                        posXStart = chart_signal.ChartAreas[0].AxisX.PixelPositionToValue(e.Location.X) - (xMax - xMin) * 1.1;
                        posXFinish = chart_signal.ChartAreas[0].AxisX.PixelPositionToValue(e.Location.X) + (xMax - xMin) * 1.1;
                    }
                } else if (e.Delta > 0) {
                    if (Control.ModifierKeys == Keys.Control) {
                        posYStart = chart_signal.ChartAreas[0].AxisY.PixelPositionToValue(e.Location.Y) - (yMax - yMin) / 3;
                        posYFinish = chart_signal.ChartAreas[0].AxisY.PixelPositionToValue(e.Location.Y) + (yMax - yMin) / 3;
                    } else {
                        posXStart = chart_signal.ChartAreas[0].AxisX.PixelPositionToValue(e.Location.X) - (xMax - xMin) / 3;
                        posXFinish = chart_signal.ChartAreas[0].AxisX.PixelPositionToValue(e.Location.X) + (xMax - xMin) / 3;
                    }


                }
                if (Control.ModifierKeys == Keys.Control) {
                    chart_signal.ChartAreas[0].AxisY.ScaleView.Zoom(posYStart, posYFinish);
                } else {
                    chart_signal.ChartAreas[0].AxisX.ScaleView.Zoom(posXStart, posXFinish);
                    _zoom_value = (int)(posXFinish - posXStart);
                }

            } catch { }
        }
        #endregion

        #region Form
        private void Form1_FormClosing(object sender, FormClosingEventArgs e) {
            _settings_save();
        }
        #endregion

        private void fontToolStripMenuItem_Click(object sender, EventArgs e) {
            FontDialog fd = new FontDialog();
            fd.Font = rtb_terminal.Font;
            try {
                if (fd.ShowDialog() == DialogResult.OK) {
                    rtb_terminal.Font = fd.Font;
                }
            } catch {
                rtb_terminal.AppendText("ERROR! Not a TrueType font!\n", Color.Red);
            }
        }
        #endregion

        private void btn_map_Click(object sender, EventArgs e) {
            _update_map();
        }

        private void _update_map() {
            String link = "http://www.google.com/maps?q=" + tbx_long.Text.Replace(',', '.') + "," + tbx_lat.Text.Replace(',', '.');
            wb_maps.Navigate(link);
            wb_maps.Visible = chk_map.Checked = true;
        }

        private void chk_map_CheckedChanged(object sender, EventArgs e) {
            wb_maps.Visible = chk_map.Checked;
            //_map_docked = chk_map.Checked;
            //_map_set_docked_state();
        }

        private void _map_set_docked_state() {
            //if (_map_docked) {
            //    this.pnl_main.Controls.Add(wb_maps);
            //    wb_maps.BringToFront();
            //    this.wb_maps.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
            //    | System.Windows.Forms.AnchorStyles.Left)
            //    | System.Windows.Forms.AnchorStyles.Right));
            //    this.wb_maps.Location = new System.Drawing.Point(7, 31);
            //    this.wb_maps.Size = new System.Drawing.Size(rtb_terminal.Width - 3, rtb_terminal.Height - 3);
            //    _map_docked = false;
            //} else {
            //    this.pnl_ctrls.Controls.Add(wb_maps);
            //    this.wb_maps.Anchor = ((System.Windows.Forms.AnchorStyles)(System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right));
            //    this.wb_maps.Size = new System.Drawing.Size(245, 120);
            //    //this.chart_signal.Location = new System.Drawing.Point(this.Width - chart_signal.Width - 45, 350);
            //    this.wb_maps.Location = new System.Drawing.Point(5, gb_connection.Height + gb_call.Height + gb_text_sms.Height + gb_gps.Height + (_chart_docked ? chart_signal.Height : 0) + 25);
            //    _map_docked = true;
            //}
        }
    }
}
