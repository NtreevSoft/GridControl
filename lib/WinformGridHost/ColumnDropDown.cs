using Ntreev.Windows.Forms.Grid.Design;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Ntreev.Windows.Forms.Grid
{
    public abstract class ColumnDropDown<TControl> : Column where TControl : Control
    {
        private readonly TControl m_control;
        private bool m_resizable;
        private IEditorService m_editorService;

        public ColumnDropDown()
        {
            m_control = CreateControlInstance(null);
            OnControlCreated();
        }

        public ColumnDropDown(params object[] controlArgs)
        {
            m_control = CreateControlInstance(controlArgs);
            OnControlCreated();
        }

        [DefaultValue(false)]
        public bool Resizable
        {
            get { return m_resizable; }
            set { m_resizable = value; }
        }

        [Browsable(false)]
        public TControl Control
        {
            get { return m_control; }
        }

        internal override sealed EditStyle GetEditStyle()
        {
            return EditStyle.DropDown;
        }

        protected virtual TControl CreateControlInstance(object[] controlArgs)
        {
            if (controlArgs == null)
                return Activator.CreateInstance(typeof(TControl)) as TControl;
            return Activator.CreateInstance(typeof(TControl), controlArgs) as TControl;
        }

        protected virtual void OnControlCreated()
        {

        }

        protected virtual object GetControlValue()
        {
            return null;
        }

        protected virtual void SetControlValue(object value)
        {

        }

        protected virtual void SetControlLayout(ICell cell)
        {
            if (m_control.BackColor != cell.PaintingBackColor)
                m_control.BackColor = cell.PaintingBackColor;
            if (m_control.ForeColor != cell.PaintingForeColor)
                m_control.ForeColor = cell.PaintingForeColor;
            if (m_control.Font != cell.Font)
                m_control.Font = cell.Font;
        }

        protected override sealed object EditValue(IEditorService editorService, ICell cell, object value)
        {
            SetControlLayout(cell);
            editorService.IsDropDownResizable = m_resizable;
            SetControlValue(value);
            m_editorService = editorService;
            editorService.DropDownControl(m_control);
            m_editorService = null;
            return GetControlValue();
        }

        protected void CloseDropDown()
        {
            if (m_editorService == null)
                return;

            m_editorService.Close();
        }
    }
}
