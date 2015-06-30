using Ntreev.Library.Grid;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Ntreev.Windows.Forms.Grid
{
    public abstract class CellBase : GridObject, ICellBase
    {
        private readonly GrCell cell;

        internal CellBase(GridControl gridControl, GrCell cell)
            : base(gridControl)
        {
            this.cell = cell;
            this.cell.ManagedRef = this;
        }

        internal CellBase(GrCell pCell)
            : base(pCell)
        {
            this.cell = pCell;
            this.cell.ManagedRef = this;
        }

        public void ResetForeColor()
        {
            this.cell.SetForeColor(GrColor.Empty);
        }

        public void ResetBackColor()
        {
            this.cell.SetBackColor(GrColor.Empty);
        }

        public void ResetFont()
        {
            //this.pCell->SetFont(GrFont::Empty);
        }

        public void Invalidate()
        {
            this.cell.Invalidate();
        }

        [Description("셀의 전경색을 가져오거나 설정합니다.")]
        [Category("Appearance")]
        public Color ForeColor
        {
            get { return this.cell.GetForeColor(); }
            set { this.cell.SetForeColor(value); }
        }

        [Description("셀의 배경색을 가져오거나 설정합니다.")]
        [Category("Appearance")]
        public Color BackColor
        {
            get { return this.cell.GetBackColor(); }
            set { this.cell.SetBackColor(value); }
        }

        [Category("Appearance")]
        [AmbientValue((string)null)]
        public Font Font
        {
            get
            {
                GrFont pFont = this.cell.GetPaintingFont();
                return Utility.ToManaged(pFont);
            }
            set
            {
                GrFont pFont = Utility.FromManaged(value);
                this.cell.SetFont(pFont);
            }
        }

#if DEBUG
        [Category("Debug")]
#else
        [Browsable(false)]
#endif
        public bool IsDisplayable
        {
            get { return this.cell.IsDisplayable; }
        }

        [TypeConverter(typeof(StringConverter))]
        [Category("Data")]
        [DefaultValue((string)null)]
        public object Tag
        {
            get;
            set;
        }

#if DEBUG
        [Category("Debug")]
#else
        [Browsable(false)]
#endif
        public Rectangle Bounds
        {
            get { return this.cell.Bounds; }
        }

#if DEBUG
        [Category("Debug")]
#else
        [Browsable(false)]
#endif
        public int X
        {
            get { return this.cell.X; }
        }

#if DEBUG
        [Category("Debug")]
#else
        [Browsable(false)]
#endif
        public int Y
        {
            get { return this.cell.Y; }
        }

#if DEBUG
        [Category("Debug")]
#else
        [Browsable(false)]
#endif
        public virtual int Width
        {
            get { return this.cell.Width; }
            set
            {
                throw new NotImplementedException();
            }
        }

#if DEBUG
        [Category("Debug")]
#else
        [Browsable(false)]
#endif
        public virtual int Height
        {
            get { return this.cell.Height; }
            set
            {
                throw new NotImplementedException();
            }
        }

#if DEBUG
        [Category("Debug")]
#else
        [Browsable(false)]
#endif
        public Point Location
        {
            get { return this.cell.Location; }
        }

#if DEBUG
        [Category("Debug")]
#else
        [Browsable(false)]
#endif
        public Size Size
        {
            get { return this.cell.Size; }
        }

#if DEBUG
        [Category("Debug")]
#else
        [Browsable(false)]
#endif
        public int Left
        {
            get { return this.cell.X; }
        }

#if DEBUG
        [Category("Debug")]
#else
        [Browsable(false)]
#endif
        public int Top
        {
            get { return this.cell.Y; }
        }

#if DEBUG
        [Category("Debug")]
#else
        [Browsable(false)]
#endif
        public int Right
        {
            get { return this.cell.Right; }
        }

#if DEBUG
        [Category("Debug")]
#else
        [Browsable(false)]
#endif
        public int Bottom
        {
            get { return this.cell.Bottom; }
        }

#if DEBUG
        [Category("Debug")]
#else
		[Browsable(false)]
#endif
        public bool Capture
        {
            get { return this.cell.GetMousePressed(); }
        }

#if DEBUG
        [Category("Debug")]
#else
        [Browsable(false)]
#endif
        public Rectangle ClientRectangle
        {
            get { return this.cell.ClientRectangle; }
        }

        [Description("셀의 안쪽 여백을 가져오거나 설정합니다.")]
        [Category("Layout")]
        public Padding Padding
        {
            get { return this.cell.GetPadding(); }
            set { this.cell.SetPadding(value); }
        }



        internal Color DisplayForeColor
        {
            get { return this.cell.GetPaintingForeColor(); }
        }

        internal Color DisplayBackColor
        {
            get { return this.cell.GetPaintingBackColor(); }
        }

        internal GrCell NativeRef
        {
            get { return this.cell; }
        }


        private bool ShouldSerializeForeColor()
        {
            return this.cell.GetForeColorCore() != GrColor.Empty;
        }

        private bool ShouldSerializeBackColor()
        {
            return this.cell.GetBackColorCore() != GrColor.Empty;
        }

        private bool ShouldSerializeFont()
        {
            return this.cell.GetFontCore() != GrFont.Empty;
        }

        private bool ShouldSerializePadding()
        {
            return this.cell.GetPaddingCore() != GrPadding.Empty;
        }

        private void ResetPadding()
        {
            this.cell.SetPadding(GrPadding.Empty);
        }

        Color ICellBase.PaintingForeColor
        {
            get { return this.cell.GetPaintingForeColor(); }
        }

        Color ICellBase.PaintingBackColor
        {
            get { return this.cell.GetPaintingBackColor(); }
        }

        bool ICellBase.Multiline
        {
            get { return this.cell.IsTextMulitiline; }
        }

        bool ICellBase.WordWrap
        {
            get { return this.cell.IsTextWordWrap; }
        }

        StringAlignment ICellBase.Alignment
        {
            get { return (StringAlignment)this.cell.TextHorzAlign; }
        }

        StringAlignment ICellBase.LineAlignment
        {
            get { return (StringAlignment)this.cell.TextVertAlign; }
        }

        CellState ICellBase.State
        {
            get
            {
                GrPaintStyle flag = this.cell.ToPaintStyle();
                return (CellState)flag & CellState.All;
            }
        }
    }
}
