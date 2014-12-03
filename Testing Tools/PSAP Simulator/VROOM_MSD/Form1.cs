/* *
 * *    Author:         Kenneth René Jensen
 * *    Description:    Test GUI simulating PSAP 
 * *    Version:        1
 * */

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Media;
using VROOM_MSD.Properties;
using System.Reflection;
using System.Resources;
using System.Windows.Media;
using System.Threading;
using Microsoft.Maps.MapControl.WPF;

namespace VROOM_MSD
{
    public partial class MainForm : Form
    {
        private string _path;
        private MSD_structure _msd;
        private int coordinate_idx;
        private SoundPlayer alarm_sound;
        private Location location;
        private readonly Location DENMARK = new Location(56.0, 10.5);
        private readonly double ZOOM = 6.08;

        public MainForm()
        {
            InitializeComponent();
            SetupBingMap();

            _path = Settings.Default.LastPath;
            path_label.Text = _path;
            MSD_File_Watcher.Path = _path;
            folderBrowserDialog1.SelectedPath = _path;
            folderBrowserDialog1.ShowNewFolderButton = false;
            folderBrowserDialog1.Description = "Locate MSD folder (*.vroom files)";
            _msd = new MSD_structure();
            alarm_sound = new SoundPlayer(Path.GetFullPath("alert.wav"));

            LoadFiles();
        }

        private void LoadFiles()
        {
            String[] items = Directory.GetFiles(MSD_File_Watcher.Path, MSD_File_Watcher.Filter);

            foreach (String file in items)
            {
                string s = Path.GetFileName(file);
                msd_text_box.Items.Add(s);
                _msd.AddNewMSD(s, ReadSelectedFile(_path + s));
            }

            msd_text_box.SelectedIndex = msd_text_box.Items.Count - 1;
        }

        private void SetupBingMap()
        {
            mapUserControl.Map.CredentialsProvider = new ApplicationIdCredentialsProvider("AvNimwfYic8BJOuBLqc6jTBeroePra8F7kovNHua9kXACh4SPdBBtoL3a1PZCqmT");

            mapUserControl.Map.Height = elementHost.Height;
            mapUserControl.Map.Width = elementHost.Width;
            mapUserControl.Map.SetView(DENMARK, ZOOM);     
            MapViewStyle.SelectedIndex = 1;
        }

        private void MSD_File_Watcher_Changed(object sender, System.IO.FileSystemEventArgs e)
        {
            msd_text_box.Items.Add(e.Name);
            _msd.AddNewMSD(e.Name, ReadSelectedFile(_path + e.Name));

            alarm_sound.Play();
        }

        private void MSD_File_Watcher_Deleted(object sender, System.IO.FileSystemEventArgs e)
        {
            msd_text_box.Items.Remove(e.Name);
            _msd.DeleteMSD(e.Name);
        }

        private Byte[] ReadSelectedFile(String path)
        {
            byte[] data;

            while (true)
            {
                try
                {
                    using (FileStream msd = new FileStream(path, FileMode.Open))
                    {
                        data = new byte[msd.Length];
                        msd.Read(data, 0, (int)msd.Length);
                        break;
                    }
                }
                catch (IOException)
                {
                    Thread.Sleep(100);
                }
            }

            return data;
        }

        private void msd_file_box_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (msd_text_box.SelectedItem != null) 
            {
                _msd.DecodeMSD(msd_text_box.Text);

                if (_msd.ControlPositionTrusted())
                {
                    location = new Location(_msd.GetLatitudeDD(), _msd.GetLongitudeDD());
                    Pushpin pin = new Pushpin() {
                                        Heading = -45,
                                        Background = new SolidColorBrush(Colors.Red),
                                        ContentStringFormat = msd_text_box.Text,
                                        Content = "+",                     
                                        Location = location,
                                    };

                    mapUserControl.Map.Children.Add(pin);
                    mapUserControl.Map.Focus();
                }

                msd_details.Items.Clear();
                msd_details.Items.Add("=======================================");
                msd_details.Items.Add(msd_text_box.Text);
                msd_details.Items.Add("=======================================");
                msd_details.Items.Add(String.Format("Version:\t\t{0}", _msd.version));
                msd_details.Items.Add(String.Format("Msg. Identifier:\t{0}", _msd.msg_identifier));
                msd_details.Items.Add("---------------------------------------------------------------------");
                msd_details.Items.Add(String.Format("Control Byte:\t{0}", _msd.GetControlByteString()));
                msd_details.Items.Add(" - Auto alarm:\t" + _msd.ControlAutoAlarm());
                msd_details.Items.Add(" - Manual alarm:\t" + _msd.ControlManualAlarm());
                msd_details.Items.Add(" - Test call:\t" + _msd.ControlTestCall());
                msd_details.Items.Add(" - Position trusted:\t" + _msd.ControlPositionTrusted());
                msd_details.Items.Add("---------------------------------------------------------------------");
                msd_details.Items.Add(String.Format("Vehicle Class:\t{0}", _msd.GetVehicleClass()));
                msd_details.Items.Add(String.Format("VIN:\t\t{0}", _msd.VIN));
                msd_details.Items.Add(String.Format("Fuel Type:\t{0}", _msd.GetFuelType()));
                msd_details.Items.Add("---------------------------------------------------------------------");
                msd_details.Items.Add(String.Format("UTC Seconds:\t{0}", _msd.UTC_sec));

                msd_details.Items.Add(String.Format("UTC Time:\t{0}", _msd.GetTimeStamp()));
                msd_details.Items.Add("---------------------------------------------------------------------");
                msd_details.Items.Add(String.Format("Latitude:\t\t{0}", _msd.latutude));
                msd_details.Items.Add(String.Format("Longitude:\t{0}", _msd.longitude));
                coordinate_idx = msd_details.Items.Count;
                msd_details.Items.Add(String.Format("Coordinates DD:\t{0}, {1}", _msd.GetLatitudeDD(), _msd.GetLongitudeDD()));
                msd_details.Items.Add("---------------------------------------------------------------------");
                msd_details.Items.Add(String.Format("Direction:\t\t{0}°", _msd.direction));
                msd_details.Items.Add("---------------------------------------------------------------------");
                msd_details.Items.Add("Optional Data:");

                /* Old version of MSD optional data encoding */
                if (_msd.version == 1)
                {
                   String[] optional_data = _msd.optional.Split('|');
                    foreach (String data in optional_data)
                        msd_details.Items.Add(" - " + data);
                }  

                /* New version of MSD optional data encoding (*/
                else if (_msd.version == 2)
                {
                    String[] optional_data = _msd.optional.Split('\n');

                    msd_details.Items.Add(String.Format(" - Acceleration [G]:\t{0}", optional_data[0]));
                    msd_details.Items.Add(String.Format(" - Temperature [°C]:\t{0}", optional_data[1]));
                    msd_details.Items.Add(String.Format(" - Passengers:\t{0}", optional_data[2]));
                    msd_details.Items.Add(String.Format(" - Speed:\t\t{0}", optional_data[3]));
                }
 
                msd_details.Items.Add("=======================================");

            }
            else
            {
                msd_details.Items.Clear();
            }
        }

        private void msd_details_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void msd_details_MouseClick(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            { 
                mapUserControl.Map.SetView(DENMARK, ZOOM);
                mapUserControl.Map.Focus();
            }
        }

        private void msd_details_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            // Double clicked on coordinates tap
            if (msd_details.SelectedIndex == coordinate_idx)
            {
                mapUserControl.Map.SetView(location, 18);
                mapUserControl.Map.Focus();
            }
        }

        private void msd_file_MouseClick(object sender, MouseEventArgs e)
        {
            // Right clicked on MSD File
            if (e.Button == MouseButtons.Right)
            {
                MSD_HEX_File_ShowDialog(_msd.GetMSDHexString(msd_text_box.Text));
            }
        }

        private void MSD_HEX_File_ShowDialog(string hex)
        {
            Form prompt = new Form();
            prompt.Width = 580;
            prompt.Height = 180;
            prompt.Text = ".vroom file HEX view";
            prompt.StartPosition = FormStartPosition.CenterScreen; 
            TextBox text = new TextBox() { 
                            ReadOnly = true, 
                            Multiline = true, 
                            Height = prompt.Height, 
                            Width = prompt.Width, 
                            Text = hex, 
                            Font = new System.Drawing.Font(System.Drawing.FontFamily.GenericMonospace, 9) 
            };

            prompt.Controls.Add(text);
            prompt.ShowDialog();
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e) 
        {
            Settings.Default.LastPath = _path;
            Settings.Default.Save();
        }

        private void button1_Click(object sender, EventArgs e)
        {            
            DialogResult result = folderBrowserDialog1.ShowDialog();
            if (result == DialogResult.OK) 
            {
                MSD_File_Watcher.Path = _path = folderBrowserDialog1.SelectedPath + "\\";
                msd_text_box.Items.Clear();
                msd_details.Items.Clear();
                _msd = new MSD_structure();

                LoadFiles();

                path_label.Text = _path;
            } 
        }

        private void MapViewStype_SelectedIndexChanged(object sender, EventArgs e)
        {
            switch (MapViewStyle.SelectedIndex)
            {
                case 0: mapUserControl.Map.Mode = new RoadMode(); break;
                case 1: mapUserControl.Map.Mode = new AerialMode(true); break;
                case 2: mapUserControl.Map.Mode = new AerialMode(false); break;
            }
        }
    }
}
