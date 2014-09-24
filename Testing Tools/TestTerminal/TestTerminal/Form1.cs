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
            if (_connect_to_serial()) {
                cbb_com_port.Enabled = false;
            } else {
                MessageBox.Show("Error opening port", "Connection error");
            }
        }

        private void btn_refresh_Click(object sender, EventArgs e) {
            _update_com();
        }

        private void btn_answer_Click(object sender, EventArgs e) {
            if (btn_answer.Text == "Answer") {
                _send_to_com(AT_commands.AT_ANSWER);
                btn_answer.Text = "Hang Up";
                btn_answer.BackColor = Color.Red;
            } else if (btn_answer.Text == "Hang Up") {
                _send_to_com(AT_commands.AT_HANG_UP);
                btn_answer.Text = "Answer";
                btn_answer.BackColor = default(Color);
            }

        }

        private void tbx_send_KeyDown(object sender, KeyEventArgs e) {
            if (e.KeyCode == Keys.Enter) {
                e.Handled = true;
                if (_port == null || !_port.IsOpen) {
                    _connect_to_serial();
                }
                if (_port.IsOpen && tbx_send.Text.Length > 0) {
                    try {
                        _send_to_com(tbx_send.Text);
                    } catch (Exception) {
                        throw;
                    }
                }
            }
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e) {
            _save_settings();
        }
        #endregion

        private void btn_ctrl_z_Click(object sender, EventArgs e) {
            if (_port == null || !_port.IsOpen) {
                _connect_to_serial();
            }
            _port.WriteLine(((char)26).ToString());
        }
    }
}
