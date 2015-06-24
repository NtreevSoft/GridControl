using Ntreev.Library.Grid;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Linq;
using System.Text;

namespace Ntreev.Windows.Forms.Grid
{
    [TypeConverter(typeof(ExpandableObjectConverter))]
    public class Row : RowBase
    {
        CellCollection m_cells;
        CellTagCollection m_cellTags;
        GrDataRow m_pDataRow;
        object m_component;
        //int m_componentIndex;
        int m_editedCount;
        bool m_editing;

        string m_error = string.Empty;
        string m_sourceError = string.Empty;

        Dictionary<Cell, string> m_errors;
        Dictionary<Cell, string> m_sourceErrors;
        Dictionary<Cell, string> m_invalidValues;

        internal int m_textCapacity;

        protected Row(RowBuilder rowBuilder)
            : base(rowBuilder.GridControl, rowBuilder.NativeRef)
        {
            m_pDataRow = rowBuilder.NativeRef;
            m_cells = new CellCollection(this);

            foreach (Column item in this.GridControl.Columns)
            {
                this.NewCell(item);
            }
        }

        internal Row(RowBuilder rowBuilder, GridControl gridControl)
            : this(rowBuilder)
        {

        }

        public override string ToString()
        {
            return this.Index.ToString();
        }

        public void BeginEdit()
        {
            m_editing = true;
        }

        public void CancelEdit()
        {
            try
            {
                if (m_editedCount == 0)
                    return;

                foreach (Cell cell in m_cells)
                {
                    cell.CancelEditInternal();
                }

                if (m_editedCount < 0)
                    throw new Exception("먼가 수상합니다.");
            }
            finally
            {
                m_editedCount = 0;
                m_editing = false;
            }

            foreach (Cell item in m_cells)
            {
                item.UpdateNativeText();
            }
        }

        public void EndEdit()
        {
            this.GridControl.EndCurrentEdit(this);
        }

        public void BringIntoView()
        {
            if (m_pDataRow.GetDisplayable() == true)
                return;
            this.GridControl.BringIntoView(this);
        }

        public void Select()
        {
            this.Selector.SelectDataRow(m_pDataRow, GrSelectionType.Normal);
        }

        public void Select(SelectionType selectionType)
        {
            this.Selector.SelectDataRow(m_pDataRow, (GrSelectionType)selectionType);
        }

        public void ResetCellForeColor()
        {
            this.CellForeColor = System.Drawing.Color.Empty;
        }

        public void ResetCellBackColor()
        {
            this.CellBackColor = System.Drawing.Color.Empty;
        }

        public Cell GetAt(int index)
        {
            return this.Cells[index];
        }

        public int GetCellsTextCapacity()
        {
            return m_textCapacity;
        }

        [Description("셀들의 컬렉션입니다.")]
        [Category("Appearance")]
        public CellCollection Cells
        {
            get { return m_cells; }
        }

        [Description("셀태그의 컬렉션입니다.")]
        [Category("Appearance")]
        public CellTagCollection CellTags
        {
            get
            {
                if (m_cellTags == null)
                {
                    m_cellTags = new CellTagCollection(this);
                }
                return m_cellTags;
            }
        }

#if DEBUG
        [Category("Debug")]
        [Description("셀들의 갯수입니다.")]
#else
        [Browsable(false)]
#endif
        public int CellCount
        {
            get { return this.Cells.Count; }
        }

        public object this[int index]
        {
            get { return this.Cells[index].Value; }
            set { this.Cells[index].Value = value; }
        }

        public object this[string columnName]
        {
            get { return m_cells[columnName].Value; }
            set { m_cells[columnName].Value = value; }
        }

        public object this[Column column]
        {
            get { return this.Cells[column].Value; }
            set { this.Cells[column].Value = value; }
        }

#if DEBUG
        [Category("Debug")]
        [Description("열의 고유 아이디입니다.")]
#else
        [Browsable(false)]
#endif
        public int RowID
        {
            get { return m_pDataRow.GetDataRowID(); }
        }

        [Description("표시되고 있는지의 여부를 가져오거나 설정합니다.")]
        [Category("Appearance")]
        [DefaultValue(true)]
        public override bool IsVisible
        {
            get { return m_pDataRow.GetVisible(); }
            set { m_pDataRow.SetVisible(value); }
        }

        [Description("읽기 전용인지에 대한 여부를 가져오거나 설정합니다.")]
        [Category("Appearance")]
        [DefaultValue(false)]
        public bool IsReadOnly
        {
            get { return m_pDataRow.GetReadOnly(); }
            set { m_pDataRow.SetReadOnly(value); }
        }

        [Description("선택되어 있는지의 여부를 가져오거나 설정합니다.")]
        [Category("Appearance")]
        [DefaultValue(false)]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        public bool IsSelected
        {
            get { return m_pDataRow.GetSelected(); }
            set { m_pDataRow.SetSelected(value); }
        }

#if DEBUG
        [Category("Debug")]
        [Description("소유하고 있는 셀의 대한 편집 여부를 가져옵니다.")]
#else
        [Browsable(false)]
#endif
        public bool IsBeingEdited
        {
            get { return m_editing; }
        }

#if DEBUG
        [Category("Debug")]
        [Description("소유하고 있는 셀의 대한 편집 여부를 가져옵니다.")]
#else
        [Browsable(false)]
#endif
        public bool IsEdited
        {
            get { return m_editedCount > 0 ? true : false; }
        }

#if DEBUG
        [Category("Debug")]
        [Description("컨트롤에 내에 Row 인덱스를 가져옵니다.")]
#else
        [Browsable(false)]
#endif
        public int Index
        {
            get { return m_pDataRow.GetDataRowIndex(); }
        }

#if DEBUG
        [Category("Debug")]
        [Description("소유하고 있는 모든 셀이 선택되었는지의 여부를 가져옵니다.")]
#else
        [Browsable(false)]
#endif
        public bool IsFullSelected
        {
            get { return m_pDataRow.GetFullSelected(); }
        }

        [Description("셀들의 전경색입니다")]
        [Category("Appearance")]
        public Color CellForeColor
        {
            get { return m_pDataRow.GetItemForeColor(); }
            set { m_pDataRow.SetItemForeColor(value); }
        }

        /// <summary>
        /// <see cref="Cells"/>의 배경색을 가져오거나 설정합니다.
        /// </summary>
        /// <returns>
        /// <see cref="Cells"/>의 배경색을 나타내는 <see cref="Color"/>입니다.
        /// </returns>
        [Description("셀들의 배경색입니다")]
        [Category("Appearance")]
        public Color CellBackColor
        {
            get { return m_pDataRow.GetItemBackColor(); }
            set { m_pDataRow.SetItemBackColor(value); }
        }

        [DefaultValue("")]
        public string Error
        {
            get { return m_error; }
            set
            {
                value = value == null ? System.String.Empty : value;

                if (m_error == value)
                    return;

                m_error = value;
                if (m_error == System.String.Empty)
                {
                    if (m_errors.Count == 0)
                        this.GridControl.ErrorDescriptor.Remove(this);
                }
                else
                {
                    this.GridControl.ErrorDescriptor.Add(this);
                }
            }
        }

        public bool HasErrors
        {
            get
            {
                if (System.String.IsNullOrEmpty(m_error) == false || System.String.IsNullOrEmpty(m_sourceError) == false)
                    return true;
                if (m_errors != null && m_errors.Count > 0)
                    return true;
                if (m_sourceErrors != null && m_sourceErrors.Count > 0)
                    return true;
                if (m_invalidValues != null && m_invalidValues.Count > 0)
                    return false;
                return false;
            }
        }

        public object Component
        {
            get { return m_component; }
            //internal:
            //    void set(object);
        }

        protected virtual Cell NewCellFromBuilder(CellBuilder builder)
        {
            return new Cell(builder.GridControl, builder.NativeRef);
        }

        internal void AddEditedCell()
        {
            m_editedCount++;
        }

        internal void RemoveEditedCell()
        {
            m_editedCount--;
            if (m_editedCount < 0)
                throw new Exception();
        }

        internal void AttachChildControl()
        {
            foreach (Column item in this.GridControl.Columns)
            {
                if (item.PropertyDescriptor != null && item.PropertyDescriptor.PropertyType == typeof(IBindingList))
                {
                    GrGridRow pChildRow = new GrGridRow(this.GridControl, item.PropertyDescriptor, this);
                    m_pDataRow.AddChild(pChildRow);
                }
            }
        }

        internal void DetachChildControl()
        {
            foreach (RowBase item in this.Childs)
            {
                GridRow gridRow = item as GridRow;
                if (gridRow == null)
                    continue;

                this.GridControl.Controls.Remove(gridRow.ChildGrid);
                //delete gridRow.ChildGrid;
            }
        }

        internal Cell NewCell(Column column)
        {
            GrItem pItem = m_pDataRow.GetItem(column.NativeRef);

            Cell cell = FromNative.Get(pItem);
            if (cell == null)
            {
                CellBuilder builder = new CellBuilder();
                builder.GridControl = this.GridControl;
                builder.NativeRef = pItem;
                cell = this.NewCellFromBuilder(builder);
            }

            return cell;
        }

        internal void InvokeChanged(PropertyDescriptor descriptor)
        {
            //if(m_component == null)
            //	throw new System.ArgumentException();

            IDataErrorInfo dataErrorInfo = m_component as IDataErrorInfo;

            if (dataErrorInfo != null)
            {
                this.SourceError = dataErrorInfo.Error;
            }

            if (descriptor == null)
            {
                foreach (Cell item in m_cells)
                {
                    item.UpdateNativeText();

                    Column column = item.Column;

                    if (column.PropertyDescriptor != null && column.PropertyDescriptor.PropertyType == typeof(IBindingList))
                    {
                        for (int i = 0; i < m_pDataRow.GetChildCount(); i++)
                        {
                            GrGridRow childRow = m_pDataRow.GetChild(i) as GrGridRow;
                            if (childRow != null)
                            {
                                if (childRow.GetPropertyDescriptor() == column.PropertyDescriptor)
                                {
                                    childRow.Update();
                                }
                            }
                        }
                    }

                    if (dataErrorInfo != null && column.PropertyDescriptor != null && column.PropertyDescriptor.PropertyType != typeof(IBindingList))
                    {
                        this.SetSourceError(item, dataErrorInfo[column.ColumnName]);
                    }
                }
            }
            else
            {
                Cell cell = m_cells[descriptor.Name];
                cell.UpdateNativeText();

                if (dataErrorInfo != null)
                {
                    this.SetSourceError(cell, dataErrorInfo[descriptor.Name]);
                }
                this.GridControl.InvokeValueChanged(cell);
            }
        }

        internal void UpdateNativeText()
        {
            foreach (Cell item in m_cells)
            {
                item.UpdateNativeText();
            }
        }

        internal string GetError(Cell cell)
        {
            if (m_errors == null)
                return string.Empty;

            if (m_errors.ContainsKey(cell) == false)
                return string.Empty;

            return m_errors[cell];
        }

        internal void SetError(Cell cell, string text)
        {
            text = text == null ? string.Empty : text;

            if (text == string.Empty)
            {
                if (m_errors == null)
                    return;

                if (m_errors.ContainsKey(cell) == true)
                    m_errors.Remove(cell);

                if (this.HasErrors == false)
                    this.GridControl.ErrorDescriptor.Remove(this);
            }
            else
            {
                if (m_errors == null)
                    m_errors = new Dictionary<Cell, string>();

                if (m_errors.ContainsKey(cell) == false || m_errors[cell] != text)
                {
                    m_errors[cell] = text;
                    this.GridControl.ErrorDescriptor.Add(this);
                }
            }
        }

        internal string GetSourceError(Cell cell)
        {
            if (m_sourceErrors == null)
                return string.Empty;

            if (m_sourceErrors.ContainsKey(cell) == false)
                return string.Empty;

            return m_sourceErrors[cell];
        }

        internal void SetSourceError(Cell cell, string text)
        {
            text = text == null ? string.Empty : text;

            if (text == string.Empty)
            {
                if (m_sourceErrors == null)
                    return;

                if (m_sourceErrors.ContainsKey(cell) == true)
                    m_sourceErrors.Remove(cell);

                if (this.HasErrors == false)
                    this.GridControl.ErrorDescriptor.Remove(this);
            }
            else
            {
                if (m_sourceErrors == null)
                    m_sourceErrors = new Dictionary<Cell, string>();

                if (m_sourceErrors.ContainsKey(cell) == false || m_sourceErrors[cell] != text)
                {
                    m_sourceErrors[cell] = text;
                    this.GridControl.ErrorDescriptor.Add(this);
                }
            }
        }

        internal string GetInvalidValue(Cell cell)
        {
            if (m_invalidValues == null)
                return string.Empty;

            if (m_invalidValues.ContainsKey(cell) == false)
                return string.Empty;

            return m_invalidValues[cell];
        }

        internal void SetInvalidValue(Cell cell, string text)
        {
            text = text == null ? string.Empty : text;

            if (text == string.Empty)
            {
                if (m_invalidValues == null)
                    return;

                if (m_invalidValues.ContainsKey(cell) == true)
                    m_invalidValues.Remove(cell);

                if (this.HasErrors == false)
                    this.GridControl.ErrorDescriptor.Remove(this);
            }
            else
            {
                if (m_invalidValues == null)
                    m_invalidValues = new Dictionary<Cell, string>();

                if (m_invalidValues.ContainsKey(cell) == false || m_invalidValues[cell] != text)
                {
                    m_invalidValues[cell] = text;
                    this.GridControl.ErrorDescriptor.Add(this);
                }
            }
        }

        internal bool HasInvalidValue(Cell cell)
        {
            if (m_invalidValues == null)
                return false;

            return m_invalidValues.ContainsKey(cell);
        }

        internal void EndEditInternal()
        {
            try
            {
                if (m_editedCount == 0)
                    return;

                foreach (Cell cell in m_cells)
                {
                    cell.EndEditInternal();
                }

                if (m_editedCount < 0)
                    throw new Exception("먼가 수상합니다.");
            }
            finally
            {
                m_editedCount = 0;
                m_editing = false;
            }

            foreach (Cell item in m_cells)
            {
                item.UpdateNativeText();
            }
            this.GridControl.InvokeRowChanged(this);
        }

        internal void AttachComponent(object component)
        {
            m_component = component;

            foreach (Cell item in m_cells)
            {
                try
                {
                    item.LocalValueToSource(component);
                    item.UpdateNativeText();
                }
                catch (Exception)
                {

                }
            }

            System.ComponentModel.IDataErrorInfo dataErrorInfo = m_component as IDataErrorInfo;

            if (dataErrorInfo != null)
            {
                if (string.IsNullOrEmpty(dataErrorInfo.Error) == false)
                    this.SourceError = dataErrorInfo.Error;

                foreach (Cell item in m_cells)
                {
                    Column column = item.Column;
                    if (column.PropertyDescriptor == null || column.PropertyDescriptor.PropertyType == typeof(IBindingList))
                        continue;

                    string error = dataErrorInfo[column.ColumnName];
                    if (string.IsNullOrEmpty(error) == false)
                        this.SetSourceError(item, error);
                }
            }
        }

        internal void DetachComponent()
        {
            {
                var oldComponent = m_component;
                m_component = null;
                foreach (Cell item in m_cells)
                {
                    try
                    {
                        if (this.HasInvalidValue(item) == true)
                            continue;
                        item.SourceValueToLocal(oldComponent);
                        //item.UpdateNativeText();
                    }
                    catch (Exception)
                    {

                    }
                }
            }
        }

        internal string SourceError
        {
            get;
            set;
        }

        internal Dictionary<Cell, string> Errors
        {
            get { return m_errors; }
        }

        internal Dictionary<Cell, string> SourceErrors
        {
            get { return m_sourceErrors; }
        }

        internal Dictionary<Cell, string> InvalidValues
        {
            get { return m_invalidValues; }
        }

        internal new GrDataRow NativeRef
        {
            get { return m_pDataRow; }
        }

        private bool ShouldSerializeCellForeColor()
        {
            return m_pDataRow.GetItemForeColor() != GrColor.Empty;
        }

        private bool ShouldSerializeCellBackColor()
        {
            return m_pDataRow.GetItemBackColor() != GrColor.Empty;
        }

        private bool ShouldSerializeCellFont()
        {
            return m_pDataRow.GetItemFont() != null;
        }

        private object GetSourceValue(Column column)
        {
            PropertyDescriptor propertyDescriptor = column.PropertyDescriptor;

            if (propertyDescriptor == null || m_component == null)
                throw new System.ArgumentException();
            System.Object value = propertyDescriptor.GetValue(m_component);
            return column.ConvertFromSource(value);
        }

        private void SetSourceValue(Column column, object value)
        {
            PropertyDescriptor propertyDescriptor = column.PropertyDescriptor;

            if (propertyDescriptor.IsReadOnly == false)
            {
                value = column.ConvertToSource(value);
                try
                {
                    //if(propertyDescriptor.ShouldSerializeValue(component) == false)
                    propertyDescriptor.SetValue(m_component, value);
                }
                catch (Exception e)
                {
                    if (value == null)
                        propertyDescriptor.SetValue(m_component, System.DBNull.Value);
                    else
                        throw e;
                }
            }
        }

        private bool HasSourceValue(Column column)
        {
            PropertyDescriptor propertyDescriptor = column.PropertyDescriptor;
            if (propertyDescriptor == null || m_component == null)
                return false;
            return true;
        }
    }
}
