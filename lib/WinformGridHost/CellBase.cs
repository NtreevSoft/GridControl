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
        private GrCell m_pCell;

        internal CellBase(GridControl gridControl, GrCell pCell)
            : base(gridControl)
        {
            m_pCell = pCell;
            m_pCell.ManagedRef = this;
        }

        internal CellBase(GrCell pCell)
            : base(pCell)
        {
            m_pCell.ManagedRef = this;
        }

        public void ResetForeColor()
        {
            m_pCell.SetForeColor(GrColor.Empty);
        }

        public void ResetBackColor()
        {
            m_pCell.SetBackColor(GrColor.Empty);
        }

        public void ResetFont()
        {
            //m_pCell->SetFont(GrFont::Empty);
        }

        public void Invalidate()
        {
            m_pCell.Invalidate();
        }

        [Description("셀의 전경색을 가져오거나 설정합니다.")]
        [Category("Appearance")]
        public Color ForeColor
        {
            get { return m_pCell.GetForeColor(); }
            set { m_pCell.SetForeColor(value); }
        }

        [Description("셀의 배경색을 가져오거나 설정합니다.")]
        [Category("Appearance")]
        public Color BackColor
        {
            get { return m_pCell.GetBackColor(); }
            set { m_pCell.SetBackColor(value); }
        }

        [Category("Appearance")]
        [AmbientValue((string)null)]
        public Font Font
        {
            get { throw new NotImplementedException(); }
            set { throw new NotImplementedException(); }
        }

#if DEBUG
        [Category("Debug")]
#else
        [Browsable(false)]
#endif
        public bool IsDisplayable
        {
            get { return m_pCell.GetDisplayable(); }
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
            get { return m_pCell.GetRect(); }
        }

#if DEBUG
        [Category("Debug")]
#else
        [Browsable(false)]
#endif
        public int X
        {
            get { return m_pCell.GetX(); }
        }

#if DEBUG
        [Category("Debug")]
#else
        [Browsable(false)]
#endif
        public int Y
        {
            get { return m_pCell.GetY(); }
        }

#if DEBUG
        [Category("Debug")]
#else
        [Browsable(false)]
#endif
        public int Width
        {
            get { return m_pCell.GetWidth(); }
        }

#if DEBUG
        [Category("Debug")]
#else
        [Browsable(false)]
#endif
        public virtual int Height
        {
            get { return m_pCell.GetHeight(); }
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
            get { return m_pCell.GetLocation(); }
        }

#if DEBUG
        [Category("Debug")]
#else
        [Browsable(false)]
#endif
        public Size Size
        {
            get { return m_pCell.GetSize(); }
        }

#if DEBUG
        [Category("Debug")]
#else
        [Browsable(false)]
#endif
        public int Left
        {
            get { return m_pCell.GetX(); }
        }

#if DEBUG
        [Category("Debug")]
#else
        [Browsable(false)]
#endif
        public int Top
        {
            get { return m_pCell.GetY(); }
        }

#if DEBUG
        [Category("Debug")]
#else
        [Browsable(false)]
#endif
        public int Right
        {
            get { return m_pCell.GetRight(); }
        }

#if DEBUG
        [Category("Debug")]
#else
        [Browsable(false)]
#endif
        public int Bottom
        {
            get { return m_pCell.GetBottom(); }
        }

#if DEBUG
        [Category("Debug")]
#else
		[Browsable(false)]
#endif
        public bool Capture
        {
            get { return m_pCell.GetMousePressed(); }
        }

#if DEBUG
        [Category("Debug")]
#else
        [Browsable(false)]
#endif
        public Rectangle ClientRectangle
        {
            get { return m_pCell.GetClientRect(); }
        }

        [Description("셀의 안쪽 여백을 가져오거나 설정합니다.")]
        [Category("Layout")]
        public Padding Padding
        {
            get { return m_pCell.GetPadding(); }
            set { m_pCell.SetPadding(value); }
        }



        internal Color DisplayForeColor
        {
            get { return m_pCell.GetPaintingForeColor(); }
        }

        internal Color DisplayBackColor
        {
            get { return m_pCell.GetPaintingBackColor(); }
        }

        internal GrCell NativeRef
        {
            get { return m_pCell; }
        }


        private bool ShouldSerializeForeColor()
        {
            return m_pCell.GetForeColorCore() != GrColor.Empty;
        }

        private bool ShouldSerializeBackColor()
        {
            return m_pCell.GetBackColorCore() != GrColor.Empty;
        }

        private bool ShouldSerializeFont()
        {
            throw new NotImplementedException();
            //return m_pCell.GetFontCore() != GrFont.Empty;
        }

        private bool ShouldSerializePadding()
        {
            return m_pCell.GetPaddingCore() != GrPadding.Empty;
        }

        private void ResetPadding()
        {
            m_pCell.SetPadding(GrPadding.Empty);
        }


        Color ICellBase.PaintingForeColor
        {
            get { return m_pCell.GetPaintingForeColor(); }
        }

        Color ICellBase.PaintingBackColor
        {
            get { return m_pCell.GetPaintingBackColor(); }
        }

        bool ICellBase.Multiline
        {
            get { return m_pCell.GetTextMulitiline(); }
        }

        bool ICellBase.WordWrap
        {
            get { return m_pCell.GetTextWordWrap(); }
        }

        StringAlignment ICellBase.Alignment
        {
            get { return (StringAlignment)m_pCell.GetTextHorzAlign(); }
        }

        StringAlignment ICellBase.LineAlignment
        {
            get { return (StringAlignment)m_pCell.GetTextVertAlign(); }
        }

        CellState ICellBase.State
        {
            get
            {
                GrPaintStyle flag = m_pCell.ToPaintStyle();
                return (CellState)flag & CellState.All;
            }
        }
    }
}
