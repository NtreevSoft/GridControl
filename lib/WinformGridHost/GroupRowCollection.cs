using Ntreev.Library.Grid;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Windows.Forms.Grid
{
    public class GroupRowCollection : GridObject, IEnumerable<GroupRow>, ICollection
    {
        private readonly GrDataRowList m_pDataRowList;

        internal GroupRowCollection(GridControl gridControl)
            : base(gridControl)
        {
            m_pDataRowList = this.GridCore.GetDataRowList();
        }

        public GroupRow this[int index]
        {
            get
            {
                if (index >= this.Count)
                    throw new ArgumentOutOfRangeException("index");

                GrGroupRow pGroupRow = m_pDataRowList.GetChild(index) as GrGroupRow;
                return FromNative.Get(pGroupRow);
            }
        }

        public GroupRow this[string text]
        {
            get
            {
                foreach (GroupRow item in this)
                {
                    if (item.Text == text)
                        return item;
                }
                return null;
            }
        }

        public int Count
        {
            get
            {
                if (this.GridControl.IsGrouped == false)
                    return 0;

                return m_pDataRowList.GetChildCount();
            }
        }

        #region ICollection

        void ICollection.CopyTo(Array array, int index)
        {
            foreach (GroupRow item in this)
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

        IEnumerator<GroupRow> IEnumerable<GroupRow>.GetEnumerator()
        {
            return new Enumerator(this.GridControl);
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return new Enumerator(this.GridControl);
        }

        #endregion

        #region classes

        class Enumerator : GridObject, IEnumerator<GroupRow>
        {
            private int m_index = -1;

            public Enumerator(GridControl gridControl)
            {

            }

            public void Dispose()
            {

            }

            public bool MoveNext()
            {
                GrGridCore pGridCore = this.GridCore;
                int count = 0;
                if (pGridCore.IsGrouped() == true)
                {
                    count = pGridCore.GetDataRowList().GetChildCount();
                }
                m_index++;
                return m_index < count;

            }

            public void Reset()
            {
                m_index = -1;
            }

            public GroupRow Current
            {
                get
                {
                    GrDataRowList pDataRowList = this.GridCore.GetDataRowList();
                    GrGroupRow pGroupRow = pDataRowList.GetChild(m_index) as GrGroupRow;

                    return FromNative.Get(pGroupRow);
                }
            }

            object IEnumerator.Current
            {
                get { return this.Current; }
            }
        }

        #endregion
    }
}
