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

namespace TestTerminal {
    public partial class Form1 : Form {
        private SerialPort _port;
        private bool _port_open = false;
        private BaudRate _baud_rate;

        public Form1() {
            InitializeComponent();
            _init_rest();
        }

        private void _init_rest() {
            _update_com();
            cbb_baud_rate.DataSource = BaudRateList.getInstance().getArray();
            tbx_send.Multiline = false;
            _load_settings();
        }

        private void _load_settings() {
            cbb_baud_rate.SelectedIndex = Settings.Default.cbb_baud_rate;
            this.Size = Settings.Default.window_size;
            this.WindowState = Settings.Default.window_state;
        }

        private void _save_settings() {
            Settings.Default.cbb_baud_rate = cbb_baud_rate.SelectedIndex;
            Settings.Default.window_state = this.WindowState != FormWindowState.Minimized ? this.WindowState : FormWindowState.Normal;
            if (this.WindowState == FormWindowState.Maximized) {
                this.WindowState = FormWindowState.Normal;
            }
            Settings.Default.window_size = this.Size;

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
            } catch {}
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
            } catch (Exception) {
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
            rtb_terminal.AppendText(p);
            rtb_terminal.ScrollToCaret();

            if (p.Contains("RING")) {
                btn_answer.BackColor = Color.Green;
            } else if (p.Contains("+CMTI:")) {
                btn_read_msg.BackColor = Color.Green;
            } else if (p.Contains("+CSQ:")) {
                lbl_signal_strength.Text = (p.Substring(p.IndexOf(' ') + 1, 4));
            } else if (p == "OK\r" || p == "ERROR\r") {
                rtb_terminal.AppendText("=========================\n");
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
                if (_connect_to_serial()) {
                    cbb_com_port.Enabled = false;
                    (sender as Button).Text = "Disconnect";
                } else {
                    MessageBox.Show("Error opening port", "Connection error");
                }
            } else if ((sender as Button).Text == "Disconnect") {
                cbb_com_port.Enabled = true;
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

        private void btn_delete_all_msg_Click(object sender, EventArgs e) {
            _send_to_com(AT_commands.AT_MSG_DEL_ALL);
        }

        private void btn_ctrl_z_Click(object sender, EventArgs e) {
            if (_port == null || !_port.IsOpen) {
                _connect_to_serial();
            }
            _port.WriteLine(((char)26).ToString());
        }

        private void btn_signal_strength_Click(object sender, EventArgs e) {
            _send_to_com(AT_commands.AT_CONN_SIGNAL_STRENGTH);
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

        private void tbx_send_KeyDown(object sender, KeyEventArgs e) {
            if (e.KeyCode == Keys.Enter) {
                e.Handled = true;
                if (tbx_send.Text == "exit") {
                    Application.Exit();
                } else {
                    _send_to_com(tbx_send.Text);
                }
            }
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e) {
            _save_settings();
        }
        #endregion

        private void cbb_baud_rate_SelectedIndexChanged(object sender, EventArgs e) {
            if (_port != null) {
                _port.Dispose();
            }
            cbb_com_port.Enabled = true;
            btn_connect.Text = "Connect";
        }

        private void btn_clear_Click(object sender, EventArgs e) {
            rtb_terminal.Clear();
        }
    }
}
