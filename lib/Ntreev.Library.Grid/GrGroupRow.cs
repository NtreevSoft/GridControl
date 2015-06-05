using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public class GrGroupRow : IDataRow
    {
        internal uint m_groupLevel;

        GrColumn m_pColumn;
        string m_itemText;
        internal string m_key;

        GrGroupHeader m_pLabel;

        public GrGroupRow()
        {
            m_groupLevel = 0;
            m_pColumn = null;

            m_pLabel = new GrGroupHeader(this);

            Expand(true);
        }
        //virtual ~GrGroupRow();

        public GrColumn GetColumn()
        {
            return m_pColumn;
        }

        public GrGroupHeader GetLabel()
        {
            return m_pLabel;
        }

        public uint GetGroupLevel() { return m_groupLevel; }

        public override GrRowType GetRowType() { return GrRowType.GroupRow; }
        public override void Paint(GrGridPainter pPainter, GrRect clipRect)
        {
            base.Paint(pPainter, clipRect);
            m_pLabel.Paint(pPainter, clipRect);
        }


        public override IFocusable GetFocusable(GrColumn pColumn)
        {
            return m_pLabel;
        }

        public string GetKey() { return m_key; }


        protected override void OnHeightChanged()
        {
            base.OnHeightChanged();
            SetTextAlignChanged();
        }


        protected virtual void OnUpdatePositionCell(int x, ref GrRect pBounds)
        {
            pBounds.X = x;
            pBounds.Width = m_pLabel.GetX() + m_pLabel.GetWidth() - x;
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



        internal void SetReference(GrColumn pColumn, string itemText)
        {
            m_pColumn = pColumn;
            m_itemText = itemText;

            m_key = pColumn.GetText();
            m_key += " | ";
            m_key += itemText;
        }

        internal void ProcessAfterGroup()
        {
            GrGroupRow pParent = GetParent() as GrGroupRow;
            if (pParent != null)
            {
                m_groupLevel = pParent.m_groupLevel + 1;
            }
            else
            {
                m_groupLevel = 0;
            }

            string text = GetText();


            //wchar_t itemText[30];
            //swprintf(itemText, 30, L" - %d items", GetChildCount());
            //text = m_itemText + itemText;
            //m_pLabel.SetText(text.c_str());
            m_pLabel.SetText(m_itemText);
        }


    }
}
