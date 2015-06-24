using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Ntreev.Windows.Forms.Grid.Design
{
    public interface IEditorService : IServiceProvider
    {
        void ShowControl(Control control);

        void DropDownControl(Control control);

        void ShowDialog(Control control);

        void Close();

        bool IsDropDownResizable
        {
            set;
        }

        EditingReasonType ReasonType
        {
            get;
        }

        DialogResult Result
        {
            get;
        }
    }
}
