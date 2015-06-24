using Ntreev.Library.Grid;
using Ntreev.Windows.Forms.Grid.Design;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Ntreev.Windows.Forms.Grid
{
    class WinFormWindow : GrGridWindow
    {
        GridControl m_gridControl;
        Cursor m_cursorAdd;
        Cursor m_cursorRemove;
        Cursor m_cursorMove;
        //gcroot<Design.GridValueEditor> m_valueEditor;

        WinFormScroll m_horzScroll;
        WinFormScroll m_vertScroll;
        WinFormInvalidator m_invalidator;
        GrGridPainter m_pGridPainter;
        GrFont m_pFont;
        GrColor m_foreColor;
        GrColor m_backColor;

        public WinFormWindow(GridControl gridControl)
        {
            m_gridControl = gridControl;
            m_horzScroll = new WinFormScroll(gridControl, Orientation.Horizontal);
            m_vertScroll = new WinFormScroll(gridControl, Orientation.Vertical);
            m_invalidator = new WinFormInvalidator(gridControl);

            m_pGridPainter = new GrGridPainterDC(m_gridControl.Handle);
            using (Stream stream = new MemoryStream(Properties.Resources.Ntreev_Windows_Forms_Grid_Cursors_Movable))
            {
                
                m_cursorMove = new Cursor(stream);
            }

            using (Stream stream = new MemoryStream(Properties.Resources.Ntreev_Windows_Forms_Grid_Cursors_Add))
            {
                m_cursorAdd = new Cursor(stream);
            }

            using (Stream stream = new MemoryStream(Properties.Resources.Ntreev_Windows_Forms_Grid_Cursors_Remove))
            {
                m_cursorRemove = new Cursor(stream);
            }
        }

        public override GrRect GetSrceenRect()
        {
            return Screen.PrimaryScreen.Bounds;
        }

        public override GrPoint ClientToScreen(GrPoint location)
        {
            return m_gridControl.PointToScreen(location);
        }

        public override int GetMouseWheelScrollLines()
        {
            return SystemInformation.MouseWheelScrollLines;
        }

        public override int GetMouseWheelScrollDelta()
        {
            return SystemInformation.MouseWheelScrollDelta;
        }

        public override GrSize GetDragSize()
        {
            return SystemInformation.DragSize;

        }

        public bool GetMouseDragEventSupported()
        {
            return false;
        }

        //public int GetEscapeKey();
        public override bool SetCursor(int cursor)
        {
            Cursor temp;
            switch ((GrCursor)cursor)
            {
                case GrCursor.No:
                    temp = Cursors.No;
                    break;
                case GrCursor.Wait:
                    temp = Cursors.WaitCursor;
                    break;
                case GrCursor.Add:
                    temp = m_cursorAdd;
                    break;
                case GrCursor.Remove:
                    temp = m_cursorRemove;
                    break;
                case GrCursor.Move:
                    temp = m_cursorMove;
                    break;
                case GrCursor.HSplit:
                    temp = Cursors.HSplit;
                    break;
                case GrCursor.VSplit:
                    temp = Cursors.VSplit;
                    break;
                case GrCursor.SizeWE:
                    temp = Cursors.SizeWE;
                    break;
                case GrCursor.SizeNS:
                    temp = Cursors.SizeNS;
                    break;
                default:
                    temp = Cursors.Default;
                    break;
            }

            //if(m_gridControl.Site != null)
            //{
            // m_gridControl.Cursor = temp;
            // //m_gridControl.CaptionRow.Text = temp.ToString();
            // //return Cursor.Current != Cursors.Default;
            // return true;
            //}

            if (m_gridControl.Cursor != temp)
            {
                m_gridControl.Cursor = temp;
#if DEBUG
                //System.Diagnostics.Trace.WriteLine(temp);
#endif
                if (m_gridControl.Site != null)
                {
                    Cursor.Current = temp;
                }

                return true;
            }

            return false;
        }

        public override GrKeys GetModifierKeys()
        {
            GrKeys modifierKeys = 0;
            if ((Control.ModifierKeys & Keys.Control) == Keys.Control)
                modifierKeys |= GrKeys.Control;
            if ((Control.ModifierKeys & Keys.Shift) == Keys.Shift)
                modifierKeys |= GrKeys.Shift;
            if ((Control.ModifierKeys & Keys.Alt) == Keys.Alt)
                modifierKeys |= GrKeys.Alt;
            return modifierKeys;
        }


        public override void OnEditValue(GrEditEventArgs e)
        {

            GrEditingReason reason = e.GetReason();
            Cell cell = FromNative.Get(e.GetItem());
            Row row = cell.Row;

            if (m_gridControl.InvokeEditBegun(cell) == false)
                return;

            //m_gridControl.Refresh();
            TypeEditorForm form = new TypeEditorForm(m_gridControl, cell, new EditingReason(reason));
            m_gridControl.Update();

            try
            {
                object newValue = cell.Column.EditValueInternal(form, cell, cell.Value);

                if (form.DialogResult != DialogResult.Cancel && newValue != null)
                {
                    if (row != m_gridControl.InsertionRow && cell.Row.Index < 0)
                        throw new System.Exception("행이 삭제 되었을수도 있습니다.");

                    if (System.Object.Equals(newValue, cell.Value) == false)
                    {
                        cell.Value = newValue;
                        e.SetHandled(true);
                    }
                    //m_gridControl.Update();
                }
            }
            catch (Exception e1)
            {
                m_gridControl.ShowMessage(e1.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                //cell.CancelEdit();
            }
            finally
            {
                form.Dispose();
            }

            CellEventArgs ee = new CellEventArgs(cell);
            m_gridControl.InvokeEditEnded(ee);

        }

        public override GrScroll GetHorzScroll()
        {
            return m_horzScroll;
        }

        public override GrScroll GetVertScroll()
        {
            return m_vertScroll;
        }

        public override GrInvalidator GetInvalidator()
        {
            return m_invalidator;

        }
        public override GrGridPainter GetGridPainter()
        {
            return m_pGridPainter;
        }


        //GrColor GetForeColor() ;
        //GrColor GetBackColor() ;
        //      GrFont GetFont() ;
        //GrFont GetFont(void fontData) ;
        //GrFont GetDefaultFont() ; 

        public override ITimer CreateTimer()
        {
            return new WinFormTimer(m_gridControl);
        }

        public override void DestroyTimer(ITimer pTimer)
        {
            pTimer.Dispose();
        }


        //      void SetFont(System.Drawing.Font font);
        //void SetForeColor(System.Drawing.Color foreColor);
        //void SetBackColor(System.Drawing.Color backColor);

        //static GrFont FromManagedFont(System.Drawing.Font font);
        //static System.Drawing.Font ToManagedFont(GrFont pFont);

        public override bool CanEdit(GrItem pItem, GrEditingReason reason)
        {
            Cell cell =
                FromNative.Get(pItem);

            //if(GrGridWindow.CanEdit(pItem, reason) == false)
            return cell.Column.CanEditInternal(cell, new EditingReason(reason));
        }

    }
}
