using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public interface GrGridWindow
    {
        GrRect GetSrceenRect();
        GrPoint ClientToScreen(GrPoint location);
        int GetMouseWheelScrollLines();
        int GetMouseWheelScrollDelta();
        GrSize GetDragSize();
        GrKeys GetModifierKeys();
        bool SetCursor(int cursor);

        GrScroll GetHorzScroll();
        GrScroll GetVertScroll();
        GrInvalidator GetInvalidator();
        GrGridPainter GetGridPainter();

        GrTimer CreateTimer();
        void DestroyTimer(GrTimer pTimer);

        void OnEditValue(GrEditEventArgs e);

        GrSelectionType GetSelectionType();
        GrSelectionRange GetSelectionRange();

        void OnMouseDown(GrPoint location);
        void OnMouseMove(GrPoint location, bool pressed);
        void OnMouseUp(GrPoint location);
        void OnMouseClick(GrPoint location);
        void OnMouseDoubleClick(GrPoint location);
        void OnMouseWheel(GrPoint location, int delta);
        void OnMouseLeave();
        void OnMouseEnter();
        bool OnKeyDown(GrKeys key);
        void OnKeyUp(GrKeys key);
        void OnGotFocus();
        void OnLostFocus();
        void OnSizeChanged(GrRect rect);
        void OnUpdate();
        void OnPaint(object painterDevice, GrRect clipping);

        bool CanEdit(GrItem pItem, GrEditingReason reason);

        //protected:
        //    virtual void OnGridCoreAttached();

        //public:
        //    GrRect ClientToScreen( GrRect rect) ;
    }
}
