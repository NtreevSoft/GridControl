using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public class GrTextUpdater : GrObject
    {
        private int m_nBaseCapacity = 50;
        private List<GrCell> m_vecTextBounds = new List<GrCell>();

        public void AddTextBounds(GrCell pCell)
        {
            if (pCell.m_textBoundsChanged == true)
                return;
#if DEBUG
            if (m_vecTextBounds.Capacity == m_vecTextBounds.Count)
            {
                Console.WriteLine("용량 늘려 {0}", (int)m_vecTextBounds.Capacity);
            }
#endif
            m_vecTextBounds.Add(pCell);
            pCell.m_textBoundsChanged = true;
        }

        public void AddTextBoundsByColumn(GrColumn pColumn)
        {
            GrDataRow pInsertionRow = this.GridCore.GetInsertionRow();
            GrDataRowList pDataRowList = this.GridCore.GetDataRowList();
            AddTextBounds((GrCell)pColumn);

            GrItem pItem = pInsertionRow.GetItem(pColumn);
            AddTextBounds(pItem);

            for (int i = 0; i < pDataRowList.GetDataRowCount(); i++)
            {
                GrDataRow pDataRow = pDataRowList.GetDataRow(i);
                pItem = pDataRow.GetItem(pColumn);
                AddTextBounds(pItem);
            }
        }

        public void AddTextAlign(GrCell pCell)
        {
#if DEBUG
            if (pCell.IsGridCoreAttached() == false)
                throw new Exception();
#endif
            if (pCell.m_textAlignChanged == true)
                return;
            //m_vecTextAligns.push_back(pCell);
            pCell.m_textAlignChanged = true;
        }

        public void AddTextAlignByColumn(GrColumn pColumn)
        {
            GrDataRow pInsertionRow = this.GridCore.GetInsertionRow();
            GrDataRowList pDataRowList = this.GridCore.GetDataRowList();
            AddTextAlign((GrCell)pColumn);

            GrItem pItem = pInsertionRow.GetItem(pColumn);
            AddTextAlign(pItem);

            for (int i = 0; i < pDataRowList.GetDataRowCount(); i++)
            {
                GrDataRow pDataRow = pDataRowList.GetDataRow(i);
                pItem = pDataRow.GetItem(pColumn);
                AddTextAlign(pItem);
            }
        }

        public void RemoveTextBounds(GrCell pCell)
        {
            //    GrCells::iterator itor = std::find(m_vecTextBounds.begin(), m_vecTextBounds.end(), pCell);
            //if(itor != m_vecTextBounds.end())
            //{
            //    pCell.m_textBoundsChanged = false;
            //    m_vecTextBounds.Remove(itor);
            //}
            m_vecTextBounds.Remove(pCell);
        }
        public void RemoveTextAlign(GrCell pCell)
        {

        }

        public void UpdateTextBounds()
        {
            foreach (var value in m_vecTextBounds)
            {
                value.ComputeTextBounds();
                value.m_textBoundsChanged = false;
            }
            m_vecTextBounds.Clear();
        }
        public void UpdateTextAlign()
        {

        }


        protected override void OnGridCoreAttached()
        {
            this.GridCore.Cleared += gridCore_Cleared;
            this.GridCore.CapacityChanged += gridCore_CapacityChanged;
        }


        private void gridCore_Cleared(object sender, EventArgs e)
        {
            this.UpdateTextBounds();
            this.UpdateTextAlign();
        }

        private void gridCore_CapacityChanged(object sender, EventArgs e)
        {
            int capacity = this.GridCore.GetReservedColumn() * this.GridCore.GetReservedRow() + m_nBaseCapacity;
            m_vecTextBounds.Capacity = capacity;

        }

    }
}
