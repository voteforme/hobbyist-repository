namespace SerialTempMonitor
{
    partial class Transmission_detail_term
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
            this.TxInfo = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // TxInfo
            // 
            this.TxInfo.BackColor = System.Drawing.SystemColors.WindowText;
            this.TxInfo.ForeColor = System.Drawing.SystemColors.Info;
            this.TxInfo.Location = new System.Drawing.Point(12, 12);
            this.TxInfo.Multiline = true;
            this.TxInfo.Name = "TxInfo";
            this.TxInfo.Size = new System.Drawing.Size(318, 292);
            this.TxInfo.TabIndex = 0;            
            // 
            // Transmission_detail_term
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(342, 316);
            this.Controls.Add(this.TxInfo);
            this.Name = "Transmission_detail_term";
            this.Text = "Transmission Detail";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        public System.Windows.Forms.TextBox TxInfo;
    }
}