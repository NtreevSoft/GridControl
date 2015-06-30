using Ntreev.Library.Grid;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Linq;
using System.Text;

namespace Ntreev.Windows.Forms.Grid
{
    [TypeConverter(typeof(ExpandableObjectConverter))]
    public class CaptionRow : CellBase
    {
        private readonly GrCaption caption;

        internal CaptionRow(GridControl gridControl, GrCaption caption)
            : base(gridControl, caption)
        {
            this.caption = caption;
        }

        public override string ToString()
        {
            return this.Text;
        }

        [Category("Layout")]
        public string Text
        {
            get { return this.caption.Text; }
            set { this.caption.Text = value; }
        }

        [Category("Layout")]
        [DefaultValue(StringAlignment.Near)]
        public StringAlignment Alignment
        {
            get { return (StringAlignment)this.caption.TextHorzAlign; }
            set { this.caption.SetTextHorzAlign((GrHorzAlign)value); }
        }

        [Category("Layout")]
        [DefaultValue(StringAlignment.Center)]
        public StringAlignment LineAlignment
        {
            get { return (StringAlignment)this.caption.TextVertAlign; }
            set { this.caption.SetTextVertAlign((GrVertAlign)value); }
        }

        [Category("Appearance")]
        [DefaultValue(true)]
        public bool IsVisible
        {
            get { return this.caption.IsVisible; }
            set { this.caption.IsVisible = value; }
        }

        [Description("제목의 높이입니다.")]
        [Category("Layout")]
        [SettingsBindable(true)]
        public override int Height
        {
            get { return this.caption.Height; }
            set
            {
                if (value < 0)
                    throw new ArgumentOutOfRangeException("value");
                this.caption.Height = value;
            }
        }

        private bool ShouldSerializeHeight()
        {
            return this.caption.Height != this.caption.GetMinHeight();
        }

        private bool ShouldSerializeText()
        {
            return false;
        }
    }
}
