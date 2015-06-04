using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public abstract class GrCell : GrObject
    {
        private string m_text;
        private GrStyleData m_pStyleData;

        internal bool m_textBoundsChanged;
        internal bool m_textAlignChanged;
        private bool m_textVisible = true;
        private GrTextLayout m_layout;

        private static Dictionary<int, string> s_texts = new Dictionary<int, string>();

        public GrCell()
        {


        }
        //virtual ~GrCell();

        public abstract int GetX();
        public abstract int GetY();
        public abstract int GetWidth();
        public abstract int GetHeight();
        public abstract GrRow GetRow();

        public abstract GrCellType GetCellType();
        public abstract bool GetVisible();
        public abstract bool GetDisplayable();
        public abstract void Paint(GrGridPainter pPainter, GrRect clipRect);

        public bool Contains(GrPoint point)
        {
            return this.ContainsHorz(point.X) && this.ContainsVert(point.Y);
        }
        public bool ContainsHorz(int x)
        {
            if (x < this.GetX() || x >= this.GetRight())
                return false;
            return true;
        }
        public bool ContainsVert(int y)
        {
            if (y < this.GetY() || y >= this.GetBottom())
                return false;
            return true;
        }
        public bool IntersectsHorzWith(int left, int right)
        {
            if (this.GetX() > right || this.GetRight() <= left)
                return false;
            return true;
        }
        public bool IntersectsHorzWith(GrRect rect)
        {
            return this.IntersectsHorzWith(rect.Left, rect.Right);
        }
        public bool IntersectsVertWith(int top, int bottom)
        {
            if (this.GetY() > bottom || this.GetBottom() <= top)
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

        public GrRect GetClientRect()
        {
            GrPadding padding = this.GetPadding();
            return GrRect.FromLTRB(padding.Left, padding.Top, this.GetWidth() - (padding.Right), this.GetHeight() - (padding.Bottom));
        }
        public GrRect GetRect()
        {
            return new GrRect(this.GetX(), this.GetY(), this.GetWidth(), this.GetHeight());
        }
        public GrPoint GetLocation()
        {
            return new GrPoint(this.GetX(), this.GetY());
        }
        public GrSize GetSize()
        {
            return new GrSize(this.GetWidth(), this.GetHeight());
        }

        public int GetBottom()
        {
            return this.GetY() + this.GetHeight();
        }

        public int GetRight()
        {
            return this.GetX() + this.GetWidth();
        }

        public void SetBackColor(GrColor color)
        {
            if (m_pStyleData == null)
                m_pStyleData = new GrStyleData();

            m_pStyleData.backColor = color;
            this.Invalidate();
        }
        public void SetForeColor(GrColor color)
        {
            if (m_pStyleData == null)
                m_pStyleData = new GrStyleData();

            m_pStyleData.foreColor = color;
            this.Invalidate();

        }
        public void SetLineColor(GrColor color)
        {
            if (m_pStyleData == null)
                m_pStyleData = new GrStyleData();

            m_pStyleData.lineColor = color;
            this.Invalidate();
        }
        public void SetFont(GrFont pFont)
        {
            if (m_pStyleData == null)
                m_pStyleData = new GrStyleData();

            m_pStyleData.pFont = pFont;
            this.SetTextBoundsChanged();
        }
        public void SetPadding(GrPadding padding)
        {
            if (m_pStyleData == null)
                m_pStyleData = new GrStyleData();

            m_pStyleData.padding = padding;
            this.SetTextBoundsChanged();
        }

        public GrColor GetForeColorCore()
        {
            if (m_pStyleData == null)
                return GrStyleData.Default.foreColor;
            return m_pStyleData.foreColor;
        }
        public GrColor GetBackColorCore()
        {
            if (m_pStyleData == null)
                return GrStyleData.Default.backColor;
            return m_pStyleData.backColor;
        }
        public GrColor GetLineColorCore()
        {
            if (m_pStyleData == null)
                return GrStyleData.Default.lineColor;
            return m_pStyleData.lineColor;
        }
        public GrFont GetFontCore()
        {
            if (m_pStyleData == null)
                return GrStyleData.Default.pFont;
            return m_pStyleData.pFont;
        }
        public GrPadding GetPaddingCore()
        {
            if (m_pStyleData == null)
                return GrStyleData.Default.padding;
            return m_pStyleData.padding;
        }


        public string GetText()
        {
            if (m_text == null)
                return string.Empty;
            return m_text;
        }
        public void SetText(string text)
        {
            if (m_text == text)
                return;

            m_text = text;
            this.OnTextChanged();
        }
        public int GetTextLineCount()
        {
            return m_layout.linesCount;
        }
        public GrLineDesc GetTextLine(int index)
        {
            return m_layout.pLines[index];
        }
        public GrSize GetTextBounds()
        {
            if (m_layout == null)
                return GrSize.Empty;
            return new GrSize(m_layout.width, m_layout.height);
        }
        public virtual GrSize GetPreferredSize()
        {
            GrSize size = this.GetTextBounds();
            size.Width += this.GetPadding().GetHorizontal();
            size.Height += this.GetPadding().GetVertical();
            return size;
        }
        public GrPoint AlignText(GrLineDesc line, int index, int count)
        {
            GrFont pFont = this.GetPaintingFont();
            GrPoint startLocation = new GrPoint();
            GrPadding padding = GetPadding();
            int width = this.GetWidth() - (padding.Left + padding.Right);
            int height = this.GetHeight() - (padding.Top + padding.Bottom);

            int lineHeight = pFont.GetHeight() + pFont.GetExternalLeading();

            //for(int i=0 ; i<m_vecTextLine.size() ; i++)
            //{
            //    GrLineDesc& cl = m_vecTextLine[i];

            switch (this.GetTextHorzAlign())
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

            switch (this.GetTextVertAlign())
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
            //    m_textBounds.SetLocation(cl.x, cl.y);
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
            //    ti_f.text = m_text;
            //    ti_f.multiline = GetTextMulitiline();

            //    auto itor = textLayouts_f.find(ti_f);

            //    if(itor != textLayouts_f.end())
            //    {
            //        m_layout = itor.second;
            //        if(GetTextBounds() != oldTextBounds)
            //            OnTextSizeChanged();
            //        return;
            //    }
            //}
            //else
            //{
            //    ti.pFont = GetPaintingFont();
            //    ti.text = m_text;
            //    ti.multiline = GetTextMulitiline();
            //    ti.wordwrap = GetTextWordWrap();
            //    ti.width = GetWidth();

            //    auto itor = textLayouts.find(ti);

            //    if(itor != textLayouts.end())
            //    {
            //        m_layout = itor.second;
            //        if(GetTextBounds() != oldTextBounds)
            //            OnTextSizeChanged();
            //        return;
            //    }
            //}


            GrFont pFont = this.GetPaintingFont();
            GrPadding padding = GetPadding();

            m_layout = new GrTextLayout();

            int maxWidth = 0;
            int maxHeight = 0;


            if (GetText().Length > 0)
            {
                int cellWidth = GetWidth() - (padding.Left + padding.Right);

                if (GetTextMulitiline() == false)
                {
                    m_layout.pLines = new GrLineDesc[1];
                    m_layout.pLines[0] = new GrLineDesc();
                    m_layout.linesCount = 1;
                    GrTextUtil.SingleLine(ref m_layout.pLines[0], GetText(), pFont);
                }
                else
                {
                    List<GrLineDesc> lines = new List<GrLineDesc>();
                    GrTextUtil.MultiLine(lines, GetText(), cellWidth, pFont, GetTextWordWrap());

                    m_layout.linesCount = lines.Count;
                    m_layout.pLines = lines.ToArray();
                }

                for (int i = 0; i < m_layout.linesCount; i++)
                {
                    GrLineDesc lineDesc = m_layout.pLines[i];
                    maxWidth = Math.Max(maxWidth, (int)lineDesc.width);
                    maxHeight += pFont.GetHeight() + pFont.GetExternalLeading();
                }
            }
            else
            {
                maxHeight = pFont.GetHeight() + pFont.GetExternalLeading();
            }

            m_layout.width = (ushort)maxWidth;
            m_layout.height = (ushort)maxHeight;

            if (GetTextWordWrap() == false)
            {
                //textLayouts_f.insert(std.pair<tbinfo_fixed, GrTextLayout*>(ti_f, m_layout));
            }
            else
            {
                //textLayouts.insert(std.pair<tbinfo, GrTextLayout*>(ti, m_layout));
            }

            //if(maxWidth + (padding.left + padding.right) > GetWidth() || 
            //    maxHeight + padding.top + padding.bottom > GetHeight())
            //    m_textClipped = true;
            //else
            //    m_textClipped = false;

            //m_textBounds = GrSize(maxWidth, maxHeight);
            if (GetTextBounds() != oldTextBounds)
                OnTextSizeChanged();
        }
        public bool GetTextVisible()
        {
            return m_textVisible;
        }
        public bool GetTextClipped()
        {
            GrPadding padding = GetPaintingPadding();

            if (m_layout.width + (padding.Left + padding.Right) > GetWidth() ||
                m_layout.height + padding.Top + padding.Bottom > GetHeight())
                return true;
            return false;
        }
        public void SetTextVisible(bool b)
        {
            m_textVisible = b;
        }
        public virtual void Invalidate()
        {
            if (this.GridCore == null || this.GetDisplayable() == false)
                return;

            this.GridCore.Invalidate(GetRect());
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
        public void DrawText(GrGridPainter pPainter, GrColor foreColor, GrRect paintRect, GrRect? pClipRect)
        {
            if (this.GetTextVisible() == false)
                return;

            GrFont pFont = this.GetPaintingFont();

            for (int i = 0; i < this.GetTextLineCount(); i++)
            {
                GrLineDesc cl = this.GetTextLine(i);

                GrPoint offset = this.AlignText(cl, i, GetTextLineCount());
                GrRect textRect = GrRect.FromLTRB(paintRect.Left + offset.X,
                    paintRect.Top + offset.Y,
                    paintRect.Left + offset.X + cl.width,
                    paintRect.Top + offset.Y + pFont.GetHeight());

                if (textRect.Top > paintRect.Bottom || textRect.Bottom <= paintRect.Top)
                    continue;

                if (this.GetTextClipped() == true || pClipRect != null)
                {
                    GrRect clipRect = paintRect;
                    clipRect.Contract(GetPadding());

                    if (pClipRect != null)
                    {
                        clipRect = GrRect.FromLTRB(clipRect.Left, clipRect.Top,
                            Math.Min(clipRect.Right, pClipRect.Value.Right),
                            Math.Min(clipRect.Bottom, pClipRect.Value.Bottom));
                    }

                    pPainter.DrawText(pFont, GetText() + cl.textBegin, cl.length, textRect, foreColor, clipRect);
                }
                else
                {
                    pPainter.DrawText(pFont, GetText() + cl.textBegin, cl.length, textRect, foreColor, null);
                }
            }
        }

        public virtual GrHorzAlign GetTextHorzAlign()
        {
            return GrHorzAlign.Left;
        }
        public virtual GrVertAlign GetTextVertAlign()
        {
            return GrVertAlign.Top;
        }
        public virtual bool GetTextWordWrap()
        {
            return false;
        }

        public virtual bool GetTextMulitiline() { return false; }

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
            if (localLocation.X < 0 || localLocation.Y < 0 ||
        localLocation.X >= this.GetWidth() || localLocation.Y >= this.GetHeight())
                return 0;
            return 1;
        }

        public GrGridCore GetGridCore() { return this.GridCore; }


        protected virtual void OnTextChanged()
        {
            this.SetTextBoundsChanged();
        }
        protected virtual void OnTextSizeChanged()
        {

        }

        protected virtual void OnGridCoreAttached()
        {
            base.OnGridCoreAttached();
            GrTextUpdater pTextUpdater = this.GridCore.GetTextUpdater();
            pTextUpdater.AddTextBounds(this);
            pTextUpdater.AddTextAlign(this);
        }
        protected virtual void OnGridCoreDetached()
        {
            GrTextUpdater pTextUpdater = this.GridCore.GetTextUpdater();
            if (m_textAlignChanged == true)
                pTextUpdater.RemoveTextAlign(this);
            if (m_textBoundsChanged == true)
                pTextUpdater.RemoveTextBounds(this);
            base.OnGridCoreDetached();
        }

        protected void SetTextBoundsChanged()
        {
            if (this.GridCore != null)
            {
                GrTextUpdater pTextUpdater = this.GridCore.GetTextUpdater();
                pTextUpdater.AddTextBounds(this);
                if (this.GetDisplayable() == true)
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
                if (this.GetDisplayable() == true)
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




    }
}
