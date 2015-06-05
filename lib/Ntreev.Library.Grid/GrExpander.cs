using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Library.Grid
{
    public class GrExpander : GrCell
    {
        private readonly IDataRow m_pDataRow;
        public GrExpander(IDataRow pDataRow)
        {
            this.m_pDataRow = pDataRow;
        }

        public override int GetX()
        {
            int dataDepth = m_pDataRow.GetDataDepth();
            return m_pDataRow.GetRight() + dataDepth * GrDefineUtility.DEF_GROUP_WIDTH;
        }
        public override int GetY()
        {
            return m_pDataRow.GetY();
        }
        public override int GetWidth()
        {
            return GrDefineUtility.DEF_GROUP_WIDTH;
        }
        public override int GetHeight()
        {
            return m_pDataRow.GetHeight();
        }
        public override GrRow GetRow()
        {
            return m_pDataRow;
        }


        public override GrCellType GetCellType()
        {
            return GrCellType.Expander;
        }
        public override bool GetVisible()
        {
            return m_pDataRow.GetVisible() && m_pDataRow.GetChildCount() > 0;
        }
        public override bool GetDisplayable()
        {
            return m_pDataRow.GetDisplayable();
        }

        public override void Paint(GrGridPainter pPainter, GrRect clipRect)
        {
            GrRect paintRect = GetRect();
            GrColor foreColor = GetPaintingLineColor();
            GrColor backColor = GetPaintingBackColor();

            GrPaintStyle paintStyle = GrPaintStyle.None;

            if (m_pDataRow.IsLastVisible() == true)
                paintStyle |= GrPaintStyle.BottomLine;

            pPainter.DrawExpander(paintStyle, paintRect, (GrControlState)0, m_pDataRow.IsExpanded(), foreColor, backColor);
        }
    
    }
}
