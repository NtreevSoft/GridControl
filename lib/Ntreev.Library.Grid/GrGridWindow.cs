using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public abstract class GrGridWindow : GrObject
    {
        public abstract GrRect GetSrceenRect();
        public abstract GrPoint ClientToScreen(GrPoint location);
        public abstract int GetMouseWheelScrollLines();
        public abstract int GetMouseWheelScrollDelta();
        public abstract GrSize GetDragSize();
        public abstract GrKeys GetModifierKeys();
        public abstract bool SetCursor(int cursor);

        public abstract GrScroll GetHorzScroll();
        public abstract GrScroll GetVertScroll();
        public abstract GrInvalidator GetInvalidator();
        public abstract GrGridPainter GetGridPainter();

        public abstract ITimer CreateTimer();
        public abstract void DestroyTimer(ITimer pTimer);

        public abstract void OnEditValue(GrEditEventArgs e);

        public virtual GrSelectionType GetSelectionType()
        {
            if ((GetModifierKeys() & GrKeys.Control) == GrKeys.Control)
                return GrSelectionType.Add;
            return GrSelectionType.Normal;
        }

        public virtual GrSelectionRange GetSelectionRange()
        {
            if ((GetModifierKeys() & GrKeys.Shift) == GrKeys.Shift)
                return GrSelectionRange.Multi;
            return GrSelectionRange.One;
        }

        public virtual void OnMouseDown(GrPoint location)
        {
            this.GridCore.GetStateManager().OnMouseDown(location, GetModifierKeys());
        }

        public virtual void OnMouseMove(GrPoint location, bool pressed)
        {
            this.GridCore.GetStateManager().OnMouseMove(location, GetModifierKeys(), pressed);
        }

        public virtual void OnMouseUp(GrPoint location)
        {
            this.GridCore.GetStateManager().OnMouseUp(location, GetModifierKeys());
        }

        public virtual void OnMouseClick(GrPoint location)
        {
            this.GridCore.GetStateManager().OnMouseClick(location, GetModifierKeys());
        }

        public virtual void OnMouseDoubleClick(GrPoint location)
        {
            this.GridCore.GetStateManager().OnMouseDoubleClick(location, GetModifierKeys());
        }

        public virtual void OnMouseWheel(GrPoint location, int delta)
        {
            this.GridCore.GetStateManager().OnMouseWheel(location, GetModifierKeys(), delta);
        }

        public virtual void OnMouseLeave()
        {
            this.GridCore.SetMouseOver(null, GrPoint.Empty);
        }

        public virtual void OnMouseEnter()
        {


        }
        public virtual bool OnKeyDown(GrKeys key)
        {
            return this.GridCore.GetStateManager().OnKeyDown(key, GetModifierKeys());
        }

        public virtual void OnKeyUp(GrKeys key)
        {
            this.GridCore.GetStateManager().OnKeyUp(key, GetModifierKeys());
        }

        public virtual void OnGotFocus()
        {

        }

        public virtual void OnLostFocus()
        {

        }

        public virtual void OnSizeChanged(GrRect rect)
        {
            this.GridCore.DisplayRectangle = rect;
        }

        public virtual void OnUpdate()
        {
            this.GridCore.Update();
        }

        public virtual void OnPaint(object painterDevice, GrRect clipping)
        {
            this.GridCore.Update();
            GrGridPainter pPainter = GetGridPainter();
            pPainter.BeginPaint(painterDevice);
            this.GridCore.BeginPaint();
            this.GridCore.InvokePrePaint(pPainter, clipping);
            this.GridCore.InvokePaint(pPainter, clipping);
            this.GridCore.InvokePostPaint(pPainter, clipping);
            this.GridCore.EndPaint();
            pPainter.EndPaint();
        }


        public virtual bool CanEdit(GrItem pItem, GrEditingReason reason)
        {
            if (pItem.GetReadOnly() == true)
                return false;

            switch (reason.editingType)
            {
                case GrEditingType.Mouse:
                    return pItem.HitMouseOverTest(reason.location) != 0;
                case GrEditingType.Key:
                    {
                        if (reason.key == GrKeys.F2)
                            return true;
                        else if (reason.key == GrKeys.Enter)
                            return true;
                    }
                    break;
                default:
                    return false;
            }
            return false;
        }


        protected override void OnGridCoreAttached()
        {
            base.OnGridCoreAttached();

            GrScroll pHorzScroll = GetHorzScroll();
            GrScroll pVertScroll = GetVertScroll();

            this.GridCore.AttachObject(pHorzScroll);
            this.GridCore.AttachObject(pVertScroll);

            pHorzScroll.IsVisible = false;
            pVertScroll.IsVisible = false;
        }


        public GrRect ClientToScreen(GrRect rect)
        {
            GrPoint clientLocation = rect.Location;
            GrPoint screenLocation = ClientToScreen(clientLocation);
            GrRect value = rect;
            value.Offset(screenLocation.X - clientLocation.X, screenLocation.Y - clientLocation.Y);
            return value;

        }
    }
}
