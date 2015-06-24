using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Windows.Forms.Grid.Natives
{
    [Flags]
    enum DrawFrameControlStates : uint
    {
        // =====================================================================================
        // If uType is DFC_BUTTON, uState can be one of the following values.
        // =====================================================================================
        /// <summary>
        /// Check box
        /// </summary>
        DFCS_BUTTONCHECK = 0,
        /// <summary>
        /// Image for radio button (nonsquare needs image)
        /// </summary>
        DFCS_BUTTONRADIOIMAGE = 1,
        /// <summary>
        /// Mask for radio button (nonsquare needs mask)
        /// </summary>
        DFCS_BUTTONRADIOMASK = 2,
        /// <summary>
        /// Radio button
        /// </summary>
        DFCS_BUTTONRADIO = 4,
        /// <summary>
        /// Three-state button
        /// </summary>
        DFCS_BUTTON3STATE = 8,
        /// <summary>
        /// Push button
        /// </summary>
        DFCS_BUTTONPUSH = 0x10,
        // =====================================================================================
        // If uType is DFC_CAPTION, uState can be one of the following values.
        // =====================================================================================
        /// <summary>
        /// Close button
        /// </summary>
        DFCS_CAPTIONCLOSE = 0,
        /// <summary>
        /// Minimize button
        /// </summary>
        DFCS_CAPTIONMIN = 1,
        /// <summary>
        /// Maximize button
        /// </summary>
        DFCS_CAPTIONMAX = 2,
        /// <summary>
        /// Restore button
        /// </summary>
        DFCS_CAPTIONRESTORE = 3,
        /// <summary>
        /// Help button
        /// </summary>
        DFCS_CAPTIONHELP = 4,
        // =====================================================================================
        // If uType is DFC_MENU, uState can be one of the following values.
        // =====================================================================================
        /// <summary>
        /// Submenu arrow
        /// </summary>
        DFCS_MENUARROW = 0,
        /// <summary>
        /// Check mark
        /// </summary>
        DFCS_MENUCHECK = 1,
        /// <summary>
        /// Bullet
        /// </summary>
        DFCS_MENUBULLET = 2,
        /// <summary>
        /// Submenu arrow pointing left. This is used for the right-to-left cascading menus used with right-to-left languages such as Arabic or Hebrew.
        /// </summary>
        DFCS_MENUARROWRIGHT = 4,
        // =====================================================================================
        // If uType is DFC_SCROLL, uState can be one of the following values.
        // =====================================================================================
        /// <summary>
        /// Up arrow of scroll bar
        /// </summary>
        DFCS_SCROLLUP = 0,
        /// <summary>
        /// Down arrow of scroll bar
        /// </summary>
        DFCS_SCROLLDOWN = 1,
        /// <summary>
        /// Left arrow of scroll bar
        /// </summary>
        DFCS_SCROLLLEFT = 2,
        /// <summary>
        /// Right arrow of scroll bar
        /// </summary>
        DFCS_SCROLLRIGHT = 3,
        /// <summary>
        /// Combo box scroll bar
        /// </summary>
        DFCS_SCROLLCOMBOBOX = 5,
        /// <summary>
        /// Size grip in lower-right corner of window
        /// </summary>
        DFCS_SCROLLSIZEGRIP = 8,
        /// <summary>
        /// Size grip in lower-left corner of window. This is used with right-to-left languages such as Arabic or Hebrew.
        /// </summary>
        DFCS_SCROLLSIZEGRIPRIGHT = 0x10,
        // =====================================================================================
        // The following style can be used to adjust the bounding rectangle of the push button.
        // =====================================================================================
        /// <summary>
        /// Bounding rectangle is adjusted to exclude the surrounding edge of the push button.
        /// </summary>
        DFCS_ADJUSTRECT = 0x2000,
        // =====================================================================================
        // One or more of the following values can be used to set the state of the control to be drawn.
        // =====================================================================================
        /// <summary>
        /// Button is inactive (grayed).
        /// </summary>
        DFCS_INACTIVE = 0x100,
        /// <summary>
        /// Button is pushed.
        /// </summary>
        DFCS_PUSHED = 0x200,
        /// <summary>
        /// Button is checked.
        /// </summary>
        DFCS_CHECKED = 0x400,
        /// <summary>
        /// The background remains untouched. This flag can only be combined with DFCS_MENUARROWUP or DFCS_MENUARROWDOWN.
        /// </summary>
        DFCS_TRANSPARENT = 0x800,
        /// <summary>
        /// Button is hot-tracked.
        /// </summary>
        DFCS_HOT = 0x1000,
        /// <summary>
        /// Button has a flat border.
        /// </summary>
        DFCS_FLAT = 0x4000,
        /// <summary>
        /// Button has a monochrome border.
        /// </summary>
        DFCS_MONO = 0x8000
    }

}
