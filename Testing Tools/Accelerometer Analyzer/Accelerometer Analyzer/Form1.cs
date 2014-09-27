using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using AMD.Util;
using Accelerometer_Analyzer.AMD.util;
using System.Windows.Forms.DataVisualization.Charting;
using AMD.Util.LiniarAlgebra;
using System.Threading;
using Accelerometer_Analyzer.Properties;
using AMD.Util.Serial.BaudRates;

namespace Accelerometer_Analyzer {
    public partial class Form1 : Form {
        /* COM port */
        private const int NO_OF_RECEIVED_BYTES = 8;
        private SerialPort _port;
        private bool _start_reading;
        private bool _port_open = false;
        private BaudRate _baud_rate;

        /* data collection for accelerometer data */
        private BindingList<byte> _received_data;
        private BindingList<Vector3D> _received_vectors;
        private byte _prev_byte;
        private int _length_offset;
        private int _readingIndex;

        /* timing */
        DateTime _start_time;

        /* file writing */
        private const String _DateTime_format = "yyyy-MM-dd HH-mm-ss";
        private bool _file_saved = true;
        private String _file_path = "";

        public Form1() {
            InitializeComponent();
            Init_rest();
            Init_chart();
            Init_state();
        }

        private void Init_rest() {

            /* COM port */
            _start_reading = false;
            _update_com();
            cbb_baud_rate.DataSource = BaudRateList.getInstance().getArray();

            /* data collection for accelerometer data */
            _received_data = new BindingList<byte>();
            _received_data.AllowNew = true;
            _received_vectors = new BindingList<Vector3D>();
            _received_vectors.AllowNew = true;

            _received_data.ListChanged += new ListChangedEventHandler(raw_list_changed);
            _received_vectors.ListChanged += new ListChangedEventHandler(list_changed);

            /* register mousewheel event handler for chart1 */
            chart1.MouseWheel += chart1_MouseWheel;
        }

        private void Init_state() {
            /* loading settings */
            _load_settings();

            /* set checkbox status */
            chart1.Series["acc_x"].Enabled = chk_x.Checked;
            chart1.Series["acc_y"].Enabled = chk_y.Checked;
            chart1.Series["acc_z"].Enabled = chk_z.Checked;
            chart1.Series["v_len"].Enabled = chk_v_len.Checked;
        }

        private void _load_settings() {
            chk_x.Checked = Settings.Default.x_axis_en;
            chk_y.Checked = Settings.Default.y_axis_en;
            chk_z.Checked = Settings.Default.z_axis_en;
            chk_v_len.Checked = Settings.Default.v_len_en;
            chk_raw_data.Checked = Settings.Default.col_raw_data;
            cbb_baud_rate.SelectedIndex = Settings.Default.cbb_baud_rate;
            this.Size = Settings.Default.form_size;
            this.WindowState = Settings.Default.form_state;
        }

        private void _save_settings() {
            Settings.Default.x_axis_en = chk_x.Checked;
            Settings.Default.y_axis_en = chk_y.Checked;
            Settings.Default.z_axis_en = chk_z.Checked;
            Settings.Default.v_len_en = chk_v_len.Checked;
            Settings.Default.col_raw_data = chk_raw_data.Checked;
            Settings.Default.cbb_baud_rate = cbb_baud_rate.SelectedIndex;
            Settings.Default.form_state = this.WindowState != FormWindowState.Minimized ? this.WindowState : FormWindowState.Normal;
            if (this.WindowState == FormWindowState.Maximized) {
                this.WindowState = FormWindowState.Normal;
            }
            Settings.Default.form_size = this.Size;
            
            Settings.Default.Save();
        }

        #region communication
        private void _update_com() {
            cbb_comport.DataSource = SerialPortConnector.getAvalComPort();
        }

        private bool _connect_to_serial() {
            try {
                if (_port_open) {
                    _port.Close();
                }
                _port = new SerialPort();
                _baud_rate = cbb_baud_rate.SelectedItem as BaudRate;
                SerialPortConnector.SerialSetup(_port, cbb_comport.SelectedValue.ToString(), (cbb_baud_rate.SelectedValue as BaudRate));
                _port.DataReceived += new SerialDataReceivedEventHandler(uart_data_received_handler);
                _port_open = _port.IsOpen;
                cbb_comport.Enabled = false;
                chart1.BorderlineColor = Color.LimeGreen;
            } catch (Exception) {
                chart1.BorderlineColor = Color.Red;
                return false;
            }
            return true;
        }
        #endregion

        #region chart control
        private void Init_chart() {
            chart1.Series.Clear();
            chart1.ChartAreas[0].AxisX.Minimum = 0;
            ChartControl.ChartSetup(chart1, "acc_x", 1, Color.DarkRed, SeriesChartType.Line, ChartValueType.Int32);
            ChartControl.ChartSetup(chart1, "acc_y", 1, Color.DarkGreen, SeriesChartType.Line, ChartValueType.Int32);
            ChartControl.ChartSetup(chart1, "acc_z", 1, Color.DarkBlue, SeriesChartType.Line, ChartValueType.Int32);
            ChartControl.ChartSetup(chart1, "v_len", 1, Color.DarkOrange, SeriesChartType.Line, ChartValueType.Int32);
            //ChartControl.ChartSetup(chart1, "GAng", 1, Color.DarkGoldenrod, SeriesChartType.Line, ChartValueType.Int32);
            //ChartControl.ChartSetup(chart1, "AccLen", 1, Color.Black, SeriesChartType.Line, ChartValueType.Int32);
            //ChartControl.ChartSetup(chart1, "Velocity", 1, Color.Purple, SeriesChartType.Line, ChartValueType.Int32);
        }

        delegate void _add_new_point_eh(String seriesName, double x, double y);
        private void _add_new_point(String seriesName, double x, double y) {
            if (this.InvokeRequired) {
                this.BeginInvoke(new _add_new_point_eh(_add_new_point), seriesName, x, y);
                return;
            }
            try {
                chart1.Series[seriesName].Points.AddXY((double)x, (double)y);
            } catch {} 
            _zoom_trigger();
        }

        delegate void _clear_data_eh();
        private void _clear_data() {
            if (InvokeRequired) {
                this.BeginInvoke(new _clear_data_eh(_clear_data));
                return;
            }
            foreach (var series in chart1.Series) {
                series.Points.Clear();
            }
            rtb_data.Clear();
        }

        delegate void _chart_visible_eh(bool b);
        private void _chart_visible(bool b) {
            if (InvokeRequired) {
                this.BeginInvoke(new _chart_visible_eh(_chart_visible), b);
                return;
            }
            chart1.Visible = b;
        }

        delegate void _zoom_trigger_eh();
        private void _zoom_trigger() {
            if (InvokeRequired) {
                this.BeginInvoke(new _zoom_trigger_eh(_zoom_trigger));
                return;
            }
            if (!chkFullScale.Checked) {
                chart1.ChartAreas[0].CursorX.AutoScroll = true;
                chart1.ChartAreas[0].AxisX.ScaleView.Zoomable = true;
                chart1.ChartAreas[0].AxisX.ScaleView.SizeType = DateTimeIntervalType.Number;
                chart1.ChartAreas[0].AxisX.ScaleView.Zoom(_readingIndex - tkb_scale.Value, _readingIndex);
            } else {
                chart1.ChartAreas[0].CursorX.AutoScroll = false;
                chart1.ChartAreas[0].AxisX.ScaleView.Zoomable = false;
                chart1.ChartAreas[0].AxisX.ScaleView.ZoomReset();
            }
        }

        private delegate void process_data_eh(Vector3D acc);
        private void process_data(Vector3D acc) {
            _readingIndex++;
            double len = acc.Length;
            _add_new_point("acc_x", _readingIndex, acc.x);
            _add_new_point("acc_y", _readingIndex, acc.y);
            _add_new_point("acc_z", _readingIndex, acc.z);
            _add_new_point("v_len", _readingIndex, len + _length_offset);
            if (chk_raw_data.Checked) {
                _rtb_append(_readingIndex + " -> " + acc.ToString() + "\n");
            }
        }
        #endregion

        #region gui
        delegate void _set_title_eh(string p);
        private void _set_title(string p) {
            if (InvokeRequired) {
                this.BeginInvoke(new _set_title_eh(_set_title), p);
                return;
            }
            this.Text = p;
        }

        delegate void _rtb_append_eh(String p);
        private void _rtb_append(String p) {
            if (InvokeRequired) {
                this.BeginInvoke(new _rtb_append_eh(_rtb_append), p);
                return;
            }
            rtb_data.AppendText(p);
        }

        delegate void _set_series_state_eh(Nullable<bool> state);
        private void _set_series_state(Nullable<bool> state) {
            if (InvokeRequired) {
                this.BeginInvoke(new _set_series_state_eh(_set_series_state), state);
                return;
            }
            chart1.Series["acc_x"].Enabled = state == null ? chk_x.Checked : (bool)state;
            chart1.Series["acc_y"].Enabled = state == null ? chk_y.Checked : (bool)state;
            chart1.Series["acc_z"].Enabled = state == null ? chk_z.Checked : (bool)state;
            chart1.Series["v_len"].Enabled = state == null ? chk_v_len.Checked : (bool)state;
        }
        #endregion

        #region file
        private void _process_file(String file_path) {
            StreamReader sr = new StreamReader(file_path);
            try {
                _file_path = file_path;
                _set_title(file_path);
            } catch (Exception ex) {
                MessageBox.Show("Error: Could not read file from disk. Original error: " + ex.Message);
            }

            if (sr != null) {
                System.Windows.Forms.Cursor.Current = Cursors.WaitCursor;
                Thread t = new Thread(() => _print_csv(sr));
                t.IsBackground = true;
                t.Start();
            }
        }

        public void _print_csv(StreamReader sr) {
            String line;
            _clear_data();
            _readingIndex = 0;
            _set_series_state(true);
            while ((line = sr.ReadLine()) != null) {
                process_data(_get_vector_from_csv(line));
            }
            sr.Dispose();
            _set_title(_file_path + " - END OF FILE!");
            Thread.Sleep(100);
            _zoom_trigger();
            _set_series_state(null);
        }

        private Vector3D _get_vector_from_csv(String data) {
            data.Replace(@"\r", "");
            string[] dataArr = data.Split(',');
            if (dataArr.Length > 3) {
                double acc_x_raw = Int32.Parse(dataArr[0]);
                double acc_y_raw = Int32.Parse(dataArr[1]);
                double acc_z_raw = Int32.Parse(dataArr[2]);
                return new Vector3D(acc_x_raw, acc_y_raw, acc_z_raw);
            } else {
                return new Vector3D();
            }
        }

        private bool _write_to_file(bool save_as) {
            if (_readingIndex > 0) {
                String current_dir = Directory.GetCurrentDirectory() + "\\";
                String path = "Reading byte " + _start_time.ToString(_DateTime_format) + ".npp";

                if (save_as) {
                    SaveFileDialog sfd = new SaveFileDialog();
                    sfd.Filter = "Notepad++ Files (.npp)|*.npp|Text Files (.txt)|*.txt";
                    sfd.Title = "Save As...";
                    sfd.DefaultExt = "*.npp";
                    sfd.RestoreDirectory = true;
                    sfd.FileName = path;
                    if (sfd.ShowDialog() == DialogResult.OK) {
                        path = sfd.FileName;
                    } else {
                        return false;
                    }
                } else {
                    path = current_dir + path;
                }

                _file_path = path;
                using (StreamWriter file_formatted = new StreamWriter(path, false)) {
                    foreach (var v in _received_vectors) {
                        try { file_formatted.Write(v.x + "," + v.y + "," + v.z + "," + "\n"); } catch { }
                    }
                    file_formatted.Close();
                }
                _readingIndex = 0;
                _received_data.Clear();
                _received_vectors.Clear();
                return _file_saved = true;
            }
            return false;
        }

        private void _reload_file() {
            if (_file_path != null && _file_path.Length > 0) {
                StreamReader sr = new System.IO.StreamReader(_file_path);
                Thread t = new Thread(() => _print_csv(sr));
                t.IsBackground = true;
                t.Start();
            }
        }
        #endregion

        #region EventHandlers
        private void list_changed(object sender, ListChangedEventArgs e) {
            if (_received_vectors.Count > 0) {
                process_data(_received_vectors[e.NewIndex]);
            }
        }

        private void raw_list_changed(object sender, ListChangedEventArgs e) {
            if (_received_data.Count > 0) {
                byte readFromCom = _received_data[e.NewIndex];
                if (_prev_byte == 0x7f &&
                    readFromCom == 0xff &&
                    _received_data.Count > NO_OF_RECEIVED_BYTES + 2 &&
                    _received_data[_received_data.Count - NO_OF_RECEIVED_BYTES - 2] == 0x7f &&
                    _received_data[_received_data.Count - NO_OF_RECEIVED_BYTES - 1] == 0xff
                    ) { // detecting line change

                    int length = _received_data.Count;
                    bool msb = true;
                    byte b = 0;
                    byte b_prev = 0;
                    int[] axis = new int[3];
                    for (int i = 0; i < NO_OF_RECEIVED_BYTES - 2; i++) {
                        b = _received_data[length - NO_OF_RECEIVED_BYTES + i];
                        if (msb) {
                            msb = false;
                            b_prev = b;
                        } else {
                            int value = 0;
                            value = b_prev << 8 | b;
                            if (b_prev > 127)
                                value = value - 65535;
                            axis[i / 2] = value;
                            msb = true;
                        }
                    }
                    _received_vectors.Add(new Vector3D(axis[0], axis[1], axis[2]));
                }
                _prev_byte = readFromCom;
            }
        }

        private void btn_start_Click(object sender, EventArgs e) {
            _readingIndex = 0;
            _received_data.Clear();
            _received_vectors.Clear();
            _file_path = "";
            if (_port == null || !_port.IsOpen) {
                _connect_to_serial();
            }
            if (_port.IsOpen) {
                _port.DiscardInBuffer();
                _clear_data();
                _start_reading = true;
                _start_time = DateTime.Now;
                _file_saved = false;
            } else {
                MessageBox.Show("An error occurred while trying to open the COM port.", "Connection error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void btn_rst_buf_Click(object sender, EventArgs e) {
            if (_port != null && _port.IsOpen) {
                _port.DiscardInBuffer();
            }
        }

        private void btn_stop_Click(object sender, EventArgs e) {
            _start_reading = false;
        }

        private void btn_refresh_Click(object sender, EventArgs e) {
            _update_com();
        }

        private void btn_raw_Click(object sender, EventArgs e) {
            Button b = sender as Button;
            if (b.Text == "Show") {
                if (!chk_raw_data.Checked) {
                    chk_raw_data.Checked = true;
                }
                b.Text = "Hide";
                pnl_chart.Size = new System.Drawing.Size((int)(pnl_chart.Size.Width - 200), pnl_chart.Size.Height);
                rtb_data.Visible = true;
            } else {
                b.Text = "Show";
                pnl_chart.Size = new System.Drawing.Size((int)(pnl_chart.Size.Width + 200), pnl_chart.Size.Height);
                rtb_data.Visible = false;
            }
        }

        private void tkb_length_offset_KeyDown(object sender, KeyEventArgs e) {
            if (e.KeyCode == Keys.R) {
                tkb_length_offset.Value = _length_offset = 0;
            }
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e) {
            _start_reading = false;

            OpenFileDialog ofd = new OpenFileDialog();

            ofd.Filter = "Notepad++ (.npp)|*.npp|Text Files (.txt)|*.txt|All Files (*.*)|*.*";
            ofd.FilterIndex = 1;
            ofd.RestoreDirectory = true;

            ofd.Multiselect = false;

            if (ofd.ShowDialog() == DialogResult.OK) {
                _process_file(ofd.FileName);
                
            }
        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e) {
            try {
                //_port.Close();
                _port.Dispose();
                chart1.BorderlineColor = Color.Black;
            } catch { }
            _start_reading = false;
            _write_to_file(true);
        }

        private void printToolStripMenuItem_Click(object sender, EventArgs e) {
            chart1.Printing.Print(true);
        }

        private void cbb_baud_rate_SelectedIndexChanged(object sender, EventArgs e) {
            _baud_rate = ((sender as ComboBox).SelectedItem as BaudRate);
            if (_port_open) {
                try {
                    _port.Close();
                    _connect_to_serial();
                } catch (Exception) {
                    
                    throw;
                }
            }
        }

        private void tbx_send_KeyDown(object sender, KeyEventArgs e) {
            if (e.KeyCode == Keys.Enter) {
                if (_port == null || !_port.IsOpen) {
                    _connect_to_serial();
                }
                if (_port.IsOpen && tbx_send.Text.Length > 0) {
                    try {
                        _port.Write(tbx_send.Text);
                    } catch (Exception) {
                        throw;
                    }
                }
            }
        }

        private void tkb_scale_MouseEnter(object sender, EventArgs e) {
            if (!tbx_send.Focused) {
                tkb_scale.Focus();
            }
        }

        private void tkb_scale_Scroll(object sender, EventArgs e) {
            _zoom_trigger();
        }

        private void tkb_length_offset_MouseEnter(object sender, EventArgs e) {
            if (!tbx_send.Focused) {
                tkb_length_offset.Focus();
            }
        }

        private void chart1_MouseEnter(object sender, EventArgs e) {
            if (!tbx_send.Focused) {
                chart1.Focus();
            }
        }

        private void chart1_MouseWheel(object sender, MouseEventArgs e) {
            try {
                double xMin = chart1.ChartAreas[0].AxisX.ScaleView.ViewMinimum;
                double xMax = chart1.ChartAreas[0].AxisX.ScaleView.ViewMaximum;
                double yMin = chart1.ChartAreas[0].AxisY.ScaleView.ViewMinimum;
                double yMax = chart1.ChartAreas[0].AxisY.ScaleView.ViewMaximum;
                double posXStart = 0;
                double posXFinish = 0;
                double posYStart = 0;
                double posYFinish = 0;

                if (e.Delta < 0) {
                    if (Control.ModifierKeys == Keys.Control) {
                        posYStart = chart1.ChartAreas[0].AxisY.PixelPositionToValue(e.Location.Y) - (yMax - yMin) * 1.1;
                        posYFinish = chart1.ChartAreas[0].AxisY.PixelPositionToValue(e.Location.Y) + (yMax - yMin) * 1.1;
                    } else {
                        posXStart = chart1.ChartAreas[0].AxisX.PixelPositionToValue(e.Location.X) - (xMax - xMin) * 1.1;
                        posXFinish = chart1.ChartAreas[0].AxisX.PixelPositionToValue(e.Location.X) + (xMax - xMin) * 1.1;
                    }
                } else if (e.Delta > 0) {
                    if (Control.ModifierKeys == Keys.Control) {
                        posYStart = chart1.ChartAreas[0].AxisY.PixelPositionToValue(e.Location.Y) - (yMax - yMin) / 3;
                        posYFinish = chart1.ChartAreas[0].AxisY.PixelPositionToValue(e.Location.Y) + (yMax - yMin) / 3;
                    } else {
                        posXStart = chart1.ChartAreas[0].AxisX.PixelPositionToValue(e.Location.X) - (xMax - xMin) / 3;
                        posXFinish = chart1.ChartAreas[0].AxisX.PixelPositionToValue(e.Location.X) + (xMax - xMin) / 3;
                    }


                }
                if (Control.ModifierKeys == Keys.Control) {
                    chart1.ChartAreas[0].AxisY.ScaleView.Zoom(posYStart, posYFinish);
                } else {
                    chart1.ChartAreas[0].AxisX.ScaleView.Zoom(posXStart, posXFinish);
                    tkb_scale.Value = (int)(posXFinish - posXStart);
                }

            } catch { }
        }

        private void chk_x_CheckedChanged(object sender, EventArgs e) {
            chart1.Series["acc_x"].Enabled = chk_x.Checked;
        }

        private void chk_y_CheckedChanged(object sender, EventArgs e) {
            chart1.Series["acc_y"].Enabled = chk_y.Checked;
        }

        private void chk_z_CheckedChanged(object sender, EventArgs e) {
            chart1.Series["acc_z"].Enabled = chk_z.Checked;
        }

        private void chk_v_len_CheckedChanged(object sender, EventArgs e) {
            chart1.Series["v_len"].Enabled = chk_v_len.Checked;
        }

        private void uart_data_received_handler(object sender, SerialDataReceivedEventArgs e) {
            if (!_start_reading) {
                if (_port_open) {
                    try {
                        _port.Close();
                    } catch {}
                }
                return;
            } else {
                byte b = (byte)_port.ReadByte();
                _received_data.Add(b);
            }
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e) {
            _start_reading = false;
            _save_settings();
            if (!_file_saved) {
                DialogResult dr = MessageBox.Show("You have unsaved data, do you want to save them before exiting?", "Unsaved data", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Question);
                switch (dr) {
                    case DialogResult.Cancel:
                        e.Cancel = true;
                        break;
                    case DialogResult.Yes:
                        e.Cancel = true;
                        if (_write_to_file(true)) {
                            e.Cancel = false;
                        }
                        break;
                    default:
                        break;
                }
            }
            //Thread.Sleep(_readingIndex / 100 + 250);
        }
        #endregion

        private void chart1_DragDrop(object sender, DragEventArgs e) {

            // process_data(_get_vector_from_csv(line));
            // Handle FileDrop data. 
            if (e.Data.GetDataPresent(DataFormats.FileDrop)) {
                // Assign the file names to a string array, in  
                // case the user has selected multiple files. 
                string[] files = (string[])e.Data.GetData(DataFormats.FileDrop);
                if (files.Length > 1) {
                    MessageBox.Show("You can only drop \"1\" file at the time", "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                }
                try {
                    _process_file(files[0]);
                } catch (Exception ex) {
                    MessageBox.Show(ex.Message);
                    return;
                }
            }
        }

        private void chart1_DragEnter(object sender, DragEventArgs e) {
            if (e.Data.GetDataPresent(DataFormats.Text) ||
               e.Data.GetDataPresent(DataFormats.FileDrop)) {
                e.Effect = DragDropEffects.Copy;
            } else {
                e.Effect = DragDropEffects.None;
            }
        }

        private void tkb_length_offset_ValueChanged(object sender, EventArgs e) {
            _length_offset = (sender as TrackBar).Value;
            _reload_file();
        }
    }
}
