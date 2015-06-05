using Ntreev.Library.Grid;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Windows.Forms.Grid
{
    static class FromNative
    {
        public static Cell Get(GrItem pItem)
        {
            return pItem.ManagedRef as Cell;
        }

        public static Column Get(GrColumn pColumn)
        {
            if (pColumn == null)
                return null;
            return pColumn.ManagedRef as Column;
        }

        public static RowBase Get(IDataRow pDataRow)
        {
            {
                if (pDataRow == null)
                {
                    return null;
                }

                GrGridRow pGridRow = pDataRow as GrGridRow;
                if (pGridRow != null)
                {
                    return Get(pGridRow);
                }
                else if (pDataRow.GetRowType() == GrRowType.GroupRow)
                {
                    return Get((GrGroupRow)pDataRow);
                }
                else if (pDataRow.GetRowType() == GrRowType.DataRow || pDataRow.GetRowType() == GrRowType.InsertionRow)
                {
                    return Get((GrDataRow)pDataRow);
                }

                return null;
            }
        }

        public static GroupRow Get(GrGroupRow pGroupRow)
        {
            object refObject = pGroupRow.ManagedRef;
            if (refObject == null)
            {
                refObject = new GroupRow(pGroupRow);
                pGroupRow.ManagedRef = refObject;
            }

            return refObject as GroupRow;
        }

        public static Row Get(GrDataRow pDataRow)
        {
            return pDataRow.ManagedRef as Row;
        }

        public static CaptionRow Get(GrCaption pCaption)
        {
            return pCaption.ManagedRef as CaptionRow;
        }

        public static GridRow Get(GrGridRow pGridRow)
        {
            object refObject = pGridRow.ManagedRef;
            if (refObject == null)
            {
                refObject = new GridRow(pGridRow);
                pGridRow.ManagedRef = refObject;
            }

            return refObject as GridRow;
        }
    }
}
