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
    class DataTypeEditor : TypeEditor
    {
        private UITypeEditor m_typeEditor;

        public DataTypeEditor(Type dataType)
            : base(dataType)
        {
            m_typeEditor = TypeDescriptor.GetEditor(dataType, typeof(UITypeEditor)) as UITypeEditor;
        }

        public override object EditValue(IEditorService editorService, ICell cell, object value)
        {
            ITypeDescriptorContext context = editorService.GetService(typeof(ITypeDescriptorContext)) as ITypeDescriptorContext;
            editorService.IsDropDownResizable = m_typeEditor.IsDropDownResizable;

            if (value == DBNull.Value)
            {
                object newValue = m_typeEditor.EditValue(context, editorService, null);
                if (newValue == null)
                    return DBNull.Value;
                return newValue;
            }
            return m_typeEditor.EditValue(context, editorService, value);
        }


        public override EditStyle GetEditStyle()
        {
            switch (m_typeEditor.GetEditStyle())
            {
                case UITypeEditorEditStyle.DropDown:
                    return EditStyle.DropDown;
                case UITypeEditorEditStyle.Modal:
                    return EditStyle.Modal;
                default:
                    return EditStyle.Control;
            }
        }

        public override void PaintValue(Graphics graphics, Rectangle paintRect, ICell cell, object value)
        {
            if (value == null || value.ToString() == string.Empty)
                return;

            paintRect.Width = 20;
            m_typeEditor.PaintValue(value, graphics, paintRect);
            paintRect.Width--;
            paintRect.Height--;
            graphics.DrawRectangle(SystemPens.WindowText, paintRect);
        }

        public override ViewType ViewType
        {
            get
            {
                if (m_typeEditor.GetPaintValueSupported() == true)
                    return ViewType.Icon;
                return ViewType.Text;
            }
        }
    }
}
