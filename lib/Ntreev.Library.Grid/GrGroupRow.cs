using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public class GrGroupRow : IDataRow
    {
        internal uint m_groupLevel;

        GrColumn m_column;
        string m_itemText;
        internal string m_key;

        GrGroupHeader m_pLabel;

        public GrGroupRow()
        {
            m_groupLevel = 0;
            m_column = null;

            m_pLabel = new GrGroupHeader(this);

            this.IsExpanded = true;
        }
        //virtual ~GrGroupRow();

        public GrColumn GetColumn()
        {
            return m_column;
        }

        public GrGroupHeader GetLabel()
        {
            return m_pLabel;
        }

        public uint GetGroupLevel() { return m_groupLevel; }

        public override GrRowType GetRowType() { return GrRowType.GroupRow; }
        public override void Paint(GrGridPainter painter, GrRect clipRect)
        {
            base.Paint(painter, clipRect);
            m_pLabel.Paint(painter, clipRect);
        }


        public override IFocusable GetFocusable(GrColumn column)
        {
            return m_pLabel;
        }

        public string GetKey() { return m_key; }

        protected override void OnSizeChanged(EventArgs e)
        {
            base.OnSizeChanged(e);
            this.SetTextAlignChanged();
        }

        protected virtual void OnUpdatePositionCell(int x, ref GrRect pBounds)
        {
            pBounds.X = x;
            pBounds.Width = m_pLabel.X + m_pLabel.Width - x;
        }
        protected override void OnGridCoreAttached()
        {
            base.OnGridCoreAttached();
            this.GridCore.AttachObject(m_pLabel);
        }

        protected override GrCell OnHitTest(int x)
        {
            if (m_pLabel.ContainsHorz(x) == true)
                return m_pLabel;
            return null;
        }



        internal void SetReference(GrColumn column, string itemText)
        {
            m_column = column;
            m_itemText = itemText;

            m_key = column.Text;
            m_key += " | ";
            m_key += itemText;
        }

        internal void ProcessAfterGroup()
        {
            GrGroupRow pParent = this.Parent as GrGroupRow;
            if (pParent != null)
            {
                m_groupLevel = pParent.m_groupLevel + 1;
            }
            else
            {
                m_groupLevel = 0;
            }

            string text = this.Text;


            //wchar_t itemText[30];
            //swprintf(itemText, 30, L" - %d items", GetChildCount());
            //text = m_itemText + itemText;
            //m_pLabel.SetText(text.c_str());
            m_pLabel.Text = m_itemText;
        }


    }
}
