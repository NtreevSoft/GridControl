using Ntreev.Library.Grid;
using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.ComponentModel.Design;
using System.Drawing.Design;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Ntreev.Windows.Forms.Grid
{
    [EditorAttribute("Design.ColumnCollectionEditor, Design, Version=2.0.4510.20986, Culture=neutral, PublicKeyToken=7a9d7c7c4ba5dfca", typeof(UITypeEditor))]
    public class ColumnCollection : GridObject, IList, IEnumerable<Column>
    {
        GrColumnList m_pColumnList;
        CurrencyManager m_manager;
        ListChangedEventHandler m_listChangedEventHandler;
        List<Column> m_tempBindableColumns;

        internal ColumnCollection(GridControl gridControl)
            : base(gridControl)
        {
            m_pColumnList = this.GridCore.ColumnList;

            m_listChangedEventHandler = new ListChangedEventHandler(currencyManager_ListChanged);

            gridControl.CurrencyManagerChanging += gridControl_CurrencyManagerChanging;
            gridControl.CurrencyManagerChanged += gridControl_CurrencyManagerChanged;
            gridControl.Clearing += gridControl_Clearing;
            gridControl.Cleared += gridControl_Cleared;
        }

        public void Add(Column item)
        {
            Insert(Count, item);
        }

        public void AddRange(IEnumerable<Column> values)
        {
            if (values == null)
                throw new System.ArgumentNullException("values");

            foreach (Column item in values)
            {
                Add(item);
            }
        }

        public Column Add()
        {
            return Insert(this.Count);
        }

        public Column Add(string name)
        {
            return Insert(this.Count, name);
        }

        public Column Add(string name, Type type)
        {
            return Insert(this.Count, name, type);
        }

        public void Insert(int index, Column item)
        {
            if (index < 0 || index > this.Count)
                throw new ArgumentOutOfRangeException("index");
            if (item == null)
                throw new ArgumentNullException("item");
            if (item.Index != -1)
                throw new ArgumentException("이미 추가된 항목입니다.");

            if (this[item.ColumnName] != null)
                throw new System.ArgumentException(string.Format("\"{0}\"의 이름을 가진 항목이 이미 있습니다.", item.ColumnName));

            if (item.ColumnName == string.Empty)
                item.ColumnName = NewColumnName();

            if (this.GridControl.InvokeColumnInserting(item) == false)
                return;

            item.GridControl = GridControl;
            m_pColumnList.InsertColumn(item.NativeRef, index);

            this.GridControl.InvokeColumnInserted(item);
        }

        public Column Insert(int index)
        {
            return Insert(index, NewColumnName());
        }

        public Column Insert(int index, string name)
        {
            return Insert(index, name, typeof(string));
        }

        public Column Insert(int index, string name, Type type)
        {
            if (string.IsNullOrEmpty(name) == true)
                name = NewColumnName();

            if (type == null)
                type = typeof(string);

            Column column = CreateColumnInstance();
            column.ColumnName = name;
            column.DataType = type;
            Insert(index, column);
            return column;
        }

        public void Remove(Column item)
        {
            if (item == null)
                throw new System.ArgumentNullException("item");
            if (item.Index == -1)
                throw new System.ArgumentException("이미 제거된 항목입니다.");

            item.GridControl = null;
            m_pColumnList.RemoveColumn(item.NativeRef);
        }

        public void RemoveAt(int index)
        {
            Column column = this[index];
            Remove(column);
        }

        public int IndexOf(Column item)
        {
            if (item == null)
                throw new System.ArgumentNullException("item");
            return item.Index;
        }

        public bool Contains(Column item)
        {
            if (item == null)
                throw new System.ArgumentNullException("item");
            return item.Index >= 0;
        }

        public void AdjustWidth()
        {
            this.AdjustWidth(true);
        }

        public void AdjustWidth(bool forceUpdate)
        {
            foreach (Column item in this)
            {
                item.AdjustWidth();
            }
            this.GridControl.Update(forceUpdate);
        }

        public Column this[int index]
        {
            get
                {
        if(index < 0 || index >= (int)m_pColumnList.GetColumnCount())
            throw new System.ArgumentOutOfRangeException("index");
        GrColumn pColumn = m_pColumnList.GetColumn((int)index);
        return FromNative.Get(pColumn);
    }
        }

        public int Count
        {
            get
            {
                return m_pColumnList.GetColumnCount();
            }
        }

        public Column this[string columnName]
        {
            get
            {
                if (columnName == null)
                    columnName = string.Empty;

                foreach (Column column in this)
                {
                    if (column.ColumnName == columnName)
                        return column;
                }
                return null;
            }
        }

        internal void DeleteAll()
        {
            this.GridControl.DeleteFocusedCell();

            while (this.Count > 0)
            {
                Column column = this[this.Count - 1];
                column.Dispose();
            }
        }

        internal void SetItemsByDesigner(IEnumerable<object> values)
            {
        GrGroupPanel pGroupPanel = this.GridCore.GroupPanel;
        List<GrColumn> groupings = new List<GrColumn>(pGroupPanel.Groups.Count);

        for(int i=0 ; i<pGroupPanel.Groups.Count ; i++)
        {
            GrColumn pColumn = pGroupPanel.Groups[i].Column;
            groupings.Add(pColumn);
        }

        for(int i=m_pColumnList.GetColumnCount()-1 ; i>=0 && i<m_pColumnList.GetColumnCount() ; i--)
        {
            GrColumn pColumn = m_pColumnList.GetColumn(i);
            m_pColumnList.RemoveColumn(pColumn);
        }

        m_pColumnList.Update(true);

        foreach(var value in groupings)
        {
            value.IsGrouped = false;
        }

        foreach(System.Object item in values)
        {
            Column column = item as Column;
            if(column.ColumnID == GrDefineUtility.INVALID_INDEX)
                (this as IList).Add(item);
            else
                m_pColumnList.AddColumn(column.NativeRef);
        }

        foreach(var value in groupings)
        {
            if(value.GetIndex() == GrDefineUtility.INVALID_INDEX)
                continue;
            value.IsGrouped = true;
        }
    }

        internal static Column CreateColumnInstance(IServiceProvider serviceProvider)
        {
            return CreateColumnInstance(serviceProvider, typeof(Column));
        }

        internal static Column CreateColumnInstance(IServiceProvider serviceProvider, Type columnType)
        {

            if (serviceProvider != null)
            {
                IDesignerHost designerHost = serviceProvider.GetService(typeof(IDesignerHost)) as IDesignerHost;
                if (designerHost != null)
                {
                    return designerHost.CreateComponent(columnType) as Column;
                }
            }
            return TypeDescriptor.CreateInstance(serviceProvider, columnType, null, null) as Column;
        }


        internal Column CreateColumnInstance()
        {
            ServiceProvider serviceProvider = new ServiceProvider(this.GridControl);
            return CreateColumnInstance(serviceProvider, typeof(Column));
        }

        internal Column this[GrColumn pColumn]
        {
            get
            {
                return FromNative.Get(pColumn);
            }
        }


        private string NewColumnName()
        {
            int columnIndex = 1;

            string name;
            do
            {
                name = string.Format("Column{0}", columnIndex++);
            }
            while (this[name] != null);

            return name;
        }

        private void Bind(PropertyDescriptor propertyDescriptor)
        {

            Column existColumn = this[propertyDescriptor.Name];
            Column column = this.GridControl.InvokeColumnBinding(propertyDescriptor, existColumn);

            if (column == null)
                return;

            if (existColumn == null)
                column.HasLifeline = true;

            if (column.GridControl == null)
            {
                column.GridControl = GridControl;
                column.PropertyDescriptor = propertyDescriptor;
                m_pColumnList.AddColumn(column.NativeRef);
            }
            else
            {
                m_pColumnList.RemoveColumn(column.NativeRef);
                m_pColumnList.AddColumn(column.NativeRef);
                column.PropertyDescriptor = propertyDescriptor;
            }

            if (propertyDescriptor.PropertyType == typeof(IBindingList))
                column.IsVisible = false;

            if (column.HasLifeline == true && this.GridControl.ColumnBindingCreation == ColumnBindingCreation.Hidden)
                column.IsVisible = false;

            this.GridControl.InvokeColumnBinded(column);
        }

        private void Unbind(Column column)
        {
            column.PropertyDescriptor = null;

            if (column.HasLifeline == true)
            {
                column.GridControl = null;
                m_pColumnList.RemoveColumn(column.NativeRef);
                m_pColumnList.Update();
                column.Dispose();
            }
        }

        private void currencyManager_ListChanged(object sender, System.ComponentModel.ListChangedEventArgs e)
        {
            switch (e.ListChangedType)
            {
                case ListChangedType.PropertyDescriptorAdded:
                    {
                        if (m_manager.GetItemProperties().Contains(e.PropertyDescriptor) == true)
                        {
                            this.Bind(e.PropertyDescriptor);
                        }
                    }
                    break;
                case ListChangedType.PropertyDescriptorChanged:
                    {
                        CurrencyManager currencyManager = sender as CurrencyManager;

                        List<Column> boundColumns = new List<Column>();

                        foreach (Column item in this)
                        {
                            if (item.PropertyDescriptor != null)
                                boundColumns.Add(item);
                        }

                        PropertyDescriptor changedPropertyDescriptor = null;

                        foreach (PropertyDescriptor item in currencyManager.GetItemProperties())
                        {
                            Column column = this[item.Name];
                            if (column != null)
                            {
                                column.PropertyDescriptor = item;
                                boundColumns.Remove(column);
                            }
                            else
                            {
                                changedPropertyDescriptor = item;
                            }
                        }

                        if (boundColumns.Count == 1 && changedPropertyDescriptor != null)
                        {
                            boundColumns[0].PropertyDescriptor = changedPropertyDescriptor;
                        }
                    }
                    break;
                case ListChangedType.PropertyDescriptorDeleted:
                    {
                        Column column = this[e.PropertyDescriptor.Name];
                        if (column != null)
                        {
                            this.Unbind(column);
                        }
                    }
                    break;
                case ListChangedType.Reset:
                    {
                        //PropertyDescriptorCollection props = m_manager.GetItemProperties();

                        //foreach(PropertyDescriptor item in props)
                        //{
                        //    this.Bind(item);
                        //}
                    }
                    break;
            }
        }

        private void gridControl_CurrencyManagerChanging(object sender, CurrencyManagerChangingEventArgs e)
        {
            CurrencyManager manager = e.CurrecnyManager;

            if (manager == null)
                return;

            if (e.Cancel == true)
                return;

            foreach (System.ComponentModel.PropertyDescriptor item in manager.GetItemProperties())
            {
                Column column = this[item.Name];
                if (column == null || column.HasLifeline == true)
                    continue;

                if (column.CanConvertFrom(item.PropertyType) == false)
                {
                    if (column.CanConvertFrom(column.DataType, item.PropertyType) == true)
                        continue;

                    StringBuilder builder = new StringBuilder();

                    builder.AppendLine(string.Format("{0} 은 연결될 데이터 소스와 데이터 타입이 다릅니다.", item.Name));
                    builder.AppendLine(string.Format(" GridControl : {0}", column.DataType));
                    builder.AppendLine(string.Format(" DataSource : {0}", item.PropertyType));

                    e.Cancel = true;
                    e.CancelReason = builder.ToString();
                }

                if (e.Cancel == true)
                    return;
            }
        }

        private void gridControl_CurrencyManagerChanged(object sender, CurrencyManagerChangedEventArgs e)
        {
            if (m_manager != null)
            {
                m_manager.ListChanged -= m_listChangedEventHandler;
            }

            m_manager = e.CurrecnyManager;

            if (m_manager == null)
                return;

            foreach (PropertyDescriptor item in m_manager.GetItemProperties())
            {
                this.Bind(item);
            }

            m_manager.ListChanged += m_listChangedEventHandler;
        }

        private void gridControl_Clearing(object sender, ClearEventArgs e)
        {
            if (e.DataSourceOnly == true)
            {
                m_tempBindableColumns = new List<Column>();

                foreach (Column item in this)
                {
                    if (item.HasLifeline == false || item.PropertyDescriptor == null)
                    {
                        item.PropertyDescriptor = null;
                        m_tempBindableColumns.Add(item);
                    }
                }

                foreach (Column item in m_tempBindableColumns)
                {
                    this.Remove(item);
                }
            }

            DeleteAll();
        }

        private void gridControl_Cleared(object sender, ClearEventArgs e)
        {
            if (e.DataSourceOnly == true)
            {
                foreach (Column item in m_tempBindableColumns)
                {
                    this.Add(item);
                }
            }
        }

        #region classes

        class Enumerator : IEnumerator<Column>
        {
            int m_index = -1;
            GrColumnList m_pColumnList;

            public Enumerator(GrColumnList pColumnList)
            {
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

            public Column Current
            {
                get
                {
                    GrColumn pColumn = m_pColumnList.GetColumn(m_index);
                    return FromNative.Get(pColumn);
                }
            }

            #region IEnumerator

            object IEnumerator.Current
            {
                get { return this.Current; }
            }

            #endregion
        }

        class ServiceProvider : IServiceProvider
        {
            private readonly GridControl m_gridControl;

            public ServiceProvider(GridControl gridControl)
            {
                m_gridControl = gridControl;
            }

            public object GetService(Type serviceType)
            {
                return m_gridControl.GetInternalService(serviceType);
            }
        }

        #endregion

        #region IList

        int IList.Add(object value)
        {
            int count = this.Count;
            this.Add(value as Column);
            return count;
        }

        void IList.Clear()
        {
            for (int i = this.Count - 1; i >= 0; i--)
            {
                this.RemoveAt(i);
            }
        }

        bool IList.Contains(object value)
        {
            return this.Contains(value as Column);
        }

        int IList.IndexOf(object value)
        {
            return this.IndexOf(value as Column);
        }

        void IList.Insert(int index, object value)
        {
            this.Insert(index, value as Column);
        }

        bool IList.IsFixedSize
        {
            get { return false; }
        }

        bool IList.IsReadOnly
        {
            get { return false; }
        }

        void IList.Remove(object value)
        {
            this.Remove(value as Column);
        }

        object IList.this[int index]
        {
            get { return this[index]; }
            set { throw new NotImplementedException(); }
        }

        void ICollection.CopyTo(Array array, int index)
        {
            foreach (Column item in this)
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
            return new Enumerator(m_pColumnList);
        }

        IEnumerator<Column> IEnumerable<Column>.GetEnumerator()
        {
            return new Enumerator(m_pColumnList);
        }

        #endregion
    }
}
