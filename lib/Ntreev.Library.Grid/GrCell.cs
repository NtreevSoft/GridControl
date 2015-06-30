using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public abstract class GrCell : GrObject
    {
        internal bool textBoundsChanged;
        internal bool textAlignChanged;

        private string text;
        private GrStyleData styleData;

        private bool textVisible = true;
        private GrTextLayout layout;

        private static Dictionary<int, string> s_texts = new Dictionary<int, string>();
        private bool isVisible = true;
        private GrRect bounds;

        protected GrCell()
        {


        }

        public int X
        {
            get { return this.bounds.X; }
            set
            {
                this.SetBounds(value, this.bounds.Y, this.bounds.Width, this.bounds.Height);
            }
        }

        public int Y
        {
            get { return this.bounds.Y; }
            set
            {
                this.SetBounds(this.bounds.X, value, this.bounds.Width, this.bounds.Height);
            }
        }

        public int Width
        {
            get { return this.bounds.Width; }
            set
            {
                this.SetBounds(this.bounds.X, this.bounds.Y, value, this.bounds.Height);
            }
        }

        public int Height
        {
            get { return this.bounds.Height; }
            set
            {
                this.SetBounds(this.bounds.X, this.bounds.Y, this.bounds.Width, value);
            }
        }

        public abstract GrRow GetRow();

        //public abstract GrCellType GetCellType();

        public virtual bool IsVisible
        {
            get { return this.isVisible; }
            set
            {
                if (this.isVisible == value)
                    return;
                this.isVisible = value;
                this.OnVisibleChanged(EventArgs.Empty);
            }
        }

        public abstract bool IsDisplayable
        {
            get;
        }

        public abstract void Paint(GrGridPainter painter, GrRect clipRect);

        public bool Contains(GrPoint point)
        {
            return this.ContainsHorz(point.X) && this.ContainsVert(point.Y);
        }

        public bool ContainsHorz(int x)
        {
            if (x < this.X || x >= this.Right)
                return false;
            return true;
        }

        public bool ContainsVert(int y)
        {
            if (y < this.Y || y >= this.Bottom)
                return false;
            return true;
        }

        public bool IntersectsHorzWith(int left, int right)
        {
            if (this.X > right || this.Right <= left)
                return false;
            return true;
        }

        public bool IntersectsHorzWith(GrRect rect)
        {
            return this.IntersectsHorzWith(rect.Left, rect.Right);
        }

        public bool IntersectsVertWith(int top, int bottom)
        {
            if (this.Y > bottom || this.Bottom <= top)
                return false;
            return true;
        }

        public bool IntersectsVertWith(GrRect rect)
        {
            return this.IntersectsVertWith(rect.Top, rect.Bottom);
        }

        public bool IntersectsWith(GrRect rect)
        {
            return this.IntersectsHorzWith(rect.Left, rect.Right) &&
                this.IntersectsVertWith(rect.Top, rect.Bottom);
        }

        public bool GetMouseOvered()
        {
            if (this.GridCore == null)
                return false;
            return this.GridCore.GetMouseOver() == this;
        }

        public bool GetMousePressed()
        {
            if (this.GridCore == null)
                return false;
            return this.GridCore.GetMousePress() == this;
        }

        public int GetMouseOverState()
        {
            if (this.GridCore == null)
                return 0;
            return this.GridCore.GetMouseOverState();
        }

        public GrRect ClientRectangle
        {
            get
            {
                GrPadding padding = this.GetPadding();
                return GrRect.FromLTRB(padding.Left, padding.Top, this.Width - (padding.Right), this.Height - (padding.Bottom));
            }
        }

        public GrRect Bounds
        {
            get { return this.bounds; }
            set
            {
                this.SetBounds(value.X, value.Y, value.Width, value.Height);
            }
        }

        public GrPoint Location
        {
            get { return this.bounds.Location; }
            set 
            {
                this.SetBounds(value.X, value.Y, this.bounds.Width, this.bounds.Height);
            }
        }

        public GrSize Size
        {
            get { return this.bounds.Size;}
            set 
            {
                this.SetBounds(this.bounds.X, this.bounds.Y, value.Width, value.Height);
            }
        }

        protected virtual void SetBounds(int x, int y, int width, int height)
        {
            GrRect bounds = this.bounds;
            bool locationChanged = bounds.X != x || bounds.Y != y;
            bool sizeChanged = bounds.Width != width || bounds.Height != height;

            this.bounds.Location = new GrPoint(x, y);
            this.bounds.Size = new GrSize(width, height);

            if (locationChanged == true)
            {
                this.OnLocationChanged(EventArgs.Empty);
            }

            if (sizeChanged == true)
            {
                this.OnSizeChanged(EventArgs.Empty);
            }
            
        }

        protected virtual void OnLocationChanged(EventArgs e)
        {
            if (this.LocationChanged != null)
            {
                this.LocationChanged(this, e);
            }
        }

        protected virtual void OnSizeChanged(EventArgs e)
        {
            if (this.SizeChanged != null)
            {
                this.SizeChanged(this, e);
            }
        }

        public int Top
        {
            get { return this.bounds.Top; }
        }

        public int Left
        {
            get { return this.bounds.Left; }
        }

        public int Bottom
        {
            get { return this.bounds.Bottom; }
        }

        public int Right
        {
            get { return this.bounds.Right; }
        }

        public void SetBackColor(GrColor color)
        {
            if (this.styleData == null)
                this.styleData = new GrStyleData();

            this.styleData.backColor = color;
            this.Invalidate();
        }

        public void SetForeColor(GrColor color)
        {
            if (this.styleData == null)
                this.styleData = new GrStyleData();

            this.styleData.foreColor = color;
            this.Invalidate();

        }

        public void SetLineColor(GrColor color)
        {
            if (this.styleData == null)
                this.styleData = new GrStyleData();

            this.styleData.lineColor = color;
            this.Invalidate();
        }

        public void SetFont(GrFont pFont)
        {
            if (this.styleData == null)
                this.styleData = new GrStyleData();

            this.styleData.pFont = pFont;
            this.SetTextBoundsChanged();
        }

        public void SetPadding(GrPadding padding)
        {
            if (this.styleData == null)
                this.styleData = new GrStyleData();

            this.styleData.padding = padding;
            this.SetTextBoundsChanged();
        }

        public GrColor GetForeColorCore()
        {
            if (this.styleData == null)
                return GrStyleData.Default.foreColor;
            return this.styleData.foreColor;
        }

        public GrColor GetBackColorCore()
        {
            if (this.styleData == null)
                return GrStyleData.Default.backColor;
            return this.styleData.backColor;
        }

        public GrColor GetLineColorCore()
        {
            if (this.styleData == null)
                return GrStyleData.Default.lineColor;
            return this.styleData.lineColor;
        }

        public GrFont GetFontCore()
        {
            if (this.styleData == null)
                return GrStyleData.Default.pFont;
            return this.styleData.pFont;
        }

        public GrPadding GetPaddingCore()
        {
            if (this.styleData == null)
                return GrStyleData.Default.padding;
            return this.styleData.padding;
        }

        public string Text
        {
            get
            {
                if (this.text == null)
                    return string.Empty;
                return this.text;
            }
            set
            {
                if (this.text == value)
                    return;

                this.text = value;
                this.OnTextChanged();
            }
        }

        public int GetTextLineCount()
        {
            if (this.layout == null)
                return 0;
            return this.layout.linesCount;
        }

        public GrLineDesc GetTextLine(int index)
        {
            return this.layout.pLines[index];
        }

        public GrSize GetTextBounds()
        {
            if (this.layout == null)
                return GrSize.Empty;
            return new GrSize(this.layout.width, this.layout.height);
        }

        public virtual GrSize GetPreferredSize()
        {
            GrSize size = this.GetTextBounds();
            size.Width += this.GetPadding().Horizontal;
            size.Height += this.GetPadding().Vertical;
            return size;
        }

        public GrPoint AlignText(GrLineDesc line, int index, int count)
        {
            GrFont pFont = this.GetPaintingFont();
            GrPoint startLocation = new GrPoint();
            GrPadding padding = GetPadding();
            int width = this.Width - (padding.Left + padding.Right);
            int height = this.Height - (padding.Top + padding.Bottom);

            int lineHeight = pFont.GetHeight() + pFont.GetExternalLeading();

            //for(int i=0 ; i<this.vecTextLine.size() ; i++)
            //{
            //    GrLineDesc& cl = this.vecTextLine[i];

            switch (this.TextHorzAlign)
            {
                case GrHorzAlign.Left:
                    startLocation.X = 0;
                    break;
                case GrHorzAlign.Center:
                    startLocation.X = (width - line.width) / 2;
                    break;
                case GrHorzAlign.Right:
                    startLocation.X = width - line.width;
                    break;
                default:
                    break;
            }

            switch (this.TextVertAlign)
            {
                case GrVertAlign.Top:
                    startLocation.Y = index * lineHeight;
                    break;
                case GrVertAlign.Center:
                    startLocation.Y = (height - count * lineHeight) / 2 + index * lineHeight;
                    break;
                case GrVertAlign.Bottom:
                    startLocation.Y = height - ((index + 1) * lineHeight);
                    break;
                default:
                    break;
            }

            if (height < lineHeight)
                startLocation.Y = index * lineHeight;

            return new GrPoint(startLocation.X + padding.Left, startLocation.Y + padding.Top);

            //if(i==0)
            //{
            //    this.textBounds.SetLocation(cl.x, cl.y);
            //}
            //}
        }

        public void ComputeTextBounds()
        {
            //        tbinfo_fixed ti_f;
            //tbinfo ti;
            GrSize oldTextBounds = GetTextBounds();

            //if(GetTextWordWrap() == false)
            //{
            //    ti_f.pFont = GetPaintingFont();
            //    ti_f.text = this.text;
            //    ti_f.multiline = GetTextMulitiline();

            //    auto itor = textLayouts_f.find(ti_f);

            //    if(itor != textLayouts_f.end())
            //    {
            //        this.layout = itor.second;
            //        if(GetTextBounds() != oldTextBounds)
            //            OnTextSizeChanged();
            //        return;
            //    }
            //}
            //else
            //{
            //    ti.pFont = GetPaintingFont();
            //    ti.text = this.text;
            //    ti.multiline = GetTextMulitiline();
            //    ti.wordwrap = GetTextWordWrap();
            //    ti.width = GetWidth();

            //    auto itor = textLayouts.find(ti);

            //    if(itor != textLayouts.end())
            //    {
            //        this.layout = itor.second;
            //        if(GetTextBounds() != oldTextBounds)
            //            OnTextSizeChanged();
            //        return;
            //    }
            //}


            GrFont pFont = this.GetPaintingFont();
            GrPadding padding = GetPadding();

            this.layout = new GrTextLayout();

            int maxWidth = 0;
            int maxHeight = 0;


            if (this.Text.Length > 0)
            {
                int cellWidth = this.Width - (padding.Left + padding.Right);

                if (this.IsTextMulitiline == false)
                {
                    this.layout.pLines = new GrLineDesc[1];
                    this.layout.pLines[0] = new GrLineDesc();
                    this.layout.linesCount = 1;
                    GrTextUtil.SingleLine(ref this.layout.pLines[0], this.Text, pFont);
                }
                else
                {
                    List<GrLineDesc> lines = new List<GrLineDesc>();
                    GrTextUtil.MultiLine(lines, this.Text, cellWidth, pFont, this.IsTextWordWrap);

                    this.layout.linesCount = lines.Count;
                    this.layout.pLines = lines.ToArray();
                }

                for (int i = 0; i < this.layout.linesCount; i++)
                {
                    GrLineDesc lineDesc = this.layout.pLines[i];
                    maxWidth = Math.Max(maxWidth, (int)lineDesc.width);
                    maxHeight += pFont.GetHeight() + pFont.GetExternalLeading();
                }
            }
            else
            {
                maxHeight = pFont.GetHeight() + pFont.GetExternalLeading();
            }

            this.layout.width = (ushort)maxWidth;
            this.layout.height = (ushort)maxHeight;

            if (this.IsTextWordWrap == false)
            {
                //textLayouts_f.insert(std.pair<tbinfo_fixed, GrTextLayout*>(ti_f, this.layout));
            }
            else
            {
                //textLayouts.insert(std.pair<tbinfo, GrTextLayout*>(ti, this.layout));
            }

            //if(maxWidth + (padding.left + padding.right) > GetWidth() || 
            //    maxHeight + padding.top + padding.bottom > GetHeight())
            //    this.textClipped = true;
            //else
            //    this.textClipped = false;

            //this.textBounds = GrSize(maxWidth, maxHeight);
            if (GetTextBounds() != oldTextBounds)
                OnTextSizeChanged();
        }

        public bool IsTextVisible
        {
            get { return this.textVisible; }
            set { this.textVisible = value; }
        }

        public bool IsTextClipped
        {
            get
            {
                GrPadding padding = GetPaintingPadding();

                if (this.layout.width + (padding.Left + padding.Right) > this.Width ||
                    this.layout.height + padding.Top + padding.Bottom > this.Height)
                    return true;
                return false;
            }
        }

        public virtual void Invalidate()
        {
            if (this.GridCore == null || this.IsDisplayable == false)
                return;

            this.GridCore.Invalidate(this.Bounds);
        }

        public virtual GrPaintStyle ToPaintStyle()
        {
            GrPaintStyle flag = GrPaintStyle.Default;
            if (this.GetMouseOvered() == true)
                flag |= GrPaintStyle.Mouseover;
            if (GetMousePressed() == true)
                flag |= GrPaintStyle.Pressed;
            return flag;
        }

        public void DrawText(GrGridPainter painter, GrColor foreColor, GrRect paintRect, GrRect? pClipRect)
        {
            if (this.IsTextVisible == false)
                return;

            GrFont pFont = this.GetPaintingFont();

            for (int i = 0; i < this.GetTextLineCount(); i++)
            {
                GrLineDesc cl = this.GetTextLine(i);

                GrPoint offset = this.AlignText(cl, i, GetTextLineCount());

                int left = paintRect.Left + offset.X;
                int top = paintRect.Top + offset.Y;
                int right = paintRect.Left + offset.X + cl.width;
                int bottom = paintRect.Top + offset.Y + pFont.GetHeight();
                GrRect textRect = GrRect.FromLTRB(left, top, right, bottom);

                if (textRect.Top > paintRect.Bottom || textRect.Bottom <= paintRect.Top)
                    continue;

                if (this.IsTextClipped == true || pClipRect != null)
                {
                    GrRect clipRect = paintRect;
                    clipRect.Contract(GetPadding());

                    if (pClipRect != null)
                    {
                        clipRect = GrRect.FromLTRB(clipRect.Left, clipRect.Top,
                            Math.Min(clipRect.Right, pClipRect.Value.Right),
                            Math.Min(clipRect.Bottom, pClipRect.Value.Bottom));
                    }

                    painter.DrawText(pFont, this.Text + cl.textBegin, cl.length, textRect, foreColor, clipRect);
                }
                else
                {
                    painter.DrawText(pFont, this.Text + cl.textBegin, cl.length, textRect, foreColor, null);
                }
            }
        }

        public virtual GrHorzAlign TextHorzAlign
        {
            get { return GrHorzAlign.Left; }
        }

        public virtual GrVertAlign TextVertAlign
        {
            get { return GrVertAlign.Top; }
        }

        public virtual bool IsTextWordWrap
        {
            get { return false; }
        }

        public virtual bool IsTextMulitiline
        {
            get { return false; }
        }

        public virtual GrColor GetPaintingForeColor()
        {
            return this.GetForeColor();
        }

        public virtual GrColor GetPaintingBackColor()
        {
            return this.GetBackColor();
        }

        public virtual GrColor GetPaintingLineColor()
        {
            return this.GetLineColor();
        }

        public virtual GrFont GetPaintingFont()
        {
            return this.GetFont();
        }

        public virtual GrPadding GetPaintingPadding()
        {
            return this.GetPadding();
        }

        public virtual GrColor GetForeColor()
        {
            GrColor color = this.GetForeColorCore();
            if (color != GrColor.Empty)
                return color;

            if (this.GridCore != null)
            {
                GrStyle pStyle = this.GridCore.GetStyle();
                if (pStyle != null)
                    return pStyle.ForeColor;
                return this.GridCore.GetForeColor();
            }
            return GrStyle.Default.ForeColor;
        }

        public virtual GrColor GetBackColor()
        {
            GrColor color = GetBackColorCore();
            if (color != GrColor.Empty)
                return color;

            if (this.GridCore != null)
            {
                GrStyle pStyle = this.GridCore.GetStyle();
                if (pStyle != null)
                    return pStyle.BackColor;
                return this.GridCore.GetBackColor();
            }
            return GrStyle.Default.BackColor;
        }

        public virtual GrColor GetLineColor()
        {
            GrColor color = GetLineColorCore();
            if (color != GrColor.Empty)
                return color;

            if (this.GridCore != null)
            {
                GrStyle pStyle = this.GridCore.GetStyle();
                if (pStyle != null)
                    return pStyle.LineColor;
                return this.GridCore.GetLineColor();
            }
            return GrStyle.Default.LineColor;
        }

        public virtual GrFont GetFont()
        {
            GrFont pFont = GetFontCore();
            if (pFont != null)
                return pFont;

            if (this.GridCore != null)
            {
                GrStyle pStyle = this.GridCore.GetStyle();
                if (pStyle != null)
                    return pStyle.Font;
                return this.GridCore.GetFont();
            }
            return GrStyle.Default.Font;
        }

        public virtual GrPadding GetPadding()
        {
            GrPadding padding = GetPaddingCore();
            if (padding != GrPadding.Empty)
                return padding;

            if (this.GridCore != null)
            {
                GrStyle pStyle = this.GridCore.GetStyle();
                if (pStyle != null)
                    return pStyle.Padding;
                return this.GridCore.GetPadding();
            }

            return GrStyle.Default.Padding;
        }

        public virtual int HitMouseOverTest(GrPoint localLocation)
        {
            if (localLocation.X < 0 || localLocation.Y < 0 || localLocation.X >= this.Width || localLocation.Y >= this.Height)
                return 0;
            return 1;
        }

        public event EventHandler VisibleChanged;

        public event EventHandler LocationChanged;

        public event EventHandler SizeChanged;

        protected virtual void OnTextChanged()
        {
            this.SetTextBoundsChanged();
        }

        protected virtual void OnTextSizeChanged()
        {

        }

        protected virtual void OnVisibleChanged(EventArgs e)
        {
            if (this.VisibleChanged != null)
            {
                this.VisibleChanged(this, e);
            }
        }

        protected override void OnGridCoreAttached()
        {
            base.OnGridCoreAttached();
            GrTextUpdater pTextUpdater = this.GridCore.GetTextUpdater();
            pTextUpdater.AddTextBounds(this);
            pTextUpdater.AddTextAlign(this);
        }

        protected override void OnGridCoreDetached()
        {
            GrTextUpdater pTextUpdater = this.GridCore.GetTextUpdater();
            if (this.textAlignChanged == true)
                pTextUpdater.RemoveTextAlign(this);
            if (this.textBoundsChanged == true)
                pTextUpdater.RemoveTextBounds(this);
            base.OnGridCoreDetached();
        }

        protected void SetTextBoundsChanged()
        {
            if (this.GridCore != null)
            {
                GrTextUpdater pTextUpdater = this.GridCore.GetTextUpdater();
                pTextUpdater.AddTextBounds(this);
                if (this.IsDisplayable == true)
                {
                    this.Invalidate();
                }
            }
        }

        protected void SetTextAlignChanged()
        {
            if (this.GridCore != null)
            {
                GrTextUpdater pTextUpdater = this.GridCore.GetTextUpdater();
                pTextUpdater.AddTextAlign(this);
                if (this.IsDisplayable == true)
                {
                    this.Invalidate();
                }
            }
        }

        class GrStyleData
        {
            public GrColor backColor;
            public GrColor foreColor;
            public GrColor lineColor;
            public GrFont pFont;
            public GrPadding padding;

            public static readonly GrStyleData Default = new GrStyleData();
        };

        public object ManagedRef
        {
            get;
            set;
        }
    }
}
