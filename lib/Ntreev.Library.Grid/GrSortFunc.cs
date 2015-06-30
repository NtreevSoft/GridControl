using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Library.Grid
{
    static class GrSortFunc
    {
        public static  int SortRowsDown(GrGridCore pGridCore, GrRow row1, GrRow row2, object userData)
        {
            return SortRowsUp(pGridCore, row2, row1, userData);
        }

        public static int SortRowsUp(GrGridCore pGridCore, GrRow row1, GrRow row2, object userData)
        {
            int result = row1.Text.CompareTo(row2.Text);
            if (result == 0)
                return row1.GetID().CompareTo(row2.GetID());
            return result.CompareTo(0);
        }

        public static int SortRowsNone(GrGridCore pGridCore, GrRow row1, GrRow row2, object userData)
        {
            return row1.GetID().CompareTo(row2.GetID());
        }

        public static int SortDataRowSortDown(GrGridCore pGridCore, GrRow row1, GrRow row2, object userData)
        {
            return SortDataRowSortUp(pGridCore, row2, row1, userData);
        }

        public static int SortDataRowSortNone(GrGridCore pGridCore, GrRow row1, GrRow row2, object userData)
        {
            GrDataRow pDataRow1 = row1 as GrDataRow;
            GrDataRow pDataRow2 = row2 as GrDataRow;
            return pDataRow1.GetDataRowIndex().CompareTo(pDataRow2.GetDataRowIndex());
        }

        public static int SortDataRowSortUp(GrGridCore pGridCore, GrRow row1, GrRow row2, object userData)
        {
            if (row1.GetRowType() == row2.GetRowType())
            {
                if (row1.GetRowType() == GrRowType.DataRow)
                {
                    GrColumn column = userData as GrColumn;
                    GrItem pItem1 = (row1 as GrDataRow).GetItem(column);
                    GrItem pItem2 = (row2 as GrDataRow).GetItem(column);
                    return pItem1.Text.CompareTo(pItem2.Text);
                }
                else if (row1.GetRowType() == GrRowType.GroupRow)
                {
                    return row1.Text.CompareTo(row2.Text);
                }
                else
                {
                    return row1.GetID().CompareTo(row2.GetID());
                }
            }
            return row1.GetRowType().CompareTo(row2.GetRowType());
        }
    }
}
