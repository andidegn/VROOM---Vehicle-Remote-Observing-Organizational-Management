namespace VROOM_MSD
{
    partial class MainForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.MSD_File_Watcher = new System.IO.FileSystemWatcher();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.msd_text_box = new System.Windows.Forms.ListBox();
            this.msd_details = new System.Windows.Forms.ListBox();
            this.folderBrowserDialog1 = new System.Windows.Forms.FolderBrowserDialog();
            this.path_label = new System.Windows.Forms.Label();
            this.button1 = new System.Windows.Forms.Button();
            this.fileSystemWatcher1 = new System.IO.FileSystemWatcher();
            this.MapViewStyle = new System.Windows.Forms.ComboBox();
            this.vroom_logo = new System.Windows.Forms.PictureBox();
            this.elementHost = new System.Windows.Forms.Integration.ElementHost();
            this.mapUserControl = new VROOM_MSD.MapUserControl();
            ((System.ComponentModel.ISupportInitialize)(this.MSD_File_Watcher)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.fileSystemWatcher1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.vroom_logo)).BeginInit();
            this.SuspendLayout();
            // 
            // MSD_File_Watcher
            // 
            this.MSD_File_Watcher.EnableRaisingEvents = true;
            this.MSD_File_Watcher.Filter = "*.vroom";
            this.MSD_File_Watcher.SynchronizingObject = this;
            this.MSD_File_Watcher.Created += new System.IO.FileSystemEventHandler(this.MSD_File_Watcher_Changed);
            this.MSD_File_Watcher.Deleted += new System.IO.FileSystemEventHandler(this.MSD_File_Watcher_Deleted);
            // 
            // label1
            // 
            resources.ApplyResources(this.label1, "label1");
            this.label1.Name = "label1";
            // 
            // label2
            // 
            resources.ApplyResources(this.label2, "label2");
            this.label2.Name = "label2";
            // 
            // msd_text_box
            // 
            resources.ApplyResources(this.msd_text_box, "msd_text_box");
            this.msd_text_box.ForeColor = System.Drawing.Color.DarkRed;
            this.msd_text_box.FormattingEnabled = true;
            this.msd_text_box.Name = "msd_text_box";
            this.msd_text_box.SelectedIndexChanged += new System.EventHandler(this.msd_file_box_SelectedIndexChanged);
            this.msd_text_box.MouseDown += new System.Windows.Forms.MouseEventHandler(this.msd_file_MouseClick);
            // 
            // msd_details
            // 
            this.msd_details.AccessibleRole = System.Windows.Forms.AccessibleRole.Alert;
            resources.ApplyResources(this.msd_details, "msd_details");
            this.msd_details.FormattingEnabled = true;
            this.msd_details.Name = "msd_details";
            this.msd_details.SelectedIndexChanged += new System.EventHandler(this.msd_details_SelectedIndexChanged);
            this.msd_details.MouseDown += new System.Windows.Forms.MouseEventHandler(this.msd_details_MouseClick);
            this.msd_details.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.msd_details_MouseDoubleClick);
            // 
            // path_label
            // 
            resources.ApplyResources(this.path_label, "path_label");
            this.path_label.Name = "path_label";
            // 
            // button1
            // 
            resources.ApplyResources(this.button1, "button1");
            this.button1.Name = "button1";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // fileSystemWatcher1
            // 
            this.fileSystemWatcher1.EnableRaisingEvents = true;
            this.fileSystemWatcher1.SynchronizingObject = this;
            // 
            // MapViewStyle
            // 
            resources.ApplyResources(this.MapViewStyle, "MapViewStyle");
            this.MapViewStyle.BackColor = System.Drawing.SystemColors.ControlLight;
            this.MapViewStyle.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.MapViewStyle.FormattingEnabled = true;
            this.MapViewStyle.Items.AddRange(new object[] {
            resources.GetString("MapViewStyle.Items"),
            resources.GetString("MapViewStyle.Items1"),
            resources.GetString("MapViewStyle.Items2")});
            this.MapViewStyle.Name = "MapViewStyle";
            this.MapViewStyle.SelectedIndexChanged += new System.EventHandler(this.MapViewStype_SelectedIndexChanged);
            // 
            // vroom_logo
            // 
            resources.ApplyResources(this.vroom_logo, "vroom_logo");
            this.vroom_logo.Name = "vroom_logo";
            this.vroom_logo.TabStop = false;
            // 
            // elementHost
            // 
            resources.ApplyResources(this.elementHost, "elementHost");
            this.elementHost.BackColorTransparent = true;
            this.elementHost.Cursor = System.Windows.Forms.Cursors.Cross;
            this.elementHost.ForeColor = System.Drawing.SystemColors.ButtonFace;
            this.elementHost.Name = "elementHost";
            this.elementHost.Child = this.mapUserControl;
            // 
            // MainForm
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.vroom_logo);
            this.Controls.Add(this.MapViewStyle);
            this.Controls.Add(this.elementHost);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.path_label);
            this.Controls.Add(this.msd_details);
            this.Controls.Add(this.msd_text_box);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.HelpButton = true;
            this.Name = "MainForm";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            ((System.ComponentModel.ISupportInitialize)(this.MSD_File_Watcher)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.fileSystemWatcher1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.vroom_logo)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.IO.FileSystemWatcher MSD_File_Watcher;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ListBox msd_text_box;
        private System.Windows.Forms.ListBox msd_details;
        public System.Windows.Forms.FolderBrowserDialog folderBrowserDialog1;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Label path_label;
        private System.IO.FileSystemWatcher fileSystemWatcher1;
        private System.Windows.Forms.Integration.ElementHost elementHost;
        private System.Windows.Forms.ComboBox MapViewStyle;
        private System.Windows.Forms.PictureBox vroom_logo;
        private MapUserControl mapUserControl;
    }
}

