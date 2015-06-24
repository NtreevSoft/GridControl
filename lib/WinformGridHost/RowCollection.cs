using Ntreev.Library.Grid;
using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Ntreev.Windows.Forms.Grid
{
    public sealed class RowCollection : GridObject, IList<Row>, IList
    {
        private int m_lockRef;
        private GrDataRowList m_pDataRowList;
        private CurrencyManager m_manager;
        private ListChangedEventHandler m_listChangedEventHandler;
        private Dictionary<object, Row> m_componentToRow;
        private List<object> m_components;
        private bool m_insertion;

        internal RowCollection(GridControl gridControl)
            : base(gridControl)
        {
            m_pDataRowList = this.GridCore.GetDataRowList();

            m_listChangedEventHandler = new ListChangedEventHandler(currencyManager_ListChanged);
            //m_currentChangedEventHandler = new System.EventHandler(this, &RowCollection.currencyManager_CurrentChanged);

            gridControl.CurrencyManagerChanging += gridControl_CurrencyManagerChanging;
            gridControl.CurrencyManagerChanged += gridControl_CurrencyManagerChanged;
            gridControl.Cleared += gridControl_Cleared;
            gridControl.FocusedRowChanged += gridControl_FocusedRowChanged;

            m_componentToRow = new Dictionary<object, Row>();
            m_components = new List<object>();
        }

        public Row Add()
        {
            try
            {
                Row row = this.GridControl.CreateRow(m_pDataRowList.NewDataRow());
                this.Insert(this.Count, row);
                return row;
            }
            catch (Exception e)
            {
                throw e;
            }
        }

        public IEnumerable<Row> Add(int count)
        {
            if (count <= 0)
                throw new System.ArgumentOutOfRangeException("count", "생성 갯수는 0보다 커야 합니다.");
            Row[] rows = new Row[count];

            for (int i = 0; i < count; i++)
            {
                rows[i] = Add();
            }
            return rows;
        }

        public Row AddFromInsertion()
        {
            return InsertCore(this.Count, this.InsertionRow);
        }

        public void Add(Row item)
        {
            Insert(this.Count, item);
        }

        public bool Remove(Row item)
            {
        ValidateArgument(item);

        if(item.Index == GrDefineUtility.INVALID_INDEX)
            throw new System.ArgumentException("이미 지워지거나 사용되지 않은 row입니다.");

        if(this.GridControl.InvokeRowRemoving(item) == false)
            return false;

        //ManagerEventDetach managerEventDeatch(this);

        int index = m_manager.List.IndexOf(item.Component);
        if(index < 0)
            throw new System.ArgumentException("이미 지워지거나 사용되지 않은 row입니다.");

        try
        {
            m_manager.RemoveAt(index);
        }
        catch(Exception e)
        {
            this.GridControl.ShowMessage(e.Message, "Error", System.Windows.Forms.MessageBoxButtons.OK, System.Windows.Forms.MessageBoxIcon.Error);
            return false;
        }

        RowRemovedEventArgs eRemoved = new RowRemovedEventArgs(0);
        this.GridControl.InvokeRowRemoved(eRemoved);
        
        return true;
    }

        public bool Contains(Row item)
        {
            ValidateArgument(item);
            return item.Index != GrDefineUtility.INVALID_INDEX;
        }

        public int IndexOf(Row item)
        {
            ValidateArgument(item);
            return item.Index;
        }

        public void Clear()
            {
                using (new ManagerEventDetach(this))
                {

                    for (int i = m_manager.Count - 1; i >= 0; i--)
                    {
                        m_manager.RemoveAt(i);
                    }

                    m_components.Clear();
                    m_componentToRow.Clear();
                    m_pDataRowList.Clear();
                }
    }

        public void Insert(int index, Row item)
        {
            this.InsertCore(index, item);
        }

        public void RemoveAt(int index)
        {
            Row row = this[index];
            this.Remove(row);
        }

        public Row GetAt(int index)
        {
            return this[index];
        }

        public Row this[int index]
        {
            get
            {
                if (index < 0 || index >= m_pDataRowList.GetDataRowCount())
                    throw new System.ArgumentOutOfRangeException("index");

                GrDataRow pDataRow = m_pDataRowList.GetDataRow(index);
                return FromNative.Get(pDataRow);
            }
            set { throw new NotImplementedException(); }
        }

        public int Count
        {
            get
            {
                return (int)m_pDataRowList.GetDataRowCount();
            }
            set
            {
                int count = this.Count;

                if (value > count)
                {
                    Add(value - count);
                }
                else if (value < count)
                {
                    for (int i = count - 1; i >= value; i--)
                    {
                        this.RemoveAt(i);
                    }
                }
            }
        }

        public Row InsertionRow
        {
            get
            {
                GrDataRow pDataRow = m_pDataRowList.GetInsertionRow();
                Row row = Ntreev.Windows.Forms.Grid.FromNative.Get(pDataRow);
                if (row == null)
                    row = this.GridControl.CreateRow(pDataRow);
                return row;
            }
        }

        internal void Bind(object component, int componentIndex)
        {
            Row row;

            if (m_componentToRow.ContainsKey(component) == true)
            {
                row = m_componentToRow[component];
                row.AttachComponent(component);
                m_pDataRowList.RemoveDataRow(row.NativeRef);
                m_pDataRowList.InsertDataRow(row.NativeRef, componentIndex);
                m_components.Remove(component);
                m_components.Insert(componentIndex, component);
                this.GridControl.InvokeRowChanged(row);
                return;
            }

            m_components.Insert(componentIndex, component);

            this.GridControl.InvokeRowBinding(component);

            row = this.GridControl.CreateRow(m_pDataRowList.NewDataRow());
            m_pDataRowList.InsertDataRow(row.NativeRef, componentIndex);
            m_componentToRow.Add(component, row);

            row.AttachComponent(component);
            row.AttachChildControl();

            this.GridControl.InvokeRowBinded(row);
            this.GridControl.InvokeRowChanged(row);
        }

        internal void Unbind(int componentIndex)
        {
            object component = m_components[componentIndex];
            Row row = m_componentToRow[component];
            this.GridControl.InvokeRowUnbinding(row);

            m_components.RemoveAt(componentIndex);
            m_componentToRow.Remove(component);
            m_pDataRowList.RemoveDataRow(row.NativeRef);
            row.DetachComponent();
            row.DetachChildControl();
            this.GridControl.InvokeRowUnbinded(row);
        }

        internal void SetItemsByDesigner(IEnumerable<object> values)
        {
            foreach (object item in values)
            {
                Row row = item as Row;

                if (row.Index < 0)
                {
                    Add(row);
                }
            }
        }

        internal void AttachManagerEvent()
        {
            m_lockRef++;
            if (m_lockRef == 0)
            {
                if (m_manager != null)
                {
                    m_manager.ListChanged += m_listChangedEventHandler;
                    //m_manager.CurrentChanged += m_currentChangedEventHandler;
                }
            }
        }

        internal void DetachManagerEvent()
        {
            if (m_lockRef == 0)
            {
                if (m_manager != null)
                {
                    m_manager.ListChanged -= m_listChangedEventHandler;
                    //m_manager.CurrentChanged -= m_currentChangedEventHandler;
                }
            }
            m_lockRef--;
        }

        internal void BeginInsertion()
        {
            if (this.InsertionRow.Component != null)
                return;

            m_insertion = true;
            m_manager.AddNew();
            if (this.InsertionRow.IsBeingEdited == false)
                this.InsertionRow.BeginEdit();
            this.InsertionRow.AttachComponent(m_manager.Current);
        }

        internal void EndInsertion()
        {
            if (this.InsertionRow.Component == null)
                return;

            this.InsertionRow.DetachComponent();
            m_insertion = true;
            m_manager.CancelCurrentEdit();
        }

        internal Row this[GrDataRow pDataRow]
        {
            get
            {
                return FromNative.Get(pDataRow);
            }
        }

        private void currencyManager_ListChanged(object sender, ListChangedEventArgs e)
        {
            switch (e.ListChangedType)
            {
                case ListChangedType.ItemAdded:
                    {
                        int componentIndex = e.NewIndex;
                        var component = m_manager.List[componentIndex];
                        if (m_insertion == true)
                        {
                            m_components.Add(component);
                            m_componentToRow.Add(component, this.InsertionRow);
                            m_insertion = false;
                        }
                        else
                        {
                            this.Bind(component, componentIndex);
                        }
                    }
                    break;
                case ListChangedType.ItemDeleted:
                    {
                        var component = m_components[e.NewIndex];
                        if (m_insertion == true)
                        {
                            m_components.RemoveAt(e.NewIndex);
                            m_componentToRow.Remove(component);
                            m_insertion = false;
                        }
                        else
                        {
                            this.Unbind(e.NewIndex);
                        }
                    }
                    break;
                case ListChangedType.ItemChanged:
                    {
                        object component = m_manager.List[e.NewIndex];

                        Row row = m_componentToRow[component];
                        row.InvokeChanged(e.PropertyDescriptor);
                        this.GridControl.InvokeRowChanged(row);
                    }
                    break;
                case ListChangedType.ItemMoved:
                    {
                        int oldIndex = e.OldIndex;
                        int newIndex = e.NewIndex;
                        object component = m_components[oldIndex];
                        m_components.RemoveAt(oldIndex);
                        m_components.Insert(newIndex, component);

                        Row row = m_componentToRow[component];

                        m_pDataRowList.MoveDataRow(row.NativeRef, oldIndex, newIndex);
                    }
                    break;
                case ListChangedType.Reset:
                    {
                        var components = m_components;
                        var componentToRow = m_componentToRow;
                        m_components = new List<object>(components.Count);
                        m_componentToRow = new Dictionary<object, Row>(m_componentToRow.Count);

                        if (m_manager.List.Count == 0)
                        {
                            m_pDataRowList.Clear();
                        }
                        else
                        {
                            List<GrDataRow> nativeRows = new List<GrDataRow>(m_manager.List.Count);

                            foreach (object item in m_manager.List)
                            {
                                if (item == this.GridControl.InsertionRow.Component)
                                    continue;

                                Row row = componentToRow[item];
                                m_components.Add(item);
                                m_componentToRow.Add(item, row);
                                nativeRows.Add(row.NativeRef);
                            }
                            m_pDataRowList.Reset(nativeRows);
                        }

                        if (this.InsertionRow.Component != null)
                        {
                            m_components.Add(this.InsertionRow.Component);
                            m_componentToRow.Add(this.InsertionRow.Component, this.InsertionRow);
                        }

                        this.GridControl.InvokeReset();
                    }
                    break;
            }
        }

        private void gridControl_CurrencyManagerChanging(object sender, CurrencyManagerChangingEventArgs e)
        {

        }

        private void gridControl_CurrencyManagerChanged(object sender, CurrencyManagerChangedEventArgs e)
        {
            DetachManagerEvent();

            m_manager = e.CurrecnyManager;
            m_components.Clear();
            m_componentToRow.Clear();

            if (m_manager == null)
                return;

            int componentIndex = 0;
            foreach (object item in m_manager.List)
            {
                this.Bind(item, componentIndex++);
            }

            AttachManagerEvent();
        }

        private void gridControl_Cleared(object sender, ClearEventArgs e)
        {
            m_components.Clear();
            m_componentToRow.Clear();
        }

        private void gridControl_FocusedRowChanged(object sender, System.EventArgs e)
        {
            using (ManagerEventDetach managerEventDeatch = new ManagerEventDetach(this))
            {

                Row row = this.GridControl.FocusedRow as Row;

                if (row != null && row != this.InsertionRow)
                {
                    if (m_manager.Position >= 0 && m_manager.Current != row.Component)
                    {
                        m_manager.Position = m_manager.List.IndexOf(row.Component);
                    }
                }
            }
        }

        private void ValidateArgument(Row item)
        {
            if (item == null)
                throw new System.ArgumentNullException("item");
            if (item == InsertionRow)
                throw new System.ArgumentException();
        }

        private Row InsertCore(int index, Row item)
        {
            if (index < 0 || index > Count)
                throw new System.ArgumentOutOfRangeException("index");
            if (item == null)
                throw new System.ArgumentNullException("item");
            if (item.Index != -1)
                throw new System.ArgumentException();

            bool isNew = false;
            if (item.Component == null)
            {
                try
                {
                    m_insertion = true;
                    m_manager.AddNew();
                    isNew = true;
                }
                catch (Exception)
                {
                    return null;
                }
                item.AttachComponent(m_manager.Current);
            }

            if (this.GridControl.InvokeRowInserting(item) == false)
            {
                if (isNew == true)
                {
                    item.DetachComponent();
                    m_insertion = false;
                    m_manager.CancelCurrentEdit();
                }
            }
            else
            {
                try
                {
                    m_pDataRowList.InsertDataRow(item.NativeRef, index);
                    m_manager.EndCurrentEdit();
                }
                catch (Exception e)
                {
                    m_pDataRowList.RemoveDataRow(item.NativeRef);
                    if (isNew == true)
                    {
                        item.DetachComponent();
                        m_insertion = true;
                        m_manager.CancelCurrentEdit();
                    }
                    this.GridControl.ShowMessage(e.Message, "Error", System.Windows.Forms.MessageBoxButtons.OK, System.Windows.Forms.MessageBoxIcon.Error);
                    return null;
                }

                item.AttachChildControl();
                item.EndEditInternal();
                this.GridControl.InvokeRowInserted(item);
                this.InsertionRow.UpdateNativeText();
                this.GridControl.InvokeRowChanged(item);
            }
            return item;
        }

        #region classes

        class Enumerator : IEnumerator<Row>
        {
            private int m_index = -1;
            private GrDataRowList m_pDataRowList;

            public Enumerator(GrDataRowList pDataRowList)
            {
                m_pDataRowList = pDataRowList;
            }

            public void Dispose()
            {

            }

            public bool MoveNext()
            {
                m_index++;
                return m_index < m_pDataRowList.GetDataRowCount();
            }

            public void Reset()
            {
                m_index = -1;
            }

            public Row Current
            {
                get
                {
                    GrDataRow pDataRow = m_pDataRowList.GetDataRow(m_index);
                    return FromNative.Get(pDataRow);
                }
            }

            #region IEnumerator

            object IEnumerator.Current
            {
                get { return this.Current; }
            }

            #endregion
        }

        class ManagerEventDetach : IDisposable
        {
            private RowCollection m_rows;

            public ManagerEventDetach(RowCollection rows)
            {
                m_rows = rows;
                m_rows.DetachManagerEvent();
            }

            public void Dispose()
            {
                m_rows.AttachManagerEvent();
            }
        }

        #endregion

        #region IList

        void ICollection<Row>.CopyTo(Row[] array, int arrayIndex)
        {
            foreach (Row item in this)
            {
                array.SetValue(item, arrayIndex++);
            }
        }

        int ICollection<Row>.Count
        {
            get { return this.Count; }
        }

        bool ICollection<Row>.IsReadOnly
        {
            get { return false; }
        }

        bool IList.IsReadOnly
        {
            get { return false; }
        }

        IEnumerator<Row> IEnumerable<Row>.GetEnumerator()
        {
            return new Enumerator(m_pDataRowList);
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return new Enumerator(m_pDataRowList);
        }

        int IList.Add(object value)
        {
            int count = this.Count;
            this.Add(value as Row);
            return count;
        }

        bool IList.Contains(object value)
        {
            return this.Contains(value as Row);
        }

        int IList.IndexOf(object value)
        {
            return this.IndexOf(value as Row);
        }

        void IList.Insert(int index, object value)
        {
            this.Insert(index, value as Row);
        }

        bool IList.IsFixedSize
        {
            get { return false; }
        }

        void IList.Remove(object value)
        {
            throw new NotImplementedException();
        }

        object IList.this[int index]
        {
            get
            {
                return this[index];
            }
            set
            {
                throw new NotImplementedException();
            }
        }

        void ICollection.CopyTo(Array array, int index)
        {
            foreach (Row item in this)
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

        #endregion
    }
}