using Ntreev.Library.Grid;
using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Ntreev.Windows.Forms.Grid
{
    /// <summary>
    /// 열을 나타냅니다.
    /// </summary>
    [TypeConverter(typeof(ExpandableObjectConverter))]
    [ToolboxItem(false)]
    [DesignTimeVisible(false)]
    [Designer("Design.ColumnDesigner, Design, Version=2.0.4510.20986, Culture=neutral, PublicKeyToken=7a9d7c7c4ba5dfca")]
    public class Column : CellBase, IColumn, ITextCacheProvider, IComponent, IServiceProvider
    {
        Type m_dataType;
        string m_title;
        string m_name;
        string m_tooltip;
        IComparer m_sortComparer;
        TypeConverter m_typeConverter;
        PropertyDescriptor m_propertyDescriptor;

        object m_defaultValue;
        ColumnPainter m_columnPainter;
        Design.TypeEditor m_typeEditor;

        ISite m_site;

        CustomPaint m_pCustomPaint;
        GrColumn m_pColumn;

        public Column()
            : base(new GrColumn())
        {
            m_pColumn = base.NativeRef as GrColumn;

            m_pColumn.SetSortComparer(GrSort.Up, RowComparerUp);
            m_pColumn.SetSortComparer(GrSort.Down, RowComparerDown);

            m_pCustomPaint = new CustomPaint(this);
            SetEditStyleToNative();
        }

        public void Dispose()
        {
            lock (this)
            {
                if ((m_site != null) && (m_site.Container != null))
                {
                    m_site.Container.Remove(this);
                }

                this.Disposed(this, EventArgs.Empty);

                if (m_pColumn != null)
                {
                    if (m_pColumn.GetIndex() != GrDefineUtility.INVALID_INDEX)
                    {
                        GrColumnList pColumnList = m_pColumn.ColumnList;
                        if (this.GridControl != null)
                            this.GridControl = null;
                        pColumnList.RemoveColumn(m_pColumn);
                    }
                    m_pColumn.ManagedRef = null;
                    m_pColumn = null;
                }
            }
        }

        public void Focus()
        {
            GrFocuser pFocuser = this.Focuser;
            if (pFocuser != null)
                pFocuser.Set(m_pColumn);
        }

        public void Select(SelectionType selectionType)
        {
            GrItemSelector pItemSelector = this.Selector;
            if (pItemSelector != null)
                pItemSelector.SelectColumn(m_pColumn, (GrSelectionType)selectionType);
        }

        public void Refresh()
        {
            //return;
            foreach (Row item in this.GridControl.Rows)
            {
                try
                {
                    Cell cell = item.Cells[this];
                    cell.UpdateNativeText();
                }
                catch (Exception)
                {

                }
            }
        }

        public void BringIntoView()
        {
            if (this.GridControl == null)
                return;
            this.GridControl.BringIntoView(this);
        }

        public void DisplayFirst()
        {
            if (this.GridControl == null)
                return;
            this.GridControl.DisplayFirst(this);
        }

        public void AdjustWidth()
        {
            m_pColumn.SetFit();
        }

        public override string ToString()
        {
            return this.ColumnName;
        }


        public void ResetCellForeColor()
        {
            this.CellForeColor = Color.Empty;
        }

        public void ResetCellBackColor()
        {
            this.CellBackColor = Color.Empty;
        }

        public void ResetCellPadding()
        {
            this.CellPadding = GrPadding.Empty;
        }

        public void ResetCellFont()
        {
            m_pColumn.ItemFont = null;
        }


        public virtual void PaintValue(Graphics graphics, Rectangle paintRect, ICell cell, object value)
        {
            if (m_typeEditor != null)
                m_typeEditor.PaintValue(graphics, paintRect, cell, value);
        }



        [Category("Layout")]
        public string Title
        {
            get
            {
                if (m_title == null)
                {
                    if (m_propertyDescriptor != null && m_propertyDescriptor.DisplayName != m_propertyDescriptor.Name)
                    {
                        return m_propertyDescriptor.DisplayName;
                    }
                    return this.ColumnName;
                }
                return m_title;
            }
            set
            {
                m_title = value;
                SyncDisplayText();
            }
        }

        [RefreshProperties(RefreshProperties.Repaint)]
        [Category("Layout")]
        public string ColumnName
        {
            get
            {
                if (m_propertyDescriptor != null)
                    return m_propertyDescriptor.Name;

                if (m_name == null)
                    return string.Empty;

                return m_name;
            }
            set
            {
                if (value == m_name)
                    return;

                if (m_propertyDescriptor != null)
                    throw new System.InvalidOperationException("바인딩된 컬럼에는 이름을 설정할 수 없습니다.");

                if (string.IsNullOrEmpty(value) == true)
                    throw new System.ArgumentException("빈 문자열의 이름은 설정할 수 없습니다.");

                if (this.GridControl != null)
                {
                    if (this.GridControl.Columns[value] != null)
                        throw new System.ArgumentException(string.Format("\"{0}\"의 이름을 가진 항목이 이미 있습니다.", value));
                }

                m_name = value;

                if (m_title == null)
                {
                    this.Title = null;
                }
            }
        }

        [DefaultValue("")]
        [Category("Layout")]
        public string Tooltip
        {
            get
            {
                if (m_tooltip == null)
                {
                    if (m_propertyDescriptor != null)
                        return m_propertyDescriptor.Description;
                    return string.Empty;
                }
                return m_tooltip;
            }
            set
            {
                if (value == null)
                    value = string.Empty;
                m_tooltip = value;
                m_pColumn.SetTooltip(value);
            }
        }

        [Browsable(false)]
        public int ColumnID
        {
            get
            {
                return m_pColumn.GetColumnID();
            }
        }

        [Category("Layout")]
        [SettingsBindable(true)]
        public override int Width
        {
            get
            {
                return base.Width;
            }
            set
            {
                m_pColumn.Width = value;
            }
        }

        [Category("Layout")]
        [DefaultValue(0)]
        public int MinWidth
        {
            get { return m_pColumn.MinWidth; }
            set { m_pColumn.MinWidth = value; }
        }

        [Category("Layout")]
        [DefaultValue(0)]
        public int MaxWidth
        {
            get { return m_pColumn.MaxWidth; }
            set { m_pColumn.MaxWidth = value; }
        }

        [Category("Behavior")]
        [DefaultValue(true)]
        [SettingsBindable(true)]
        public bool IsVisible
        {
            get
            {
                if (m_propertyDescriptor != null && m_propertyDescriptor.IsBrowsable == false)
                    return false;
                return m_pColumn.IsVisible;
            }
            set
            {
                m_pColumn.IsVisible = value;
            }
        }

        [Category("Behavior")]
        [DefaultValue(true)]
        [SettingsBindable(true)]
        public bool IsMovable
        {
            get { return m_pColumn.IsMovable; }
            set { m_pColumn.IsMovable = value; }
        }

        [Category("Behavior")]
        [DefaultValue(true)]
        [SettingsBindable(true)]
        public bool IsResizable
        {
            get { return m_pColumn.IsResizable; }
            set { m_pColumn.IsResizable = value; }
        }

        [Category("Behavior")]
        [DefaultValue(false)]
        [SettingsBindable(true)]
        public bool IsFrozen
        {
            get { return m_pColumn.IsFrozen; }
            set { m_pColumn.IsFrozen = value; }
        }

        [Category("Behavior")]
        [DefaultValue(false)]
        [SettingsBindable(true)]
        public bool IsReadOnly
        {
            get
            {
                if (m_propertyDescriptor != null && m_propertyDescriptor.IsReadOnly == true)
                    return true;

                return m_pColumn.IsReadOnly;
            }
            set
            {
                if (m_propertyDescriptor != null && m_propertyDescriptor.IsReadOnly == true)
                {
                    throw new System.InvalidOperationException("읽기 전용으로 바인딩된 컬럼에는 설정할 수 없습니다.");
                }
                m_pColumn.IsReadOnly = value;
            }
        }

        [Category("Behavior")]
        [DefaultValue(true)]
        public bool IsSortable
        {
            get { return m_pColumn.IsSortable; }
            set { m_pColumn.IsSortable = value; }
        }

        [Browsable(false)]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        public bool IsSelected
        {
            get { return m_pColumn.IsSelected; }
            set { m_pColumn.IsSelected = value; }
        }

        [Browsable(false)]
        public bool IsFullSelected
        {
            get { return m_pColumn.IsFullSelected; }
        }

        [Category("Behavior")]
        [DefaultValue(false)]
        [SettingsBindable(true)]
        public bool IsGrouped
        {
            get { return m_pColumn.IsGrouped; }
            set { m_pColumn.IsGrouped = value; }
        }

        [Category("Behavior")]
        [DefaultValue(true)]
        [SettingsBindable(true)]
        public bool IsGroupable
        {
            get { return m_pColumn.IsGroupable; }
            set { m_pColumn.IsGroupable = value; }
        }

        [Browsable(false)]
        public bool IsBinded
        {
            get
            {
                return m_propertyDescriptor != null;
            }
        }

        [Category("Data")]
        public Type SourceType
        {
            get
            {
                if (m_propertyDescriptor == null)
                    return this.DataType;
                return m_propertyDescriptor.PropertyType;
            }
        }

        [Editor("Ntreev.Windows.Forms.Grid.Design.TypeSelector, Ntreev.Windows.Forms.Grid.Design, Version=2.0.4510.20986, Culture=neutral, PublicKeyToken=7a9d7c7c4ba5dfca", typeof(System.Drawing.Design.UITypeEditor))]
        [Category("Data")]
        public Type DataType
        {
            get
            {
                if (m_dataType == null)
                {
                    if (m_propertyDescriptor != null)
                        return m_propertyDescriptor.PropertyType;
                    return typeof(string);
                }
                return m_dataType;
            }
            set
            {

                if (m_dataType == value)
                    return;

                if (m_propertyDescriptor != null)
                {
                    if (CanConvertFrom(value, m_propertyDescriptor.PropertyType) == false)
                    {
                        StringBuilder builder = new StringBuilder();

                        builder.AppendLine(string.Format("데이터 타입을 소스 타입으로 변환할 수가 없습니다. TypeConverter를 구현하세요"));
                        builder.AppendLine(string.Format("    DataType : {0}", value));
                        builder.AppendLine(string.Format("    SourceType : {0}", m_propertyDescriptor.PropertyType));


                        throw new System.NotSupportedException(builder.ToString());
                    }
                }

                m_dataType = value;
                m_defaultValue = null;
                m_typeConverter = null;

                SetEditStyleToNative();
            }
        }

        [Browsable(false)]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        public TypeConverter TypeConverter
        {
            get
            {
                if (m_typeConverter == null)
                {
                    if (m_dataType == null)
                    {
                        if (m_propertyDescriptor != null)
                        {
                            return m_propertyDescriptor.Converter;
                        }
                    }
                    return System.ComponentModel.TypeDescriptor.GetConverter(this.DataType);
                }

                return m_typeConverter;
            }
            set
            {
                m_typeConverter = value;
            }
        }

        [DefaultValue(SortType.None)]
        [Category("Behavior")]
        [SettingsBindable(true)]
        public SortType SortType
        {
            get
            {
                return (SortType)m_pColumn.GetSortType();
            }
            set
            {
                m_pColumn.SetSortType((GrSort)value);
            }
        }

        [Browsable(false)]
        [DefaultValue((string)null)]
        public IComparer SortComparer
        {
            get
            {
                return m_sortComparer;
            }
            set
            {
                m_sortComparer = value;
            }
        }

        [Browsable(false)]
        public int VisibleIndex
        {
            get
            {
                return m_pColumn.GetVisibleIndex();
            }
            set
            {
                m_pColumn.SetVisibleIndex(value);

                if (this.GridControl != null)
                {
                    this.GridControl.Invalidate();
                    this.GridCore.Update();
                }
            }
        }

        [Browsable(false)]
        public int DisplayIndex
        {
            get
            {
                return (int)m_pColumn.GetDisplayIndex();
            }
        }

        [Description("컨트롤에 내에 Column 인덱스를 가져옵니다.")]
#if DEBUG
        [Category("Debug")]
#else
        [Browsable(false)]
#endif
        public int Index
        {
            get
            {
                return (int)m_pColumn.GetIndex();
            }
        }


        [DefaultValue((string)null)]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        [Category("Layout")]
        public ColumnPainter ColumnPainter
        {
            get
            {
                return m_columnPainter;
            }

        }

        [Category("Cell")]
        [DefaultValue(StringAlignment.Near)]
        [SettingsBindable(true)]
        public StringAlignment CellAlignment
        {
            get
            {
                GrHorzAlign align = m_pColumn.ItemHorzAlign;
                return (StringAlignment)align;
            }
            set
            {
                m_pColumn.ItemHorzAlign = (GrHorzAlign)value;
            }
        }

        [Category("Cell")]
        [DefaultValue(StringAlignment.Near)]
        [SettingsBindable(true)]
        public StringAlignment CellLineAlignment
        {
            get
            {
                GrVertAlign align = m_pColumn.ItemVertAlign;
                return (StringAlignment)align;
            }
            set
            {
                m_pColumn.ItemVertAlign = (GrVertAlign)value;
            }
        }

        [Category("Cell")]
        [DefaultValue(false)]
        [SettingsBindable(true)]
        public bool CellWordWrap
        {
            get { return m_pColumn.ItemWordWrap; }
            set { m_pColumn.ItemWordWrap = value; }
        }

        [Category("Cell")]
        [DefaultValue(false)]
        [SettingsBindable(true)]
        public bool CellMultiline
        {
            get { return m_pColumn.ItemMultiline; }
            set { m_pColumn.ItemMultiline = value; }
        }

        [Category("Cell")]
        public Color CellForeColor
        {
            get { return m_pColumn.ItemForeColor; }
            set { m_pColumn.ItemForeColor = value; }
        }

        [Category("Cell")]
        public Color CellBackColor
        {
            get { return m_pColumn.ItemBackColor; }
            set { m_pColumn.ItemBackColor = value; }
        }

        [Category("Cell")]
        public Padding CellPadding
        {
            get { return m_pColumn.ItemPadding; }
            set
            {
                m_pColumn.ItemPadding = value;
            }
        }

        [Category("Cell")]
        public Font CellFont
        {
            get
            {
                GrFont pFont = m_pColumn.ItemFont;
                if (pFont == null)
                    return null;

                return pFont.ToManaged();
            }
            set
            {
                if (value == null)
                    m_pColumn.ItemFont = null;
                else
                    m_pColumn.ItemFont = value.FromManaged();
            }
        }

        [DefaultValue(ClickEditType.Default)]
        [Category("Behavior")]
        public ClickEditType ClickEditType
        {
            get
            {
                return (ClickEditType)m_pColumn.ItemClickEditing;
            }
            set
            {
                m_pColumn.ItemClickEditing = (GrClickEditing)value;
            }
        }

#if DEBUG
        [Category("Debug")]
#else
        [Browsable(false)]
#endif
        public virtual ViewType ViewType
        {
            get
            {
                if (m_typeEditor == null)
                    return ViewType.Text;
                return m_typeEditor.ViewType;
            }
        }

        /// <summary>
        /// 열의 <see cref="ISite"/>를 가져오거나 설정합니다.
        /// </summary>
#if DEBUG
        [Category("Debug")]
#else
        [Browsable(false)]
#endif
        public ISite Site
        {
            get;
            set;
        }


        public event EventHandler Disposed;



        protected virtual object EditValue(Design.IEditorService editorService, ICell cell, object value)
        {
            return EditValue(editorService, cell, value);
        }

        protected virtual bool CanEdit(ICell cell, EditingReason reason)
        {
            return TypeEditor.CanEdit(cell, reason);
        }

        protected virtual void OnGridControlAttached() { }

        protected virtual void OnGridControlDetached() { }



        internal object EditValueInternal(Design.IEditorService editorService, ICell cell, object value)
        {
            return EditValue(editorService, cell, value);
        }

        internal bool CanEditInternal(ICell cell, EditingReason reason)
        {
            return CanEdit(cell, reason);
        }

        internal void SyncDisplayText()
        {
            m_pColumn.Text = this.Title;
        }

        internal bool CanConvertFrom(Type dataType, Type sourceType)
        {
            if (dataType == null || dataType == sourceType)
                return true;

            if (dataType.IsEnum == true)
            {
                if (sourceType == typeof(sbyte) ||
                    sourceType == typeof(byte) ||
                    sourceType == typeof(short) ||
                    sourceType == typeof(ushort) ||
                    sourceType == typeof(int) ||
                    sourceType == typeof(uint))
                {
                    return true;
                }
            }

            return this.TypeConverter.CanConvertFrom(sourceType);
        }

        internal bool CanConvertFrom(Type sourceType)
        {
            return CanConvertFrom(m_dataType, sourceType);
        }

        internal object ConvertFromSource(object value)
        {
            if (Utility.IsNullOrDBNull(value) == true)
                return value;

            if (m_propertyDescriptor == null)
                return value;

            if (m_propertyDescriptor.PropertyType == this.DataType)
                return value;

            if (this.DataType.IsEnum == true)
            {
                Type sourceType = this.SourceType;
                if (sourceType == typeof(sbyte) ||
                     sourceType == typeof(byte) ||
                     sourceType == typeof(short) ||
                     sourceType == typeof(ushort) ||
                     sourceType == typeof(int) ||
                     sourceType == typeof(uint))
                {
                    return System.Enum.ToObject(this.DataType, value);
                }
            }

            TypeConverter typeConverter = this.TypeConverter;

            if (m_typeConverter == null)
            {
                typeConverter = System.ComponentModel.TypeDescriptor.GetConverter(this.DataType);
            }
            else
            {
                typeConverter = m_typeConverter;
            }

            return typeConverter.ConvertFrom(value);
        }

        internal object ConvertToSource(object value)
        {
            if (Utility.IsNullOrDBNull(value) == true)
                return value;

            if (m_propertyDescriptor == null)
                return value;

            if (m_propertyDescriptor.PropertyType == this.DataType)
                return value;

            if (this.DataType.IsEnum == true)
            {
                Type sourceType = this.SourceType;
                if (sourceType == typeof(sbyte))
                    return (sbyte)value;
                else if (sourceType == typeof(byte))
                    return (byte)value;
                else if (sourceType == typeof(short))
                    return (short)value;
                else if (sourceType == typeof(ushort))
                    return (ushort)value;
                else if (sourceType == typeof(int))
                    return (int)value;
                else if (sourceType == typeof(uint))
                    return (uint)value;
            }

            TypeConverter typeConverter = null;

            if (m_typeConverter == null)
            {
                typeConverter = System.ComponentModel.TypeDescriptor.GetConverter(this.DataType);
            }
            else
            {
                typeConverter = m_typeConverter;
            }

            return typeConverter.ConvertTo(value, m_propertyDescriptor.PropertyType);
        }

        internal override void OnGridControlAttachedInternal()
        {
            OnGridControlAttached();
        }

        internal override void OnGridControlDetachedInternal()
        {
            OnGridControlDetached();
        }

        internal virtual Design.EditStyle GetEditStyle()
        {
            return this.TypeEditor.GetEditStyle();
        }

        internal virtual void OnCellBoundUpdate(Cell cell, object value) { }



        internal PropertyDescriptor PropertyDescriptor
        {
            get
            {
                return m_propertyDescriptor;
            }
            set
            {
                if (value != null)
                {
                    FrozenAttribute frozenAttr = value.Attributes[typeof(FrozenAttribute)] as FrozenAttribute;
                    if (frozenAttr != null)
                        this.IsFrozen = frozenAttr.IsFrozen;

                    MovableAttribute movableAttr = value.Attributes[typeof(MovableAttribute)] as MovableAttribute;
                    if (movableAttr != null)
                        this.IsMovable = movableAttr.IsMovable;

                    ResizableAttribute resizableAttr = value.Attributes[typeof(ResizableAttribute)] as ResizableAttribute;
                    if (resizableAttr != null)
                        this.IsResizable = resizableAttr.IsResizable;

                    SortableAttribute sortableAttr = value.Attributes[typeof(SortableAttribute)] as SortableAttribute;
                    if (sortableAttr != null)
                        this.IsSortable = sortableAttr.IsSortable;
                }
                else
                {
                    m_typeEditor = null;
                }

                m_propertyDescriptor = value;

                SetEditStyleToNative();

                if (m_propertyDescriptor != null && m_propertyDescriptor.IsReadOnly == true)
                    m_pColumn.IsReadOnly = true;
                if (m_propertyDescriptor != null && m_propertyDescriptor.IsBrowsable == false)
                    m_pColumn.IsVisible = false;

                SyncDisplayText();
            }
        }

        internal new GrColumn NativeRef
        {
            get { return m_pColumn; }
        }

        internal Design.TypeEditor TypeEditor
        {
            get
            {
                if (m_typeEditor == null)
                    return Design.StringTypeEditor.Default;
                return m_typeEditor;
            }
        }

        internal bool HasLifeline { get; set; }



        object IServiceProvider.GetService(Type serviceType)
        {
            if (m_site != null)
                return m_site.GetService(serviceType);
            return null;
        }

        private bool ShouldSerializeCellForeColor()
        {
            return m_pColumn.ItemForeColor != GrColor.Empty;
        }

        private bool ShouldSerializeCellBackColor()
        {
            return m_pColumn.ItemBackColor != GrColor.Empty;
        }

        private bool ShouldSerializeCellPadding()
        {
            return m_pColumn.ItemPadding != GrPadding.Empty;
        }

        private bool ShouldSerializeCellFont()
        {
            return m_pColumn.ItemFont != null;
        }

        private bool ShouldSerializeTitle()
        {
            if (this.Title == string.Empty)
                return true;
            return this.Title != this.ColumnName;
        }

        private bool ShouldSerializeDataType()
        {
            if (m_dataType == null)
                return false;

            if (m_propertyDescriptor == null)
            {
                return m_dataType != typeof(string);
            }
            return m_dataType != m_propertyDescriptor.PropertyType;
        }

        private bool ShouldSerializeTypeConverter()
        {
            if (m_propertyDescriptor == null)
            {
                return System.ComponentModel.TypeDescriptor.GetConverter(this.DataType) != m_typeConverter;
            }
            return m_typeConverter != m_propertyDescriptor.Converter;
        }

        private bool ShouldSerializeDefaultValue()
        {
            if (m_defaultValue == null)
                return false;

            if (this.DataType == typeof(string))
            {
                return string.IsNullOrEmpty(m_defaultValue.ToString()) == false;
            }
            return true;
        }

        private bool ShouldSerializeWidth()
        {
            return m_pColumn.ShouldSerializeWidth();
        }

        private bool ShouldSerializeVisibleIndex()
        {
            return m_pColumn.ShouldSerializeVisibleIndex();
        }
        //bool ShouldSerializePriorityOnFrozen();
        //bool ShouldSerializePriorityOnUnfrozen();

        private void ResetTitle()
        {
            m_title = null;
        }
        private void ResetDataType()
        {
            this.DataType = null;
        }
        private void ResetDefaultValue()
        {
            m_defaultValue = null;
        }
        private void ResetTypeConverter()
        {
            m_typeConverter = null;
        }

        private void propertyDescriptor_ValueChanged(object sender, System.EventArgs e)
        {
            SyncDisplayText();
        }

        private void SetEditStyleToNative()
        {
            if (m_typeEditor != null && this.GridControl != null)
                this.GridControl.Invalidate();
            m_typeEditor = Design.TypeEditor.FromDataType(this.DataType);
            switch (this.ViewType)
            {
                case ViewType.Text:
                    m_pColumn.ItemTextVisible = true;
                    m_pColumn.SetItemIcon(false);
                    break;
                case ViewType.Icon:
                    m_pColumn.ItemTextVisible = true;
                    m_pColumn.SetItemIcon(true);
                    break;
                case ViewType.Custom:
                    m_pColumn.ItemTextVisible = false;
                    m_pColumn.SetItemIcon(false);
                    break;
            }

            switch (GetEditStyle())
            {
                case Design.EditStyle.Control:
                    m_pColumn.ItemType = GrItemType.Control;
                    break;
                case Design.EditStyle.DropDown:
                    m_pColumn.ItemType = GrItemType.DropDown;
                    break;
                case Design.EditStyle.Modal:
                    m_pColumn.ItemType = GrItemType.Modal;
                    break;
            }
        }


        private T GetSourceAttribute<T>(PropertyDescriptor propertyDescriptor) where T : Attribute
        {
            {
                if (propertyDescriptor == null)
                    return default(T);

                return (T)(propertyDescriptor.Attributes[typeof(T)]);
            }
        }

        object IColumn.Tag
        {
            get
            {
                return this.Tag;
            }
        }

        ColumnState IColumn.ColumnState
        {
            get
            {
                if (m_pColumn.GetMousePressed() == true)
                    return ColumnState.Pressed;
                else if (m_pColumn.GetMouseOvered() == true)
                    return ColumnState.Hot;
                return ColumnState.Normal;
            }
        }

        string[] ITextCacheProvider.TextCache
        {
            get
            {
                GrDataRowList pDataRowList = this.GridCore.DataRowList;
                HashSet<string> texts = new HashSet<string>();

                for (int i = 0; i < pDataRowList.GetVisibleDataRowCount(); i++)
                {
                    GrDataRow pDataRow = pDataRowList.GetVisibleDataRow(i);
                    GrItem pItem = pDataRow.GetItem(m_pColumn);

                    texts.Add(pItem.Text);
                }

                return texts.ToArray();

                //string[] textCache = new string[((int)texts.size())];

                //int i = 0;
                //foreach (var value in texts)
                //{
                //    const string t = value;
                //    textCache[i] = new string(t.c_str());
                //    i++;
                //}
                //return textCache;
            }
        }

        static int RowComparerUp(GrGridCore pGridCore, GrRow row1, GrRow row2, object userData)
        {
            GrDataRow pDataRow1 = row1 as GrDataRow;
            GrDataRow pDataRow2 = row2 as GrDataRow;
            GrColumn pColumn = userData as GrColumn;

            Cell cell1 = FromNative.Get(pDataRow1.GetItem(pColumn));
            Cell cell2 = FromNative.Get(pDataRow2.GetItem(pColumn));

            object value1 = cell1.Value;
            object value2 = cell2.Value;

            if (value1 == System.DBNull.Value)
                value1 = null;
            if (value2 == System.DBNull.Value)
                value2 = null;

            if (value1 != null && value2 != null)
            {
                int result;
                System.Collections.IComparer comparer = cell1.Column.SortComparer;
                if (comparer != null)
                {
                    result = comparer.Compare(value1, value2);
                }
                else
                {
                    IComparable comparable = value1 as IComparable;
                    if (comparable != null)
                        result = comparable.CompareTo(value2);
                    else
                        result = value1.ToString().CompareTo(value2.ToString());
                }

                if (result == 0)
                    return pDataRow1.GetDataRowIndex().CompareTo(pDataRow2.GetDataRowIndex());

                return result.CompareTo(0);
            }
            else if (value1 == null)
            {
                if (value2 == null)
                    return pDataRow1.GetDataRowIndex().CompareTo(pDataRow2.GetDataRowIndex());
                return 1;
            }

            return -1;
        }

        static int RowComparerDown(GrGridCore pGridCore, GrRow row1, GrRow row2, object userData)
        {
            return RowComparerUp(pGridCore, row2, row1, userData);
        }

    }
}
