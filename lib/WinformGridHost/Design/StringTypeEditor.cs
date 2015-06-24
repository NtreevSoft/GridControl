using Ntreev.Windows.Forms.Grid.Design.Controls;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Ntreev.Windows.Forms.Grid.Design
{
    class StringTypeEditor : TypeEditor
    {
        public StringTypeEditor()
            : base(typeof(string))
        {

        }

        public StringTypeEditor(Type dataType)
            : base(dataType)
        {

        }

        public override bool CanEdit(ICell cell, EditingReason reason)
        {
            switch (reason.ReasonType)
            {
                case EditingReasonType.Ime:
                    return true;
                case EditingReasonType.Char:
                    return true;
                default:
                    return base.CanEdit(cell, reason);
            }
        }

        public override Object EditValue(IEditorService editorService, ICell cell, object value)
        {
            InternalTextBox textBox = new InternalTextBox(editorService);

            string oldText = string.Empty;

            switch (editorService.ReasonType)
            {
                case EditingReasonType.Ime:
                case EditingReasonType.Char:
                    break;
                default:
                    {
                        if (Utility.IsNullOrDBNull(value) == true)
                        {
                            textBox.Text = string.Empty;
                        }
                        else
                        {
                            textBox.Text = cell.Column.TypeConverter.ConvertToString(value);
                        }
                    }
                    break;
            }

            oldText = textBox.Text;

            textBox.BackColor = cell.BackColor;
            textBox.ForeColor = cell.ForeColor;
            textBox.Font = cell.Font;
            textBox.Multiline = cell.Multiline;
            textBox.WordWrap = cell.WordWrap;

            ITextCacheProvider textCacheProvider = editorService.GetService(typeof(ITextCacheProvider)) as ITextCacheProvider;
            if (textCacheProvider != null)
            {
                textBox.AutoCompleteCustomSource.AddRange(textCacheProvider.TextCache);
            }

            if (textBox.Multiline == false)
            {
                textBox.Dock = DockStyle.Fill;
            }
            else
            {
                System.Drawing.Point pt = textBox.GetPositionFromCharIndex(0);

                System.Drawing.Rectangle rect = cell.ClientRectangle;
                rect.X = -pt.X;
                rect.Y = 0;
                rect.Width += pt.X;
                textBox.Bounds = rect;
            }

            editorService.ShowControl(textBox);

            if (editorService.Result == DialogResult.Cancel || textBox.Text == oldText)
                return value;

            return cell.Column.TypeConverter.ConvertFromString(textBox.Text);
        }

        public static StringTypeEditor Default = new StringTypeEditor();
    }
}
