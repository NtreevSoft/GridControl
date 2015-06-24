using Ntreev.Library.Grid;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Windows.Forms.Grid
{
    public sealed class CellTagCollection : IEnumerable, ICollection
    {
        private readonly Row m_row;
        private readonly GrColumnList m_pColumnList;

        internal CellTagCollection(Row row)
        {
            m_row = row;
            m_pColumnList = row.GridCore.GetColumnList();
        }

        public object this[string columnName]
        {
            get
            {
                return m_row.Cells[columnName].Tag;
            }
            set
            {
                m_row.Cells[columnName].Tag = value;
            }
        }

        public object this[Column column]
        {
            get
            {
                return m_row.Cells[column].Tag; ;
            }
            set
            {
                m_row.Cells[column].Tag = value;
            }
        }

        public object this[int index]
        {
            get
            {
                return m_row.Cells[index].Tag;
            }
            set
            {
                m_row.Cells[index].Tag = value;
            }
        }

        public int Count
        {
            get
            {
                return m_row.Cells.Count;
            }
        }

        #region classes

        class Enumerator : IEnumerator
        {
            private int m_index = -1;
            private readonly GrDataRow m_pDataRow;
            private readonly GrColumnList m_pColumnList;

            public Enumerator(GrDataRow pDataRow, GrColumnList pColumnList)
            {
                m_pDataRow = pDataRow;
                m_pColumnList = pColumnList;
            }

            public void Dispose()
            {

            }

            public bool MoveNext()
            {
                m_index++;
                return m_index < m_pColumnList.GetColumnCount();
            }
            public void Reset()
            {
                m_index = -1;
            }

            public object Current
            {
                get
                {
                    GrColumn pColumn = m_pColumnList.GetColumn(m_index - 1);
                    GrItem pItem = m_pDataRow.GetItem(pColumn);
                    object reference = pItem.ManagedRef;
                    Cell cell = reference as Cell;
                    return cell.Tag;
                }
            }
        }

        #endregion

        #region ICollection

        void ICollection.CopyTo(Array array, int index)
        {
            foreach (object item in this)
            {
                array.SetValue(item, index++);
            }
        }

        bool ICollection.IsSynchronized
        {
            get { return true; }
        }

        object ICollection.SyncRoot
        {
            get { return this.m_row.GridControl; }
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return new Enumerator(m_row.NativeRef, m_pColumnList);
        }

        #endregion
    }
}
