using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;

namespace ADAMConfig
{
    public partial class Form1 : Form
    {
        string str_path = "";
        public Form1()
        {
            InitializeComponent();
            dg_tag_config.ForeColor = Color.Black;
            dg_cmd_config.ForeColor = Color.Black;
            dg_tag_config.Font = new Font("Arial", 10, FontStyle.Bold);
            dg_cmd_config.Font = new Font("Arial", 10, FontStyle.Bold);
            
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void Form1_Load(object sender, EventArgs e)
        {
           
        }

        private void btn_client_config_Click(object sender, EventArgs e)
        {
            if (tb_path.Text != "")
            {
                int temp = 0;
                if (tb_filename.Text != "" && tb_remote_pass.Text != "" && tb_remote_user.Text != "" && tb_remote_ip.Text != "" && tb_max_day.Text != "")
                {
                    if (int.TryParse(tb_max_day.Text, out temp) == true)
                    {
                        string client_config_text = "user," + tb_remote_user.Text + ",\r\npass," + tb_remote_pass.Text + ",\r\nip," + tb_remote_ip.Text + ",\r\nfname," + tb_filename.Text + ",\r\nday," + tb_max_day.Text + ",";

                        System.IO.File.WriteAllText(str_path + @"\client_config.txt", client_config_text);
                        MessageBox.Show("Generate \"client_config.txt\" file completely !!!", "Done", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }
                    else
                    {
                        MessageBox.Show("Pleasle fill a valid value in \"Max day\" text box!!!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                }
                else
                {
                    MessageBox.Show("Pleasle fill all the text box!!!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
            else
            {
                MessageBox.Show("Please browse a path to save file first!!!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
           if(tb_path.Text != "")
           {
                dg_tag_config.Enabled = false;
                if(dg_tag_config.RowCount > 1)
                {
                    string tag_read_config = "";
                    for (int i = 0; i < dg_tag_config.RowCount-1; i++ )
                    {
                        if (dg_tag_config.Rows[i].Cells["cl_tagname"].Value != null && dg_tag_config.Rows[i].Cells["cl_tagname"].Value.ToString() != "")
                        {
                            //read unit
                            string str_unit = "";
                            if (dg_tag_config.Rows[i].Cells["cl_unit"].Value != null && dg_tag_config.Rows[i].Cells["cl_unit"].Value.ToString() != "")
                            {
                                str_unit = dg_tag_config.Rows[i].Cells["cl_unit"].Value.ToString();                           
                            }
                            else
                            {
                                str_unit = "-";
                            }
                            //if non use scale
                            if (dg_tag_config.Rows[i].Cells["cl_scale"].Value == null || dg_tag_config.Rows[i].Cells["cl_scale"].Value.ToString() == "False")
                            {
                                tag_read_config += dg_tag_config.Rows[i].Cells["cl_tagname"].Value.ToString() + "," + str_unit + ",N,\r\n";
                            }
                            //if use scale
                            else
                            {
                                //min value not null
                                if (dg_tag_config.Rows[i].Cells["cl_minvalue"].Value != null && dg_tag_config.Rows[i].Cells["cl_minvalue"].Value.ToString() != "")
                                {
                                    //max value not null
                                    if (dg_tag_config.Rows[i].Cells["cl_maxvalue"].Value != null && dg_tag_config.Rows[i].Cells["cl_maxvalue"].Value.ToString() != "")
                                    {
                                        float temp;
                                        //minvalue valid
                                        if (float.TryParse(dg_tag_config.Rows[i].Cells["cl_minvalue"].Value.ToString(), out temp) == true)
                                        {
                                            //max value valid
                                            if (float.TryParse(dg_tag_config.Rows[i].Cells["cl_maxvalue"].Value.ToString(), out temp) == true)
                                            {
                                                tag_read_config += dg_tag_config.Rows[i].Cells["cl_tagname"].Value.ToString() + "," + str_unit + ",S," + dg_tag_config.Rows[i].Cells["cl_minvalue"].Value.ToString() + "," + dg_tag_config.Rows[i].Cells["cl_maxvalue"].Value.ToString() + ",\r\n";
                                            }
                                            //max value invalid
                                            else
                                            {
                                                MessageBox.Show("The value in \"Max Value\" collumn, row " + (i + 1).ToString() + " is invalid !!!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                                break;
                                            }
                                        }
                                        // min value invalid
                                        else
                                        {
                                            MessageBox.Show("The value in \"Min Value\" collumn, row " + (i + 1).ToString() + " is invalid !!!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                            break;
                                        }
                                    }
                                    //max value null
                                    else
                                    {
                                        MessageBox.Show("Pleasle insert into \"Max Value\" collumn on row " + (i + 1).ToString() + " !!!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                        break;
                                    }
                                }
                                 //min value null
                                else
                                {
                                    MessageBox.Show("Pleasle insert into \"Min Value\" collumn on row " + (i + 1).ToString() + " !!!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                    break;
                                }
                            }
                        }
                        else
                        {
                            MessageBox.Show("Pleasle insert into \"Tag Name\" collumn on row " + (i + 1).ToString() + " !!!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                            break;
                        }
                //       MessageBox.Show(dg_tag_config.Rows[i].Cells["Scale"].Value.ToString(), "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                    Directory.CreateDirectory(str_path + @"\SaveData");
                    System.IO.File.WriteAllText(str_path + @"\SaveData\tag_read_config.txt", tag_read_config);
                    MessageBox.Show("Generate \"tag_read_config.txt\" file completely !!!", "Done", MessageBoxButtons.OK, MessageBoxIcon.Information);

                }
                else
                {
                    MessageBox.Show("Pleasle insert tag information to row!!!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
                dg_tag_config.Enabled = true;

           }
           else
           {
               MessageBox.Show("Please browse a path to save file first!!!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
           }
        }

        private void btn_receive_config_Click(object sender, EventArgs e)
        {
            if (tb_path.Text != "")
            {
                if (tb_local_user.Text != "" && tb_local_pass.Text != "" && tb_receive_fname.Text != "")
                {
                    Directory.CreateDirectory(str_path + @"\CMD_Receive");
                    string server_config_text = "user," + tb_local_user.Text + ",\r\npass," + tb_local_pass.Text + ",\r\nfname," + tb_receive_fname.Text + ",\r\n";
                    System.IO.File.WriteAllText(str_path + @"\CMD_Receive\server_config.txt", server_config_text);
                    MessageBox.Show("Generate \"server_config.txt\" file completely !!!", "Done", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else
                {
                    MessageBox.Show("Pleasle fill all the text box!!!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
            else
            {
                MessageBox.Show("Please browse a path to save file first!!!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void btn_cmd_config_Click(object sender, EventArgs e)
        {
            if (tb_path.Text != "")
            {
                //has cmd in sheet
                if (dg_cmd_config.RowCount > 1)
                {

                    //has remote user
                    if (tb_cmd_re_user.Text != "")
                    {
                        // has remote pass
                        if (tb_cmd_re_pass.Text != "")
                        {
                            //has remote ip
                            if (tb_cmd_re_ip.Text != "")
                            {
                                //has remote path
                                if (tb_cmd_re_path.Text != "")
                                {
                                    //has remote filename
                                    if (tb_cmd_re_filename.Text != "")
                                    {
                                        string cmd_config_text = "";
                                        cmd_config_text += "user," + tb_cmd_re_user.Text + ",\r\npass," + tb_cmd_re_pass.Text + ",\r\nip," + tb_cmd_re_ip.Text + ",\r\npath," + tb_cmd_re_path.Text + ",\r\n";
                                        Dictionary<string, string> dic_cmd = new Dictionary<string, string>();
                                        dic_cmd.Add("Set Digital Out", "DS");
                                        dic_cmd.Add("Reset Digital Out", "DR");
                                        dic_cmd.Add("Read Digital Value", "D");
                                        dic_cmd.Add("Read Analog Value", "A");
                                        dic_cmd.Add("Set Analog Value", "AW");
                                        bool b_read_dg_ok = true;
                                        for (int i = 0; i < dg_cmd_config.RowCount - 1; i++)
                                        {
                                            if (dg_cmd_config.Rows[i].Cells["cl_cmd_tagname"].Value != null && dg_cmd_config.Rows[i].Cells["cl_cmd_tagname"].Value.ToString() != "")
                                            {
                                                if (dg_cmd_config.Rows[i].Cells["cl_cmd_command"].Value != null && dg_cmd_config.Rows[i].Cells["cl_cmd_command"].Value.ToString() != "")
                                                {
                                                    string cmd = dic_cmd[dg_cmd_config.Rows[i].Cells["cl_cmd_command"].Value.ToString()];
                                                    cmd_config_text += cmd + "," + dg_cmd_config.Rows[i].Cells["cl_cmd_tagname"].Value.ToString() + ",\r\n";
                                                }
                                                else
                                                {
                                                    MessageBox.Show("Pleasle Select a command in row " + (i + 1).ToString() + " !!!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                                    b_read_dg_ok = false;
                                                    break;
                                                }
                                            }
                                            else
                                            {
                                                MessageBox.Show("Pleasle insert tagname to row " + (i + 1).ToString() + " !!!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                                b_read_dg_ok = false;
                                                break;
                                            }
                                        }
                                        if (b_read_dg_ok == true)
                                        {
                                            Directory.CreateDirectory(str_path + @"\CMD_Receive");
                                            System.IO.File.WriteAllText(str_path + @"\CMD_Receive\" + tb_cmd_re_filename.Text + "_cmd.txt", cmd_config_text);
                                            MessageBox.Show("Generate \"cmd_config.txt\" file completely !!!", "Done", MessageBoxButtons.OK, MessageBoxIcon.Information);
                                        }
                                    }
                                    // not has remote filename
                                    else
                                    {
                                        MessageBox.Show("Pleasle fill \"Remote Response Filename\"!!!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                    }
                                }
                                // not has remote path
                                else
                                {
                                    MessageBox.Show("Pleasle fill \"Remote Response Folder Name\"!!!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                }
                            }
                            //not has remote ip
                            else
                            {
                                MessageBox.Show("Pleasle fill \"Remote IP Address\"!!!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                            }
                        }
                        // not has remote pass
                        else
                        {
                            MessageBox.Show("Pleasle fill \"Remote password\"!!!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        }
                    }
                    // not has remote user
                    else
                    {
                        MessageBox.Show("Pleasle fill \"Remote username\"!!!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                }
                // not has cmd in sheet
                else
                {
                    MessageBox.Show("Pleasle insert cmd to sheet!!!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
            else
            {
                MessageBox.Show("Please browse a path to save file first!!!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void btn_browse_Click(object sender, EventArgs e)
        {
            folderBrowserDialog1.ShowDialog();
            tb_path.Text = folderBrowserDialog1.SelectedPath.ToString();
            str_path = tb_path.Text;
        }

        private void label11_Click(object sender, EventArgs e)
        {

        }
        
    }
}
