using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Library.Grid
{
    static class GrSortFunc
    {
        public static  int SortRowsDown(GrGridCore pGridCore, GrRow pRow1, GrRow pRow2, object userData)
        {
            return SortRowsUp(pGridCore, pRow2, pRow1, userData);
        }

        public static int SortRowsUp(GrGridCore pGridCore, GrRow pRow1, GrRow pRow2, object userData)
        {
            int result = pRow1.GetText().CompareTo(pRow2.GetText());
            if (result == 0)
                return pRow1.GetID().CompareTo(pRow2.GetID());
            return result.CompareTo(0);
        }

        public static int SortRowsNone(GrGridCore pGridCore, GrRow pRow1, GrRow pRow2, object userData)
        {
            return pRow1.GetID().CompareTo(pRow2.GetID());
        }

        public static int SortDataRowSortDown(GrGridCore pGridCore, GrRow pRow1, GrRow pRow2, object userData)
        {
            return SortDataRowSortUp(pGridCore, pRow2, pRow1, userData);
        }

        public static int SortDataRowSortNone(GrGridCore pGridCore, GrRow pRow1, GrRow pRow2, object userData)
        {
            GrDataRow pDataRow1 = pRow1 as GrDataRow;
            GrDataRow pDataRow2 = pRow2 as GrDataRow;
            return pDataRow1.GetDataRowIndex().CompareTo(pDataRow2.GetDataRowIndex());
        }

        public static int SortDataRowSortUp(GrGridCore pGridCore, GrRow pRow1, GrRow pRow2, object userData)
        {
            if (pRow1.GetRowType() == pRow2.GetRowType())
            {
                if (pRow1.GetRowType() == GrRowType.DataRow)
                {
                    GrColumn pColumn = userData as GrColumn;
                    GrItem pItem1 = (pRow1 as GrDataRow).GetItem(pColumn);
                    GrItem pItem2 = (pRow2 as GrDataRow).GetItem(pColumn);
                    return pItem1.GetText().CompareTo(pItem2.GetText());
                }
                else if (pRow1.GetRowType() == GrRowType.GroupRow)
                {
                    return pRow1.GetText().CompareTo(pRow2.GetText());
                }
                else
                {
                    return pRow1.GetID().CompareTo(pRow2.GetID());
                }
            }
            return pRow1.GetRowType().CompareTo(pRow2.GetRowType());
        }
    }
}
