using Ntreev.Windows.Forms.Grid;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Design;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Windows.Forms.Grid.Design
{
    public abstract class TypeEditor
    {
        private readonly Type m_dataType;

        protected TypeEditor(Type dataType)
        {
            m_dataType = dataType;
        }

        public virtual bool CanEdit(ICell cell, EditingReason reason)
        {
            if (reason.ReasonType == EditingReasonType.Mouse)
                return true;
            return false;
        }

        public virtual object EditValue(IEditorService editorService, ICell cell, object value)
        {
            return value;
        }

        public virtual void PaintValue(Graphics graphics, Rectangle paintRect, ICell cell, object value)
        {

        }

        public virtual EditStyle GetEditStyle()
        {
            return EditStyle.Control;
        }

        public static TypeEditor FromDataType(Type dataType)
        {
            if (typeof(IBindingList).IsAssignableFrom(dataType) == true)
            {
                return new BindingListTypeEditor(dataType);
            }
            else
            {
                UITypeEditor typeEditor = TypeDescriptor.GetEditor(dataType, typeof(UITypeEditor)) as UITypeEditor;

                if (typeEditor != null)
                    return new DataTypeEditor(dataType);

                if (dataType == typeof(bool))
                {
                    return new BooleanTypeEditor(dataType);
                }
                else if (dataType.IsEnum == true)
                {
                    if (dataType.GetCustomAttributes(typeof(FlagsAttribute), true).Length != 0)
                    {
                        return new FlagTypeEditor(dataType);
                    }
                    return new EnumTypeEditor(dataType);
                }
                else if (dataType.IsArray == true)
                {
                    return new ListTypeEditor(dataType);
                }
            }
            return new StringTypeEditor(dataType);
        }

        public virtual ViewType ViewType
        {
            get { return ViewType.Text; }
        }

        protected Type DataType
        {
            get { return m_dataType; }
        }
    }
}
