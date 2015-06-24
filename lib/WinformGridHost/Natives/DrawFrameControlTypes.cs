using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Windows.Forms.Grid.Natives
{
    [Flags()]
    enum DrawFrameControlTypes : uint
    {
        /// <summary>
        /// Standard button
        /// </summary>
        DFC_BUTTON = 4,
        /// <summary>
        /// Title bar
        /// </summary>
        DFC_CAPTION = 1,
        /// <summary>
        /// Menu bar
        /// </summary>
        DFC_MENU = 2,
        /// <summary>
        /// Popup menu item.
        /// </summary>
        DFC_POPUPMENU = 5,
        /// <summary>
        /// Scroll bar
        /// </summary>
        DFC_SCROLL = 3
    }
}
