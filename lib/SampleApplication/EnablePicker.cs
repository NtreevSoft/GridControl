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
    public class EnablePicker : CheckBox
    {
        Image[] _bitmaps = new Image[3];

        public EnablePicker()
        {
            LoadResources();
            this.SetStyle(ControlStyles.OptimizedDoubleBuffer, false);
            //this.BackColor = Color.Transparent;
        }

        protected override void OnPaintBackground(PaintEventArgs e)
        {
            int eqwr = 0;
            //e.Graphics.Clear(SystemColors.Control);
        }

        protected override void OnPaint(PaintEventArgs pevent)
        {
            //this.InvokePaintBackground(this, pevent);
            //base.OnPaintBackground(pevent);
            //base.OnPaint(pevent);
            Graphics g = pevent.Graphics;
            
            //g.FillRectangle(Brushes.White, this.DisplayRectangle);
            //g.CompositingMode = System.Drawing.Drawing2D.CompositingMode.SourceCopy;
            //g.FillRectangle(Brushes.Transparent, this.DisplayRectangle);
            //g.Clear(Color.Transparent);
            //using (Brush backBrush = new SolidBrush(this.BackColor))
            //{
            //    g.FillRectangle(backBrush, this.ClientRectangle);
            //}

            //using (Brush backBrush = new SolidBrush(Color.FromArgb(10, Color.Red)))
            //{
            //    g.FillRectangle(backBrush, this.ClientRectangle);
            //}

            DrawBitmap(g);
        }

        private void DrawBitmap(Graphics g)
        {
            Image bitmap = null;
            if (this.Checked == true)
                bitmap = _bitmaps[0];
            else
                bitmap = _bitmaps[1];

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
                "SampleApplication.images.on.png",
                "SampleApplication.images.off.png"};

            int i = 0;
            foreach (string imageFile in imageFiles)
            {
                using (Stream stream = this.GetType().Assembly.GetManifestResourceStream(imageFile))
                _bitmaps[i] = Bitmap.FromStream(stream);
                ++i;
            }
        }
    }
}
