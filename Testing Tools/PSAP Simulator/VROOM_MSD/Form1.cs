﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using VROOM_MSD.Properties;

namespace VROOM_MSD
{
    public partial class Form1 : Form
    {
        private string _path;

        public Form1()
        {
            InitializeComponent();
            _path = Settings.Default.LastPath;
            folderBrowserDialog1.SelectedPath = _path;
            DialogResult result = folderBrowserDialog1.ShowDialog();
            if (result == DialogResult.OK) 
            {
                MSD_File_Watcher.Path = _path = folderBrowserDialog1.SelectedPath + "\\";
            } 
            else
                MSD_File_Watcher.Path = _path;

            String[] items = Directory.GetFiles(MSD_File_Watcher.Path, MSD_File_Watcher.Filter);

            foreach (String file in items)
            {
                msd_text_box.Items.Add(Path.GetFileName(file));
            }
        }
        
        private void MSD_File_Watcher_Changed(object sender, System.IO.FileSystemEventArgs e)
        {

                msd_text_box.Items.Add(e.Name);
        }

        private void MSD_File_Watcher_Deleted(object sender, System.IO.FileSystemEventArgs e)
        {
            msd_text_box.Items.Remove(e.Name);
        }

        private Byte[] ReadSelectedFile(String path)
        {

            FileStream msd = new FileStream(path, FileMode.Open);
            byte[] data = new byte[msd.Length];
            msd.Read(data, 0, (int)msd.Length);
            msd.Close();

            return data;
        }

        private void msd_file_box_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (msd_text_box.SelectedItem != null) 
            {
               var MSD = ReadSelectedFile(_path + msd_text_box.Text);

               StringBuilder sb = new StringBuilder();
               String control, VIN, IPV4, optional;
               UInt32 UTC = 0;
               Int32 latutude = 0, longitude = 0;
               Byte direction;

               int index;
               int offset = 0;

               control = Convert.ToString(MSD[0], 2);
               offset = 1;

               for (index = 0; index < 20; index++)
                   sb.Append(Convert.ToChar(MSD[(index + offset)]));
               VIN = sb.ToString();
               sb.Clear();
               offset += 20;

               for (index = 3; index >= 0; index--)
               {
                   UTC = BitConverter.ToUInt32(MSD, (index + offset));
                   latutude = BitConverter.ToInt32(MSD, (index + offset + 4));
                   longitude = BitConverter.ToInt32(MSD, (index + offset + 8));
               }

               offset += 12;

               direction = MSD[offset];
               offset += 1;

               IPV4 = MSD[offset].ToString() + '.' + MSD[offset + 1].ToString() + '.' + MSD[offset + 2].ToString() + '.' + MSD[offset + 3].ToString();
               offset += 4;

               for (index = 0; index < 102; index++)
               {
                   if (Convert.ToChar(MSD[(index + offset)]) == '|')
                       sb.Append("\n");
                   else
                       sb.Append(Convert.ToChar(MSD[(index + offset)]));
               }
               optional = sb.ToString();
               sb.Clear();

               // msd_text_box.SelectedItem.ForeColor = System.Drawing.Color.Black;
               //(msd_text_box.SelectedItem.Text as Control).ForeColor = Color.Yellow;

               msd_details.Items.Clear();
               msd_details.Items.Add("----------------------------------------------------------------------");
               msd_details.Items.Add(msd_text_box.Text);
               msd_details.Items.Add("----------------------------------------------------------------------");
               msd_details.Items.Add(String.Format("Control:\t\t{0}", control));
               msd_details.Items.Add(String.Format("VIN:\t\t{0}", VIN));
               msd_details.Items.Add(String.Format("UTC:\t\t{0}", UTC));
               msd_details.Items.Add(String.Format("Latitude:\t\t{0}", latutude));
               msd_details.Items.Add(String.Format("Longitude:\t{0}", longitude));
               msd_details.Items.Add(String.Format("Direction:\t\t{0}°", direction));
               msd_details.Items.Add(String.Format("Service Provider:\t{0}", IPV4));
               msd_details.Items.Add(String.Format("Optional Data:\t{0}", optional));
               msd_details.Items.Add("----------------------------------------------------------------------");
            }
            else
            {
                msd_details.Items.Clear();
            }
        }
        private void msd_details_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e) 
        {
            Settings.Default.LastPath = _path;
            Settings.Default.Save();
        }
    }
}