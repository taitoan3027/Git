namespace ADAMConfig
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
            this.components = new System.ComponentModel.Container();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.button1 = new System.Windows.Forms.Button();
            this.dg_tag_config = new System.Windows.Forms.DataGridView();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.label12 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.btn_receive_config = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.tb_receive_fname = new System.Windows.Forms.TextBox();
            this.tb_local_pass = new System.Windows.Forms.TextBox();
            this.tb_local_user = new System.Windows.Forms.TextBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.btn_client_config = new System.Windows.Forms.Button();
            this.label11 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.tb_filename = new System.Windows.Forms.TextBox();
            this.tb_remote_ip = new System.Windows.Forms.TextBox();
            this.tb_remote_pass = new System.Windows.Forms.TextBox();
            this.tb_remote_user = new System.Windows.Forms.TextBox();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.label7 = new System.Windows.Forms.Label();
            this.btn_cmd_config = new System.Windows.Forms.Button();
            this.label6 = new System.Windows.Forms.Label();
            this.dg_cmd_config = new System.Windows.Forms.DataGridView();
            this.cl_cmd_command = new System.Windows.Forms.DataGridViewComboBoxColumn();
            this.cl_cmd_tagname = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.cl_cmd_value = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.label5 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.tb_cmd_re_user = new System.Windows.Forms.TextBox();
            this.tb_cmd_re_pass = new System.Windows.Forms.TextBox();
            this.tb_cmd_re_filename = new System.Windows.Forms.TextBox();
            this.tb_cmd_re_ip = new System.Windows.Forms.TextBox();
            this.tb_cmd_re_path = new System.Windows.Forms.TextBox();
            this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
            this.tb_path = new System.Windows.Forms.TextBox();
            this.btn_browse = new System.Windows.Forms.Button();
            this.label13 = new System.Windows.Forms.Label();
            this.folderBrowserDialog1 = new System.Windows.Forms.FolderBrowserDialog();
            this.cl_Tagname = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.cl_unit = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.cl_scale = new System.Windows.Forms.DataGridViewCheckBoxColumn();
            this.cl_minvalue = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.cl_maxvalue = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.tb_max_day = new System.Windows.Forms.TextBox();
            this.label14 = new System.Windows.Forms.Label();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dg_tag_config)).BeginInit();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.groupBox4.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dg_cmd_config)).BeginInit();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(64)))), ((int)(((byte)(64)))));
            this.groupBox1.Controls.Add(this.button1);
            this.groupBox1.Controls.Add(this.dg_tag_config);
            this.groupBox1.Font = new System.Drawing.Font("Cambria", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.groupBox1.ForeColor = System.Drawing.Color.Yellow;
            this.groupBox1.Location = new System.Drawing.Point(6, 6);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(564, 374);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Tag Configuration";
            this.toolTip1.SetToolTip(this.groupBox1, "Nhập vào danh sách các tag cần đọc");
            // 
            // button1
            // 
            this.button1.Font = new System.Drawing.Font("Cambria", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.button1.ForeColor = System.Drawing.Color.Black;
            this.button1.Location = new System.Drawing.Point(258, 317);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(86, 51);
            this.button1.TabIndex = 1;
            this.button1.Text = "Generate";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // dg_tag_config
            // 
            this.dg_tag_config.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dg_tag_config.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.cl_Tagname,
            this.cl_unit,
            this.cl_scale,
            this.cl_minvalue,
            this.cl_maxvalue});
            this.dg_tag_config.Location = new System.Drawing.Point(6, 28);
            this.dg_tag_config.Name = "dg_tag_config";
            this.dg_tag_config.Size = new System.Drawing.Size(552, 283);
            this.dg_tag_config.TabIndex = 0;
            // 
            // groupBox2
            // 
            this.groupBox2.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(64)))), ((int)(((byte)(64)))));
            this.groupBox2.Controls.Add(this.label12);
            this.groupBox2.Controls.Add(this.label2);
            this.groupBox2.Controls.Add(this.btn_receive_config);
            this.groupBox2.Controls.Add(this.label1);
            this.groupBox2.Controls.Add(this.tb_receive_fname);
            this.groupBox2.Controls.Add(this.tb_local_pass);
            this.groupBox2.Controls.Add(this.tb_local_user);
            this.groupBox2.Font = new System.Drawing.Font("Cambria", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.groupBox2.ForeColor = System.Drawing.Color.Yellow;
            this.groupBox2.Location = new System.Drawing.Point(6, 6);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(235, 372);
            this.groupBox2.TabIndex = 1;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "ADAM FTP Server Configuration";
            this.toolTip1.SetToolTip(this.groupBox2, "Thiết đặt ftp server nhận xử lý file trên ADAM");
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Font = new System.Drawing.Font("Cambria", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label12.Location = new System.Drawing.Point(13, 170);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(63, 15);
            this.label12.TabIndex = 1;
            this.label12.Text = "Filename";
            this.label12.Click += new System.EventHandler(this.label1_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Cambria", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(13, 123);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(99, 15);
            this.label2.TabIndex = 1;
            this.label2.Text = "Local Password";
            this.label2.Click += new System.EventHandler(this.label1_Click);
            // 
            // btn_receive_config
            // 
            this.btn_receive_config.Font = new System.Drawing.Font("Cambria", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_receive_config.ForeColor = System.Drawing.Color.Black;
            this.btn_receive_config.Location = new System.Drawing.Point(80, 301);
            this.btn_receive_config.Name = "btn_receive_config";
            this.btn_receive_config.Size = new System.Drawing.Size(75, 51);
            this.btn_receive_config.TabIndex = 1;
            this.btn_receive_config.Text = "Generate";
            this.btn_receive_config.UseVisualStyleBackColor = true;
            this.btn_receive_config.Click += new System.EventHandler(this.btn_receive_config_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Cambria", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(9, 76);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(103, 15);
            this.label1.TabIndex = 1;
            this.label1.Text = "Local Username";
            this.label1.Click += new System.EventHandler(this.label1_Click);
            // 
            // tb_receive_fname
            // 
            this.tb_receive_fname.Location = new System.Drawing.Point(63, 187);
            this.tb_receive_fname.Name = "tb_receive_fname";
            this.tb_receive_fname.Size = new System.Drawing.Size(153, 26);
            this.tb_receive_fname.TabIndex = 0;
            this.toolTip1.SetToolTip(this.tb_receive_fname, "Tên file viết tắt theo chuẩn. Server chỉ nhận xử lý các file đúng tên như trên (e" +
        "x: HGi_LAMa_NUO123)");
            // 
            // tb_local_pass
            // 
            this.tb_local_pass.Location = new System.Drawing.Point(63, 141);
            this.tb_local_pass.Name = "tb_local_pass";
            this.tb_local_pass.Size = new System.Drawing.Size(153, 26);
            this.tb_local_pass.TabIndex = 0;
            this.toolTip1.SetToolTip(this.tb_local_pass, "Password trên ADAM để nhận file cmd");
            // 
            // tb_local_user
            // 
            this.tb_local_user.Location = new System.Drawing.Point(63, 94);
            this.tb_local_user.Name = "tb_local_user";
            this.tb_local_user.Size = new System.Drawing.Size(153, 26);
            this.tb_local_user.TabIndex = 0;
            this.toolTip1.SetToolTip(this.tb_local_user, "Username trên ADAM để nhận file cmd");
            // 
            // groupBox3
            // 
            this.groupBox3.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(64)))), ((int)(((byte)(64)))));
            this.groupBox3.Controls.Add(this.btn_client_config);
            this.groupBox3.Controls.Add(this.label14);
            this.groupBox3.Controls.Add(this.label11);
            this.groupBox3.Controls.Add(this.label8);
            this.groupBox3.Controls.Add(this.label9);
            this.groupBox3.Controls.Add(this.label10);
            this.groupBox3.Controls.Add(this.tb_max_day);
            this.groupBox3.Controls.Add(this.tb_filename);
            this.groupBox3.Controls.Add(this.tb_remote_ip);
            this.groupBox3.Controls.Add(this.tb_remote_pass);
            this.groupBox3.Controls.Add(this.tb_remote_user);
            this.groupBox3.Font = new System.Drawing.Font("Cambria", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.groupBox3.ForeColor = System.Drawing.Color.Yellow;
            this.groupBox3.Location = new System.Drawing.Point(576, 6);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(241, 373);
            this.groupBox3.TabIndex = 1;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "ADAM FTP Client Configuration";
            this.toolTip1.SetToolTip(this.groupBox3, "Thiết đặt các thông số của ftp server trên máy tính");
            // 
            // btn_client_config
            // 
            this.btn_client_config.Font = new System.Drawing.Font("Cambria", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_client_config.ForeColor = System.Drawing.Color.Black;
            this.btn_client_config.Location = new System.Drawing.Point(87, 310);
            this.btn_client_config.Name = "btn_client_config";
            this.btn_client_config.Size = new System.Drawing.Size(85, 51);
            this.btn_client_config.TabIndex = 5;
            this.btn_client_config.Text = "Generate";
            this.btn_client_config.UseVisualStyleBackColor = true;
            this.btn_client_config.Click += new System.EventHandler(this.btn_client_config_Click);
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Font = new System.Drawing.Font("Cambria", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label11.Location = new System.Drawing.Point(7, 187);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(66, 15);
            this.label11.TabIndex = 5;
            this.label11.Text = "File name";
            this.label11.Click += new System.EventHandler(this.label11_Click);
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Font = new System.Drawing.Font("Cambria", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label8.Location = new System.Drawing.Point(7, 140);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(122, 15);
            this.label8.TabIndex = 5;
            this.label8.Text = "Remote IP Address";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Font = new System.Drawing.Font("Cambria", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label9.Location = new System.Drawing.Point(7, 93);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(115, 15);
            this.label9.TabIndex = 6;
            this.label9.Text = "Remote Password";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Font = new System.Drawing.Font("Cambria", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label10.Location = new System.Drawing.Point(7, 46);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(119, 15);
            this.label10.TabIndex = 7;
            this.label10.Text = "Remote Username";
            // 
            // tb_filename
            // 
            this.tb_filename.Location = new System.Drawing.Point(69, 205);
            this.tb_filename.Name = "tb_filename";
            this.tb_filename.Size = new System.Drawing.Size(154, 26);
            this.tb_filename.TabIndex = 4;
            this.toolTip1.SetToolTip(this.tb_filename, "Tên trạm theo quy định (ex: HGi_LAMa_NUO123)");
            // 
            // tb_remote_ip
            // 
            this.tb_remote_ip.Location = new System.Drawing.Point(69, 158);
            this.tb_remote_ip.Name = "tb_remote_ip";
            this.tb_remote_ip.Size = new System.Drawing.Size(154, 26);
            this.tb_remote_ip.TabIndex = 3;
            this.toolTip1.SetToolTip(this.tb_remote_ip, "Ip của máy tính server");
            // 
            // tb_remote_pass
            // 
            this.tb_remote_pass.Location = new System.Drawing.Point(69, 111);
            this.tb_remote_pass.Name = "tb_remote_pass";
            this.tb_remote_pass.Size = new System.Drawing.Size(154, 26);
            this.tb_remote_pass.TabIndex = 2;
            this.toolTip1.SetToolTip(this.tb_remote_pass, "Password tương ứng với username trên");
            // 
            // tb_remote_user
            // 
            this.tb_remote_user.Location = new System.Drawing.Point(69, 64);
            this.tb_remote_user.Name = "tb_remote_user";
            this.tb_remote_user.Size = new System.Drawing.Size(154, 26);
            this.tb_remote_user.TabIndex = 1;
            this.toolTip1.SetToolTip(this.tb_remote_user, "Username của FTP server trên máy tính ");
            // 
            // tabControl1
            // 
            this.tabControl1.Appearance = System.Windows.Forms.TabAppearance.Buttons;
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tabControl1.Location = new System.Drawing.Point(12, 12);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(841, 411);
            this.tabControl1.TabIndex = 2;
            // 
            // tabPage1
            // 
            this.tabPage1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(64)))), ((int)(((byte)(64)))));
            this.tabPage1.Controls.Add(this.groupBox1);
            this.tabPage1.Controls.Add(this.groupBox3);
            this.tabPage1.Location = new System.Drawing.Point(4, 28);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(833, 379);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "Send Setting";
            // 
            // tabPage2
            // 
            this.tabPage2.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(64)))), ((int)(((byte)(64)))));
            this.tabPage2.Controls.Add(this.groupBox4);
            this.tabPage2.Controls.Add(this.groupBox2);
            this.tabPage2.Location = new System.Drawing.Point(4, 28);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(833, 379);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "Receive Setting";
            // 
            // groupBox4
            // 
            this.groupBox4.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(64)))), ((int)(((byte)(64)))));
            this.groupBox4.Controls.Add(this.label7);
            this.groupBox4.Controls.Add(this.btn_cmd_config);
            this.groupBox4.Controls.Add(this.label6);
            this.groupBox4.Controls.Add(this.dg_cmd_config);
            this.groupBox4.Controls.Add(this.label5);
            this.groupBox4.Controls.Add(this.label3);
            this.groupBox4.Controls.Add(this.label4);
            this.groupBox4.Controls.Add(this.tb_cmd_re_user);
            this.groupBox4.Controls.Add(this.tb_cmd_re_pass);
            this.groupBox4.Controls.Add(this.tb_cmd_re_filename);
            this.groupBox4.Controls.Add(this.tb_cmd_re_ip);
            this.groupBox4.Controls.Add(this.tb_cmd_re_path);
            this.groupBox4.Font = new System.Drawing.Font("Cambria", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.groupBox4.ForeColor = System.Drawing.Color.Yellow;
            this.groupBox4.Location = new System.Drawing.Point(247, 6);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(580, 372);
            this.groupBox4.TabIndex = 2;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Cmd Configuration";
            this.toolTip1.SetToolTip(this.groupBox4, "Tạo file lệnh cmd ");
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Font = new System.Drawing.Font("Cambria", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label7.Location = new System.Drawing.Point(371, 216);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(63, 15);
            this.label7.TabIndex = 8;
            this.label7.Text = "Filename";
            // 
            // btn_cmd_config
            // 
            this.btn_cmd_config.Font = new System.Drawing.Font("Cambria", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_cmd_config.ForeColor = System.Drawing.Color.Black;
            this.btn_cmd_config.Location = new System.Drawing.Point(438, 301);
            this.btn_cmd_config.Name = "btn_cmd_config";
            this.btn_cmd_config.Size = new System.Drawing.Size(75, 51);
            this.btn_cmd_config.TabIndex = 1;
            this.btn_cmd_config.Text = "Generate";
            this.btn_cmd_config.UseVisualStyleBackColor = true;
            this.btn_cmd_config.Click += new System.EventHandler(this.btn_cmd_config_Click);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Font = new System.Drawing.Font("Cambria", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label6.Location = new System.Drawing.Point(371, 169);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(193, 15);
            this.label6.TabIndex = 9;
            this.label6.Text = "Remote Response Folder name";
            // 
            // dg_cmd_config
            // 
            this.dg_cmd_config.BackgroundColor = System.Drawing.Color.Gray;
            this.dg_cmd_config.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dg_cmd_config.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.cl_cmd_command,
            this.cl_cmd_tagname,
            this.cl_cmd_value});
            this.dg_cmd_config.GridColor = System.Drawing.Color.Black;
            this.dg_cmd_config.Location = new System.Drawing.Point(6, 25);
            this.dg_cmd_config.Name = "dg_cmd_config";
            this.dg_cmd_config.Size = new System.Drawing.Size(359, 324);
            this.dg_cmd_config.TabIndex = 0;
            // 
            // cl_cmd_command
            // 
            this.cl_cmd_command.HeaderText = "Command";
            this.cl_cmd_command.Items.AddRange(new object[] {
            "Set Digital Out",
            "Reset Digital Out",
            "Read Digital Value",
            "Read Analog Value",
            "Set Analog Value"});
            this.cl_cmd_command.Name = "cl_cmd_command";
            this.cl_cmd_command.Resizable = System.Windows.Forms.DataGridViewTriState.True;
            this.cl_cmd_command.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.Automatic;
            this.cl_cmd_command.Width = 150;
            // 
            // cl_cmd_tagname
            // 
            this.cl_cmd_tagname.HeaderText = "Tagname";
            this.cl_cmd_tagname.Name = "cl_cmd_tagname";
            // 
            // cl_cmd_value
            // 
            this.cl_cmd_value.HeaderText = "Value";
            this.cl_cmd_value.Name = "cl_cmd_value";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("Cambria", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label5.Location = new System.Drawing.Point(371, 123);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(122, 15);
            this.label5.TabIndex = 10;
            this.label5.Text = "Remote IP Address";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Cambria", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.Location = new System.Drawing.Point(371, 29);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(119, 15);
            this.label3.TabIndex = 12;
            this.label3.Text = "Remote Username";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Cambria", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.Location = new System.Drawing.Point(371, 76);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(115, 15);
            this.label4.TabIndex = 11;
            this.label4.Text = "Remote Password";
            // 
            // tb_cmd_re_user
            // 
            this.tb_cmd_re_user.Location = new System.Drawing.Point(415, 47);
            this.tb_cmd_re_user.Name = "tb_cmd_re_user";
            this.tb_cmd_re_user.Size = new System.Drawing.Size(149, 26);
            this.tb_cmd_re_user.TabIndex = 7;
            this.toolTip1.SetToolTip(this.tb_cmd_re_user, "Username của ftp server trên máy tính muốn nhận response");
            // 
            // tb_cmd_re_pass
            // 
            this.tb_cmd_re_pass.Location = new System.Drawing.Point(415, 94);
            this.tb_cmd_re_pass.Name = "tb_cmd_re_pass";
            this.tb_cmd_re_pass.Size = new System.Drawing.Size(149, 26);
            this.tb_cmd_re_pass.TabIndex = 6;
            this.toolTip1.SetToolTip(this.tb_cmd_re_pass, "Password của ftp server trên máy tính muốn nhận response");
            // 
            // tb_cmd_re_filename
            // 
            this.tb_cmd_re_filename.Location = new System.Drawing.Point(415, 234);
            this.tb_cmd_re_filename.Name = "tb_cmd_re_filename";
            this.tb_cmd_re_filename.Size = new System.Drawing.Size(149, 26);
            this.tb_cmd_re_filename.TabIndex = 3;
            this.toolTip1.SetToolTip(this.tb_cmd_re_filename, "Tên file viết tắt theo chuẩn. Chương trình sẽ tạo file cmd và file response theo " +
        "tên như trên (ex: HGi_LAMa_NUO123)");
            // 
            // tb_cmd_re_ip
            // 
            this.tb_cmd_re_ip.Location = new System.Drawing.Point(415, 141);
            this.tb_cmd_re_ip.Name = "tb_cmd_re_ip";
            this.tb_cmd_re_ip.Size = new System.Drawing.Size(149, 26);
            this.tb_cmd_re_ip.TabIndex = 5;
            this.toolTip1.SetToolTip(this.tb_cmd_re_ip, "Địa chỉ ip của máy tính muốn nhận response");
            // 
            // tb_cmd_re_path
            // 
            this.tb_cmd_re_path.Location = new System.Drawing.Point(415, 187);
            this.tb_cmd_re_path.Name = "tb_cmd_re_path";
            this.tb_cmd_re_path.Size = new System.Drawing.Size(149, 26);
            this.tb_cmd_re_path.TabIndex = 4;
            this.toolTip1.SetToolTip(this.tb_cmd_re_path, "Tên thư mục chứa file response (cần được tạo trước trên máy tính muốn nhận)");
            // 
            // tb_path
            // 
            this.tb_path.Font = new System.Drawing.Font("Cambria", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tb_path.Location = new System.Drawing.Point(196, 444);
            this.tb_path.Multiline = true;
            this.tb_path.Name = "tb_path";
            this.tb_path.Size = new System.Drawing.Size(404, 27);
            this.tb_path.TabIndex = 3;
            // 
            // btn_browse
            // 
            this.btn_browse.Font = new System.Drawing.Font("Cambria", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_browse.Location = new System.Drawing.Point(701, 434);
            this.btn_browse.Name = "btn_browse";
            this.btn_browse.Size = new System.Drawing.Size(75, 40);
            this.btn_browse.TabIndex = 4;
            this.btn_browse.Text = "Browse ...";
            this.btn_browse.UseVisualStyleBackColor = true;
            this.btn_browse.Click += new System.EventHandler(this.btn_browse_Click);
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Font = new System.Drawing.Font("Cambria", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label13.ForeColor = System.Drawing.Color.Yellow;
            this.label13.Location = new System.Drawing.Point(99, 447);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(66, 15);
            this.label13.TabIndex = 5;
            this.label13.Text = "Save Path";
            // 
            // cl_Tagname
            // 
            this.cl_Tagname.HeaderText = "Tag Name";
            this.cl_Tagname.Name = "cl_Tagname";
            // 
            // cl_unit
            // 
            this.cl_unit.HeaderText = "Unit";
            this.cl_unit.Name = "cl_unit";
            this.cl_unit.ToolTipText = "Unit có thể để trống.";
            // 
            // cl_scale
            // 
            this.cl_scale.HeaderText = "Scale";
            this.cl_scale.Name = "cl_scale";
            this.cl_scale.ToolTipText = "Tick vào nếu dùng scale";
            // 
            // cl_minvalue
            // 
            this.cl_minvalue.HeaderText = "Min Value";
            this.cl_minvalue.Name = "cl_minvalue";
            this.cl_minvalue.Resizable = System.Windows.Forms.DataGridViewTriState.True;
            this.cl_minvalue.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // cl_maxvalue
            // 
            this.cl_maxvalue.HeaderText = "Max Value";
            this.cl_maxvalue.Name = "cl_maxvalue";
            this.cl_maxvalue.Resizable = System.Windows.Forms.DataGridViewTriState.True;
            this.cl_maxvalue.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // tb_max_day
            // 
            this.tb_max_day.Location = new System.Drawing.Point(69, 252);
            this.tb_max_day.Name = "tb_max_day";
            this.tb_max_day.Size = new System.Drawing.Size(154, 26);
            this.tb_max_day.TabIndex = 4;
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Font = new System.Drawing.Font("Cambria", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label14.Location = new System.Drawing.Point(7, 234);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(57, 15);
            this.label14.TabIndex = 5;
            this.label14.Text = "Max day";
            this.label14.Click += new System.EventHandler(this.label11_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(64)))), ((int)(((byte)(64)))));
            this.ClientSize = new System.Drawing.Size(856, 486);
            this.Controls.Add(this.label13);
            this.Controls.Add(this.btn_browse);
            this.Controls.Add(this.tb_path);
            this.Controls.Add(this.tabControl1);
            this.Name = "Form1";
            this.Text = "ADAM Configuration";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.groupBox1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.dg_tag_config)).EndInit();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage2.ResumeLayout(false);
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dg_cmd_config)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox tb_local_pass;
        private System.Windows.Forms.TextBox tb_local_user;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.DataGridView dg_tag_config;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.TextBox tb_filename;
        private System.Windows.Forms.TextBox tb_remote_ip;
        private System.Windows.Forms.TextBox tb_remote_pass;
        private System.Windows.Forms.TextBox tb_remote_user;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.Button btn_client_config;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.Button btn_cmd_config;
        private System.Windows.Forms.DataGridView dg_cmd_config;
        private System.Windows.Forms.Button btn_receive_config;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox tb_cmd_re_user;
        private System.Windows.Forms.TextBox tb_cmd_re_pass;
        private System.Windows.Forms.TextBox tb_cmd_re_filename;
        private System.Windows.Forms.TextBox tb_cmd_re_ip;
        private System.Windows.Forms.TextBox tb_cmd_re_path;
        private System.Windows.Forms.ToolTip toolTip1;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.TextBox tb_receive_fname;
        private System.Windows.Forms.DataGridViewComboBoxColumn cl_cmd_command;
        private System.Windows.Forms.DataGridViewTextBoxColumn cl_cmd_tagname;
        private System.Windows.Forms.DataGridViewTextBoxColumn cl_cmd_value;
        private System.Windows.Forms.TextBox tb_path;
        private System.Windows.Forms.Button btn_browse;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.FolderBrowserDialog folderBrowserDialog1;
        private System.Windows.Forms.DataGridViewTextBoxColumn cl_Tagname;
        private System.Windows.Forms.DataGridViewTextBoxColumn cl_unit;
        private System.Windows.Forms.DataGridViewCheckBoxColumn cl_scale;
        private System.Windows.Forms.DataGridViewTextBoxColumn cl_minvalue;
        private System.Windows.Forms.DataGridViewTextBoxColumn cl_maxvalue;
        private System.Windows.Forms.TextBox tb_max_day;
        private System.Windows.Forms.Label label14;
    }
}

