using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SampleApplication
{
    public class DataLocationPicker : CheckBox
    {
        private DataLocation _dataLocation;
        Image[] _bitmaps = new Image[3];

        public DataLocationPicker()
        {
            this.ThreeState = true;
            this.CheckState = System.Windows.Forms.CheckState.Indeterminate;

            LoadResources();
            this.SetStyle(ControlStyles.OptimizedDoubleBuffer, false);
        }

        protected override void OnPaint(PaintEventArgs pevent)
        {
            Graphics g = pevent.Graphics;
            //using (Brush backBrush = new SolidBrush(this.BackColor))
            //{
            //    g.FillRectangle(backBrush, this.ClientRectangle);
            //}

            DrawBitmap(g);

            //base.OnPaint(pevent);
        }

        private void DrawBitmap(Graphics g)
        {
            Image bitmap = null;
            if (this.DataLocation == DataLocation.Both)
                bitmap = _bitmaps[0];
            else if (this.DataLocation == DataLocation.Server)
                bitmap = _bitmaps[1];
            else if (this.DataLocation == DataLocation.Client)
                bitmap = _bitmaps[2];

            if (bitmap == null)
                return;

            int x = (this.Width - (bitmap.Width)) / 2;
            int y = (this.Height - (bitmap.Height)) / 2;
            int width = bitmap.Width;
            int height = bitmap.Height;

            g.DrawImage(bitmap, x, y, width, height);
        }

        private void LoadResources()
        {
            string[] imageFiles = new string[] {
                "SampleApplication.images.both.png", 
                "SampleApplication.images.server.png",               
                "SampleApplication.images.client.png",};

            int i = 0;
            foreach (string imageFile in imageFiles)
            {
                using (Stream stream = this.GetType().Assembly.GetManifestResourceStream(imageFile))
                _bitmaps[i] = Bitmap.FromStream(stream);
                ++i;
            }
        }

        public DataLocation DataLocation
        {
            get { return _dataLocation; }
            set
            {
                _dataLocation = value;
                this.Refresh();
            }
        }

        protected override void OnCheckStateChanged(EventArgs e)
        {
            base.OnCheckStateChanged(e);

            switch (this.CheckState)
            {
                case System.Windows.Forms.CheckState.Indeterminate:
                    this.DataLocation = DataLocation.Both;
                    break;

                case System.Windows.Forms.CheckState.Unchecked:
                    this.DataLocation = DataLocation.Client;
                    break;
                
                case System.Windows.Forms.CheckState.Checked:
                    this.DataLocation = DataLocation.Server;
                    break;
            }
            this.Refresh();
        }
    }
}
