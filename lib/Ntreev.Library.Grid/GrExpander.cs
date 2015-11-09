using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Library.Grid
{
    public class GrExpander : GrCell
    {
        private readonly IDataRow pDataRow;

        public GrExpander(IDataRow pDataRow)
        {
            this.pDataRow = pDataRow;
        }

        //public override int X
        //{
        //    get
        //    {
        //        int dataDepth = this.pDataRow.GetDataDepth();
        //        return this.pDataRow.Right + dataDepth * GrDefineUtility.DEF_GROUP_WIDTH;
        //    }
        //}

        //public override int Y
        //{
        //    get { return this.pDataRow.Y; }
        //}

        //public override int Width
        //{
        //    get { return GrDefineUtility.DEF_GROUP_WIDTH; }
        //}

        //public override int Height
        //{
        //    get { return this.pDataRow.Height; }
        //}

        public override GrRow GetRow()
        {
            return this.pDataRow;
        }

        //public override GrCellType GetCellType()
        //{
        //    return GrCellType.Expander;
        //}

        public override bool IsVisible
        {
            get { return this.pDataRow.IsVisible && this.pDataRow.Childs.Count > 0; }
            set { }
        }

        public override bool IsDisplayable
        {
            get { return this.pDataRow.IsDisplayable; }
        }

        public override void Paint(GrGridPainter painter, GrRect clipRect)
        {
            GrRect paintRect = this.Bounds;
            GrColor foreColor = GetPaintingLineColor();
            GrColor backColor = GetPaintingBackColor();

            GrPaintStyle paintStyle = GrPaintStyle.None;

            if (this.pDataRow.IsLastVisible() == true)
                paintStyle |= GrPaintStyle.BottomLine;

            painter.DrawExpander(paintStyle, paintRect, (GrControlState)0, this.pDataRow.IsExpanded, foreColor, backColor);
        }
    }
}
