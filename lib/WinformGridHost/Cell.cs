using Ntreev.Library.Grid;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Design;
using System.Linq;
using System.Text;

namespace Ntreev.Windows.Forms.Grid
{
    [TypeConverter(typeof(ExpandableObjectConverter))]
    public class Cell : CellBase, ICell
    {
        object m_oldValue;
        object m_value;
        string m_text;
        object m_displayValue;

        static object NullValue = new object();

        internal Cell(GridControl gridControl, GrItem pItem)
            : base(gridControl, pItem)
        {
            m_oldValue = Cell.NullValue;
            m_displayValue = Cell.NullValue;
        }

        protected Cell(CellBuilder builder)
            : base(builder.GridControl, builder.NativeRef)
        {
            m_oldValue = Cell.NullValue;
            m_displayValue = Cell.NullValue;
        }

        public override string ToString()
        {
            return m_text;
        }


        public bool CancelEdit()
        {
            bool result = this.CancelEditInternal();
            return result;
        }

        public bool EndEdit()
        {
            bool result = this.EndEditInternal();
            return result;
        }

        public void Select(SelectionType selectionType)
        {
            this.Selector.SelectItem(this.NativeRef, (GrSelectionType)selectionType);
        }

        public bool Focus()
        {
            this.Focuser.Set(this.NativeRef);
            return this.IsFocused;
        }

        public void BringIntoView()
        {
            this.GridControl.BringIntoView(this);
        }


        [Category("Data")]
        [Editor("Ntreev.Windows.Forms.Grid.Design.ValueEditor", typeof(UITypeEditor))]
        [TypeConverter(typeof(StringConverter))]
        public object Value
        {
            get;
            set;
        }

        public virtual bool HasSourceValue
        {
            get
            {
                PropertyDescriptor propertyDescriptor = this.Column.PropertyDescriptor;
                object component = this.Row.Component;
                if (propertyDescriptor == null || component == null)
                    return false;
                return true;
            }
        }

        public virtual object SourceValue
        {
            get
            {
                PropertyDescriptor propertyDescriptor = this.Column.PropertyDescriptor;
                object component = this.Row.Component;
                if (propertyDescriptor == null || component == null)
                    throw new ArgumentException();
                return propertyDescriptor.GetValue(component);
            }
        }

#if DEBUG
        [Category("Behavior")]
#else
		[Browsable(false)]
#endif
        public Column Column
        {
            get
            {
                return FromNative.Get(this.NativeRef.GetColumn());
            }
        }

#if DEBUG
        [Category("Behavior")]
#else
		[Browsable(false)]
#endif
        public int ColumnID
        {
            get { return this.Column.ColumnID; }
        }

#if DEBUG
        [Category("Behavior")]
#else
		[Browsable(false)]
#endif
        public Row Row
        {
            get { return FromNative.Get(this.NativeRef.GetDataRow());}
        }

        /// <summary>
        /// 이 셀을 소유한 행의 고유 아이디를 가져옵니다.
        /// </summary>
        /// <returns>
        /// 고유 아이디를 나타내는 <see cref="System.UInt32"/>형태의 정수값입니다.
        /// </returns>
#if DEBUG
        [Category("Behavior")]
#else
		[Browsable(false)]
#endif
        public int RowID
        {
            get { return this.Row.RowID; }
        }

        [Category("Behavior")]
        [DefaultValue(false)]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        public bool IsSelected
        {
            get
            {
                return this.NativeRef.GetSelected();
            }
            set
            {
                if (this.Row.IsVisible == false)
                    throw new System.InvalidOperationException();
                this.NativeRef.SetSelected(value);
            }
        }

#if DEBUG
        [Category("Debug")]
#else
		[Browsable(false)]
#endif
        public bool IsFocused
        {
            get
            {
                return this.NativeRef.GetFocused();
            }
        }

#if DEBUG
        [Category("Debug")]
#else
		[Browsable(false)]
#endif
        public bool IsMouseOvered
        {
            get
            {
                return this.NativeRef.GetMouseOvered();
            }
        }

        [Category("Behavior")]
        [DefaultValue(false)]
        public bool IsReadOnly
        {
            get
            {
                return this.NativeRef.GetReadOnly();
            }
            set
            {
                this.NativeRef.SetReadOnly(value);
            }
        }

#if DEBUG
        [Category("Debug")]
#else
		[Browsable(false)]
#endif
        public bool IsBeingEdited
        {
            get
            {
                return this.GridControl.EditingCell == this;
            }
        }

#if DEBUG
        [Category("Debug")]
#else
		[Browsable(false)]
#endif
        public bool IsEdited
        {
            get
            {
                return m_oldValue != Cell.NullValue ? true : false;
            }
        }

        [Category("Behavior")]
        [DefaultValue("")]
        public string Error
        {
            get
            {
                return this.Row.GetError(this);
            }
            set
            {
                this.Row.SetError(this, value);
            }
        }

#if DEBUG
        [Category("Debug")]
#else
		[Browsable(false)]
#endif
        public Rectangle TextBound
        {
            get
            {
                GrSize size = this.NativeRef.GetTextBounds();
                return new Rectangle(0, 0, size.Width, size.Height);
            }
        }

        public object DisplayValue
        {
            get
            {
                if (m_displayValue == Cell.NullValue)
                    return this.ValueCore;
                return m_displayValue;
            }
            set
            {

                if (value == null || value == DBNull.Value)
                {
                    m_displayValue = Cell.NullValue;
                    this.UpdateNativeText();
                }
                else
                {
                    if (value.GetType() != this.Column.DataType)
                        throw new System.ArgumentException("설정할 수 없는 타입입니다.");

                    m_displayValue = value;
                    this.UpdateNativeText(value.ToString());
                    this.Invalidate();
                }
            }
        }





        internal void UpdateNativeText()
        {
            object value = null;

            try
            {
                value = this.ValueCore;
            }
            catch (Exception e)
            {
                this.Row.SetInvalidValue(this, e.Message);
            }

            try
            {
                string text = string.Empty;

                if (this.Row.HasInvalidValue(this) == true)
                {
                    object sourceValue = this.SourceValue;
                    if (sourceValue != null)
                        text = sourceValue.ToString();
                }

                else if (value != null && value != DBNull.Value)
                {
                    text = value.ToString();

                    if (this.Column.CellMultiline == true)
                        text = text.Replace("\r\n", "\n");
                }
                this.UpdateNativeText(text);
                if (this.NativeRef.IsGridCoreAttached() == true)
                    this.Column.OnCellBoundUpdate(this, value);
            }
            catch (Exception)
            {
                this.UpdateNativeText(string.Empty);
            }
        }

        internal void UpdateNativeText(string text)
        {
            if (m_text != null)
                this.Row.m_textCapacity -= m_text.Length;

            m_text = text;

            if (m_text != null)
            {
                this.Row.m_textCapacity += m_text.Length;
                this.NativeRef.Text = m_text;
            }
            else
            {
                this.NativeRef.Text = string.Empty;
            }
        }

        internal object ValidateValue(object value)
        {
            if (Utility.IsNullOrDBNull(value) == true)
                return value;

            Type dataType = this.Column.DataType;
            Type valueType = value.GetType();

            if (dataType == valueType)
                return value;

            if (dataType == typeof(string))
                return value.ToString();

            TypeConverter typeConverter = this.Column.TypeConverter;
            TypeDescriptorContextCore typeDescriptorContext = new TypeDescriptorContextCore(this);
            if (typeConverter.CanConvertFrom(typeDescriptorContext, valueType) == false)
            {
                string reason = string.Format("{0}형식을 {1}으로 변환할 수 없습니다.", valueType, dataType);
                throw new System.ArgumentException(reason);
            }

            return typeConverter.ConvertFrom(typeDescriptorContext, System.Windows.Forms.Application.CurrentCulture, value);
        }

        internal bool CancelEditInternal()
        {
            if (m_oldValue == Cell.NullValue)
                return false;

            this.Row.RemoveEditedCell();
            if (this.Row.HasInvalidValue(this) == false)
            {
                if (this.GridControl.InvokeValueChanging(this, m_oldValue, this.ValueCore) == true)
                {
                    this.ValueCore = m_oldValue;
                    this.GridControl.InvokeValueChanged(this);
                }
            }
            this.UpdateNativeText();

            m_oldValue = Cell.NullValue;
            return true;
        }

        internal bool EndEditInternal()
        {
            if (m_oldValue == Cell.NullValue)
                return false;

            if (m_value != null)
            {
                object value = m_value;
                m_value = null;
                this.ValueCore = value;
            }

            this.Row.RemoveEditedCell();
            m_oldValue = Cell.NullValue;
            return true;
        }

        internal void SourceValueToLocal(object component)
        {
            PropertyDescriptor propertyDescriptor = this.Column.PropertyDescriptor;
            if (propertyDescriptor == null)
                return;

            if (component == null)
            {
                m_value = null;
            }
            else
            {
                object value = propertyDescriptor.GetValue(component);
                m_value = this.Column.ConvertFromSource(value);
            }
        }

        internal void LocalValueToSource(object component)
        {
            PropertyDescriptor propertyDescriptor = this.Column.PropertyDescriptor;
            if (propertyDescriptor == null)
                return;

            object value = propertyDescriptor.GetValue(component);

            if (Utility.IsNullOrDBNull(m_value) == false)
            {
                value = this.Column.ConvertToSource(m_value);
                propertyDescriptor.SetValue(component, value);
            }

            m_value = null;
        }

        internal object GetValueFromSource()
        {
            System.ComponentModel.PropertyDescriptor propertyDescriptor = this.Column.PropertyDescriptor;
            object component = this.Row.Component;
            if (propertyDescriptor == null || component == null)
                throw new System.ArgumentException();
            object value = propertyDescriptor.GetValue(component);
            return this.Column.ConvertFromSource(value);
        }

        internal void SetValueToSource(object value)
        {
            System.ComponentModel.PropertyDescriptor propertyDescriptor = this.Column.PropertyDescriptor;
            object component = this.Row.Component;
            if (propertyDescriptor.IsReadOnly == false)
            {
                value = this.Column.ConvertToSource(value);
                try
                {
                    //if(propertyDescriptor.ShouldSerializeValue(component) == false)
                    propertyDescriptor.SetValue(component, value);
                    if (this.Row.HasInvalidValue(this) == true)
                        this.Row.SetInvalidValue(this, null);
                }
                catch (Exception e)
                {
                    if (value == null)
                        propertyDescriptor.SetValue(component, DBNull.Value);
                    else
                        throw e;
                }
            }
        }

        internal void ClearLocalValue()
        {
            m_value = null;
            m_oldValue = Cell.NullValue;
            m_displayValue = Cell.NullValue;
        }

        internal new  GrItem NativeRef
        {
            get { return base.NativeRef as GrItem; }
        }

        internal bool IsSelecting
        {
            get
            {
                return this.NativeRef.IsItemSelecting();
            }
        }

        internal object ValueCore
        {
            get
            {

                if (this.Row.HasInvalidValue(this) == true)
                    return null;

                if (this.HasSourceValue == false)
                {
                    return m_value;
                }
                return this.GetValueFromSource();
            }
            set
            {

                if (this.HasSourceValue == false)
                    m_value = value;
                else
                    this.SetValueToSource(value);
            }
        }

        internal string SourceError
        {
            get
            {
                return this.Row.GetSourceError(this);
            }
        }

        internal string InvalidValue
        {
            get
            {
                return this.Row.GetInvalidValue(this);
            }
        }



        private bool ShouldSerializeValue()
        {
            PropertyDescriptor propertyDescriptor = this.Column.PropertyDescriptor;
            object component = this.Row.Component;
            if (propertyDescriptor == null || component == null)
            {
                return m_value != null;
            }
            return propertyDescriptor.ShouldSerializeValue(this.Row.Component);
        }

        #region ICell

        object ICell.Value
        {
            get { return this.Value; }
        }

        object ICell.Tag
        {
            get { return this.Tag; }
        }

        IColumn ICell.Column
        {
            get { return this.Column; }
        }

        #endregion

        //GrItem* m_pItem;


        //static initonly object NullValue = new System.Object();
    };
}
