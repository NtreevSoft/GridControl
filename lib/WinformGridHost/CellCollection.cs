using System;
using System.Linq;
using System.Text;
using System.Collections;
using System.Collections.Generic;
using Ntreev.Library.Grid;

namespace Ntreev.Windows.Forms.Grid
{
    public class CellCollection : GridObject, ICollection, IEnumerable<Cell>
    {
        Row m_row;
        GrColumnList m_pColumnList;

        internal CellCollection(Row row)
            : base(row.GridControl)
        {
            m_row = row;
            m_pColumnList = row.GridCore.GetColumnList();
        }

        public Cell this[string columnName]
        {
            get
            {
                foreach (Cell cell in this)
                {
                    if (cell.Column.ColumnName == columnName)
                        return cell;
                }
                throw new ArgumentException();
            }

        }

        public Cell this[Column column]
        {
            get
            {
                if (column == null)
                    throw new NullReferenceException();
                foreach (Cell cell in this)
                {
                    if (cell.Column == column)
                        return cell;
                }
                throw new ArgumentException();
            }
        }


        public Cell this[int index]
        {
            get
            {
                GrColumn pColumn = m_pColumnList.GetColumn(index);
                GrDataRow pDataRow = m_row.NativeRef;
                GrItem pItem = pDataRow.GetItem(pColumn);
                return FromNative.Get(pItem);
            }
        }

        public int Count
        {
            get { return m_pColumnList.GetColumnCount(); }
        }

        internal Cell this[GrColumn pColumn]
        {
            get
            {
                GrDataRow pDataRow = m_row.NativeRef;
                GrItem pItem = pDataRow.GetItem(pColumn);
                return FromNative.Get(pItem);
            }
        }

        #region classes

        class Enumerator : IEnumerator<Cell>
        {
            int m_index = -1;
            GrDataRow m_pDataRow;
            GrColumnList m_pColumnList;
            GridControl m_gridControl;

            public Enumerator(GridControl gridControl, GrDataRow pDataRow, GrColumnList pColumnList)
            {
                m_gridControl = gridControl;
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

            public Cell Current
            {
                get
                {
                    GrColumn pColumn = m_pColumnList.GetColumn(m_index);
                    GrItem pItem = m_pDataRow.GetItem(pColumn);
                    return FromNative.Get(pItem);
                }
            }

            #region IEnumerator

            object IEnumerator.Current
            {
                get { return this.Current; }
            }

            #endregion
        }

        #endregion

        #region ICollection

        void ICollection.CopyTo(Array array, int index)
        {
            foreach (Cell item in this)
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
            get { return this.GridControl; }
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return new Enumerator(this.GridControl, m_row.NativeRef, m_pColumnList);
        }

        IEnumerator<Cell> IEnumerable<Cell>.GetEnumerator()
        {
            return new Enumerator(this.GridControl, m_row.NativeRef, m_pColumnList);
        }

        #endregion
    }
}
