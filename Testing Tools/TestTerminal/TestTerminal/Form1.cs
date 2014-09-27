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

namespace TestTerminal {
    public partial class Form1 : Form {
        private SerialPort _port;
        private bool _port_open = false;
        private BaudRate _baud_rate;
        private bool _wait_for_signal;
        private bool _checking_signal;
        private int _signal_ctr;

        private const String CHART_SERIES = "series";
        private const int SIGNAL_DELAY = 1000;
        private bool _chart_docked;
        private int _zoom_value = 20;

        public Form1() {
            InitializeComponent();
            _init_rest();
            _init_chart();
        }

        private void _update_signal() {
            do {
                _wait_for_signal = true;
                this.BeginInvoke((Action)(() => { _send_to_com(AT_commands.AT_CONN_SIGNAL_STRENGTH); }));
                Thread.Sleep(SIGNAL_DELAY);
            } while (_checking_signal);
            _wait_for_signal = false;
            this.BeginInvoke((Action)(() => { chk_signal.Checked = false; }));
        }

        private void _init_rest() {
            _update_com();
            cbb_baud_rate.DataSource = BaudRateList.getInstance().getArray();
            tbx_send.Multiline = false;
            _load_settings();
            this.ActiveControl = tbx_send;
        }

        private void _load_settings() {
            cbb_baud_rate.SelectedIndex = Settings.Default.cbb_baud_rate;
            if (Settings.Default.cbb_com_port < cbb_com_port.Items.Count) {
                cbb_com_port.SelectedIndex = Settings.Default.cbb_com_port;
            }
            this.Size = Settings.Default.window_size;
            this.WindowState = Settings.Default.window_state;
            chk_graph.Checked = Settings.Default.chart_enabled;
            _chart_docked = Settings.Default.chart_dock_state;
        }

        private void _save_settings() {
            Settings.Default.cbb_baud_rate = cbb_baud_rate.SelectedIndex;
            Settings.Default.cbb_com_port = cbb_com_port.SelectedIndex;
            Settings.Default.window_state = this.WindowState != FormWindowState.Minimized ? this.WindowState : FormWindowState.Normal;
            if (this.WindowState == FormWindowState.Maximized) {
                this.WindowState = FormWindowState.Normal;
            }
            Settings.Default.window_size = this.Size;
            Settings.Default.chart_enabled = chk_graph.Checked;
            Settings.Default.chart_dock_state = !_chart_docked;

            Settings.Default.Save();
        }

        #region communication
        private void _update_com() {
            cbb_com_port.DataSource = SerialPortConnector.getAvalComPort();
        }

        private void _send_to_com(String s) {
            if (_port == null || !_port.IsOpen) {
                _connect_to_serial();
            }
            try {
                _port.WriteLine(s + Environment.NewLine);
            } catch { }
        }

        private bool _connect_to_serial() {
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
            if (p.Contains("+CSQ:")) {
                double raw_signal_value = double.Parse(p.Substring(p.IndexOf(' ') + 1, 4));
                double.TryParse(p.Substring(p.IndexOf(' ') + 1, 4), out raw_signal_value);
                prog_signal.Value = (int)(raw_signal_value * 10);
                double dbm = _calc_dbm(raw_signal_value);
                lbl_signal_strength.Text = dbm + "dbm";
                _add_new_point(CHART_SERIES, (_signal_ctr += SIGNAL_DELAY) / 1000, dbm);
                if (raw_signal_value == 0) {
                    lbl_signal_strength.ForeColor = Color.Red;
                    prog_signal.Style = ProgressBarStyle.Marquee;
                } else {
                    lbl_signal_strength.ForeColor = Color.Black;
                    prog_signal.Style = ProgressBarStyle.Continuous;
                }

            } else if (!_wait_for_signal) {
                rtb_terminal.AppendText(p);
                rtb_terminal.ScrollToCaret();

                if (p.Contains("RING")) {
                    btn_answer.BackColor = Color.Green;
                } else if (p.Contains("+CMTI:")) {
                    btn_read_msg.BackColor = Color.Green;
                } else if (p == "OK\r" || p == "ERROR\r") {
                    rtb_terminal.AppendText("=========================\n");
                }
            } else if (p == "OK\r") {
                _wait_for_signal = false;
            }
        }

        private double _calc_dbm(double csq_value) {
            return -113 + (csq_value * 2);
        }

        private void _init_chart() {
            chart_signal.MouseWheel += chart_signal_MouseWheel;
            chart_signal.Series.Clear();
            chart_signal.ChartAreas[0].AxisX.Minimum = 0;
            //chart_signal.ChartAreas[0].Area3DStyle.Enable3D = true;
            //chart_signal.ChartAreas[0].Area3DStyle.Inclination = 10;
            //chart_signal.ChartAreas[0].Area3DStyle.Rotation = 10;
            ChartControl.ChartSetup(chart_signal, CHART_SERIES, 1, Color.DarkRed, SeriesChartType.Line, ChartValueType.Int32);
            chart_signal.Series[CHART_SERIES].IsVisibleInLegend = false;
            _set_chart_dock_state();
        }

        delegate void _add_new_point_eh(String seriesName, double x, double y);
        private void _add_new_point(String seriesName, double x, double y) {
            if (this.InvokeRequired) {
                this.BeginInvoke(new _add_new_point_eh(_add_new_point), seriesName, x, y);
                return;
            }
            try {
                chart_signal.Series[seriesName].Points.AddXY((double)x, (double)y);
            } catch { }
            _zoom_trigger();
        }

        delegate void _zoom_trigger_eh();
        private void _zoom_trigger() {
            if (InvokeRequired) {
                this.BeginInvoke(new _zoom_trigger_eh(_zoom_trigger));
                return;
            }
            chart_signal.ChartAreas[0].CursorX.AutoScroll = true;
            chart_signal.ChartAreas[0].AxisX.ScaleView.Zoomable = true;
            chart_signal.ChartAreas[0].AxisX.ScaleView.SizeType = DateTimeIntervalType.Number;
            chart_signal.ChartAreas[0].AxisX.ScaleView.Zoom(_signal_ctr / 1000 - _zoom_value, _signal_ctr / 1000);
        }

        private void _set_chart_dock_state() {
            if (_chart_docked) {
                this.chart_signal.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                | System.Windows.Forms.AnchorStyles.Left)
                | System.Windows.Forms.AnchorStyles.Right));
                this.chart_signal.Location = new System.Drawing.Point(7, 31);
                this.chart_signal.Size = new System.Drawing.Size(rtb_terminal.Width, rtb_terminal.Height);
                _chart_docked = false;
            } else {
                this.chart_signal.Anchor = ((System.Windows.Forms.AnchorStyles)(System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right));
                this.chart_signal.Size = new System.Drawing.Size(245, 120);
                this.chart_signal.Location = new System.Drawing.Point(this.Width - chart_signal.Width - 45, 350);
                _chart_docked = true;
            }
        }
        #endregion

        #region event handlers
        private void uart_data_received_handler(object sender, SerialDataReceivedEventArgs e) {
            try {
                _rtb_append(_port.ReadLine());
            } catch (Exception) {
                try {
                    _connect_to_serial();
                } catch { }
            }
        }

        private void btn_connect_Click(object sender, EventArgs e) {
            if ((sender as Button).Text == "Connect") {
                cbb_com_port.Enabled = !_connect_to_serial();
            } else if ((sender as Button).Text == "Disconnect") {
                cbb_com_port.Enabled = true;
                _checking_signal = false;
                _port.Dispose();
                (sender as Button).Text = "Connect";
            }
        }

        private void btn_refresh_Click(object sender, EventArgs e) {
            _update_com();
        }

        private void btn_answer_Click(object sender, EventArgs e) {
            if (btn_answer.Text == "Answer") {
                _send_to_com(AT_commands.AT_CALL_ANSWER);
                btn_answer.Text = "Hang Up";
                btn_answer.BackColor = Color.Red;
            } else if (btn_answer.Text == "Hang Up") {
                _send_to_com(AT_commands.AT_CALL_HANG_UP);
                btn_answer.Text = "Answer";
                btn_answer.BackColor = default(Color);
            }

        }

        private void btn_read_msg_Click(object sender, EventArgs e) {
            _send_to_com(AT_commands.AT_MSG_READ + nud_msg.Value);
            btn_read_msg.BackColor = default(Color);
        }

        private void btn_delete_msg_Click(object sender, EventArgs e) {
            _send_to_com(AT_commands.AT_MSG_DEL + nud_msg.Value + AT_commands.AT_MSG_DEL_END);
        }

        private void btn_ctrl_z_Click(object sender, EventArgs e) {
            if (_port == null || !_port.IsOpen) {
                _connect_to_serial();
            }
            _port.WriteLine(((char)26).ToString());
        }

        private void btn_send_msg_Click(object sender, EventArgs e) {
            _send_to_com(AT_commands.AT_MSG_FORMAT + "1");
            _send_to_com(AT_commands.AT_MSG_SEND + tbx_send.Text + "\"");
        }

        private void btn_set_ringer_volume_Click(object sender, EventArgs e) {
            _send_to_com(AT_commands.AT_AUDIO_SET_RINGER_VOLUME + nud_ringer_volume.Value);
        }

        private void btn_set_speaker_volume_Click(object sender, EventArgs e) {
            _send_to_com(AT_commands.AT_AUDIO_SET_SPEAKER_VOLUME + nud_speaker_volume.Value);
        }

        private void nud_ringer_volume_MouseDoubleClick(object sender, MouseEventArgs e) {
            _send_to_com(AT_commands.AT_AUDIO_GET_RINGER_VOLUME);
        }

        private void nud_speaker_volume_MouseDoubleClick(object sender, MouseEventArgs e) {
            _send_to_com(AT_commands.AT_AUDIO_GET_SPEAKER_VOLUME);
        }

        private void nud_ring_tone_MouseDoubleClick(object sender, MouseEventArgs e) {
            _send_to_com(AT_commands.AT_AUDIO_GET_CURRENT_ALERT_SOUND);
        }

        private void btn_set_ring_tone_Click(object sender, EventArgs e) {
            _send_to_com(AT_commands.AT_AUDIO_SET_CURRENT_ALERT_SOUND + nud_ring_tone.Value);
        }

        private void btn_error_report_Click(object sender, EventArgs e) {
            _send_to_com(AT_commands.AT_DIAG_GET_ERROR_REPORT);
        }

        private void btn_network_reg_status_Click(object sender, EventArgs e) {
            _send_to_com(AT_commands.AT_CONN_NETWORK_REGISTRATION_STATUS);
        }

        private void btn_clear_Click(object sender, EventArgs e) {
            rtb_terminal.Clear();
        }

        private void btn_gps_pwr_Click(object sender, EventArgs e) {
            Button b = sender as Button;
            if (b.Text == "Off") {
                _send_to_com(AT_commands.AT_GPS_POWER_ON);
                b.BackColor = Color.DarkSeaGreen;
                b.Text = "On";
            } else if ((sender as Button).Text == "On") {
                _send_to_com(AT_commands.AT_GPS_POWER_OFF);
                b.BackColor = Color.Red;
                b.Text = "Off";
            }
        }

        private void btn_gps_status_Click(object sender, EventArgs e) {
            _send_to_com(AT_commands.AT_GPS_GET_STATUS);
        }

        private void btn_gps_cold_rst_Click(object sender, EventArgs e) {
            _send_to_com(AT_commands.AT_GPS_RST_COLD);
        }

        private void btn_gps_get_location_Click(object sender, EventArgs e) {
            _send_to_com(AT_commands.AT_GPS_GET_LOCATION);
        }

        private void btn_exit_Click(object sender, EventArgs e) {
            Application.Exit();
        }

        private void tbx_send_KeyDown(object sender, KeyEventArgs e) {
            if (e.KeyCode == Keys.Enter) {
                e.Handled = true;
                e.SuppressKeyPress = true;
                tbx_send.SelectAll();
                if (tbx_send.Text == "exit") {
                    Application.Exit();
                } else {
                    _send_to_com(tbx_send.Text);
                }
            }
        }

        private void chk_signal_CheckedChanged(object sender, EventArgs e) {
            if ((sender as CheckBox).Checked) {
                prog_signal.Style = ProgressBarStyle.Continuous;
                _checking_signal = true;
                Thread t = new Thread(() => _update_signal());
                t.IsBackground = true;
                t.Start();
            } else {
                _checking_signal = false;
                _wait_for_signal = false;
                lbl_signal_strength.Text = "---";
                lbl_signal_strength.ForeColor = Color.Black;
            }
        }

        private void chk_module_state_CheckedChanged(object sender, EventArgs e) {
            CheckBox c = sender as CheckBox;
            if (c.Text == "On") {
                _send_to_com(AT_commands.AT_STATE_OFF);
                c.Checked = false;
                c.Text = "Off";
            } else {
                _send_to_com(AT_commands.AT_STATE_ON);
                c.Checked = true;
                c.Text = "On";
            }
        }

        private void chk_graph_CheckedChanged(object sender, EventArgs e) {
            chart_signal.Visible = (sender as CheckBox).Checked;
        }

        private void cbb_baud_rate_SelectedIndexChanged(object sender, EventArgs e) {
            if (_port != null) {
                _port.Dispose();
            }
            cbb_com_port.Enabled = true;
            btn_connect.Text = "Connect";
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e) {
            _save_settings();
        }

        private void chart_signal_MouseEnter(object sender, EventArgs e) {
            if (!tbx_send.Focused) {
                chart_signal.Focus();
            }
        }

        private void chart_signal_DoubleClick(object sender, EventArgs e) {
            _set_chart_dock_state();
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
    }
}
