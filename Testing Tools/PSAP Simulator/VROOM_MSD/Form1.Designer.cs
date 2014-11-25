namespace VROOM_MSD
{
    partial class Form1
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
            this.MSD_File_Watcher = new System.IO.FileSystemWatcher();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.msd_text_box = new System.Windows.Forms.ListBox();
            this.msd_details = new System.Windows.Forms.ListBox();
            this.folderBrowserDialog1 = new System.Windows.Forms.FolderBrowserDialog();
            this.path_label = new System.Windows.Forms.Label();
            this.button1 = new System.Windows.Forms.Button();
            this.web_browser = new System.Windows.Forms.WebBrowser();
            ((System.ComponentModel.ISupportInitialize)(this.MSD_File_Watcher)).BeginInit();
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
            this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(512, 11);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(125, 29);
            this.label1.TabIndex = 2;
            this.label1.Text = "MSD Files";
            // 
            // label2
            // 
            this.label2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(139, 11);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(146, 29);
            this.label2.TabIndex = 3;
            this.label2.Text = "MSD Details";
            // 
            // msd_text_box
            // 
            this.msd_text_box.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.msd_text_box.ForeColor = System.Drawing.Color.DarkRed;
            this.msd_text_box.FormattingEnabled = true;
            this.msd_text_box.Location = new System.Drawing.Point(470, 48);
            this.msd_text_box.Name = "msd_text_box";
            this.msd_text_box.ScrollAlwaysVisible = true;
            this.msd_text_box.Size = new System.Drawing.Size(216, 277);
            this.msd_text_box.TabIndex = 5;
            this.msd_text_box.SelectedIndexChanged += new System.EventHandler(this.msd_file_box_SelectedIndexChanged);
            // 
            // msd_details
            // 
            this.msd_details.AccessibleRole = System.Windows.Forms.AccessibleRole.Alert;
            this.msd_details.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.msd_details.FormattingEnabled = true;
            this.msd_details.Location = new System.Drawing.Point(27, 48);
            this.msd_details.Name = "msd_details";
            this.msd_details.ScrollAlwaysVisible = true;
            this.msd_details.Size = new System.Drawing.Size(405, 316);
            this.msd_details.TabIndex = 12;
            this.msd_details.SelectedIndexChanged += new System.EventHandler(this.msd_details_SelectedIndexChanged);
            this.msd_details.MouseDown += new System.Windows.Forms.MouseEventHandler(this.msd_details_MouseClick);
            // 
            // path_label
            // 
            this.path_label.AutoSize = true;
            this.path_label.Location = new System.Drawing.Point(24, 377);
            this.path_label.Name = "path_label";
            this.path_label.Size = new System.Drawing.Size(80, 13);
            this.path_label.TabIndex = 13;
            this.path_label.Text = "MSD path label";
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(470, 341);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(216, 23);
            this.button1.TabIndex = 14;
            this.button1.Text = "MSD path";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // web_browser
            // 
            this.web_browser.Location = new System.Drawing.Point(27, 48);
            this.web_browser.MinimumSize = new System.Drawing.Size(20, 20);
            this.web_browser.Name = "web_browser";
            this.web_browser.Size = new System.Drawing.Size(405, 316);
            this.web_browser.TabIndex = 15;
            this.web_browser.Visible = false;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(720, 415);
            this.Controls.Add(this.web_browser);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.path_label);
            this.Controls.Add(this.msd_details);
            this.Controls.Add(this.msd_text_box);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.MinimumSize = new System.Drawing.Size(720, 282);
            this.Name = "Form1";
            this.Text = "PSAP Simulation GUI";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            ((System.ComponentModel.ISupportInitialize)(this.MSD_File_Watcher)).EndInit();
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
        private System.Windows.Forms.WebBrowser web_browser;


    }
}

