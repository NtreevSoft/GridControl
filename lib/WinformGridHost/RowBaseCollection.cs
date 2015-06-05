using Ntreev.Library.Grid;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Windows.Forms.Grid
{
    public class RowBaseCollection : GridObject, IEnumerable<RowBase>, ICollection
    {
        private readonly IDataRow m_pDataRow;

        internal RowBaseCollection(IDataRow pDataRow)
        {
            m_pDataRow = pDataRow;
        }

        public RowBase this[int index]
        {
            get
            {
                IDataRow pDataRow = m_pDataRow.GetChild(index) as IDataRow;
                return FromNative.Get(pDataRow);
            }
        }

        public int Count
        {
            get { return m_pDataRow.GetChildCount(); }
        }

        #region classes

        class Enumerator : GridObject, IEnumerator<RowBase>
        {
            private readonly IDataRow m_pDataRow;
            private int m_index;

            public Enumerator(IDataRow pDataRow)
            {
                m_pDataRow = pDataRow;
                m_index = -1;
            }

            public bool MoveNext()
            {
                m_index++;
                return m_index < m_pDataRow.GetChildCount();
            }

            public void Reset()
            {
                m_index = -1;
            }

            public RowBase Current
            {
                get
                {
                    IDataRow child = m_pDataRow.GetChild(m_index) as IDataRow;
                    return FromNative.Get(child);
                }
            }

            public void Dispose()
            {

            }

            object IEnumerator.Current
            {
                get { return this.Current; }
            }
        }
        #endregion

        #region ICollection

        void ICollection.CopyTo(Array array, int index)
        {
            throw new NotImplementedException();
        }

        bool ICollection.IsSynchronized
        {
            get { return true; }
        }

        object ICollection.SyncRoot
        {
            get { return this.GridControl; }
        }

        IEnumerator<RowBase> IEnumerable<RowBase>.GetEnumerator()
        {
            return new Enumerator(m_pDataRow);
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return new Enumerator(m_pDataRow);
        }

        #endregion
    }
}
