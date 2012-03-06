#pragma once
#include "GridControl.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Native
{
    class WinFormInvalidator : public GrInvalidator
    {
    public:
        enum RectType
        {
            RectType_Empty = 0,
            RectType_Custom,
            RectType_Full,
        };
    public:
        WinFormInvalidator(gcroot<Ntreev::Windows::Forms::Grid::GridControl^> gridControl);

        void Invalidate();
        void Invalidate(int x, int y, int width, int height);
        void Lock();
        void Unlock();
        void Reset();
        bool IsInvalidated() const;

    private:
        gcroot<Ntreev::Windows::Forms::Grid::GridControl^> m_gridControl;
        RectType m_rectType;
        int m_lockRef;
        GrRect m_rect;
    };

    class WinFormScroll : public GrScroll
    {
    public:
        WinFormScroll(gcroot<Ntreev::Windows::Forms::Grid::GridControl^> gridControl, int type);
        virtual ~WinFormScroll();

        int   GetValue() const;
        void  SetValue(int value);
        int   GetSmallChange() const;
        void  SetSmallChange(int value);
        int   GetLargeChange() const;
        void  SetLargeChange(int value);
        int   GetMaximum() const;
        void  SetMaximum(int value);
        int   GetMinimum() const;
        void  SetMinimum(int value);
        bool  GetVisible() const;
        void  SetVisible(bool value);

        void  WndProc(System::IntPtr handle, System::IntPtr wParam);

    private:
        void  SetValue(int value, int scrollEventType);

    private:
        gcroot<Ntreev::Windows::Forms::Grid::GridControl^> m_gridControl;
        gcroot<System::Windows::Forms::ScrollProperties^> m_scroll;
        int   m_type;
    };

    class WinFormTimer : public GrTimer
    {
    public:
        ref class Timer : System::Timers::Timer
        {
        public:
            Timer(WinFormTimer* winformTimer, Ntreev::Windows::Forms::Grid::GridControl^ gridControl);
        private:
            void elapsed(System::Object^ sender, System::Timers::ElapsedEventArgs^ e);
        private:
            WinFormTimer* m_winformTimer;
        };

        WinFormTimer(gcroot<Ntreev::Windows::Forms::Grid::GridControl^> gridControl);

        void  Start();
        void  Stop();
        void  SetInterval(time_t interval);
        void  Invoke(time_t signalTime);

    private:
        gcroot<Ntreev::Windows::Forms::Grid::GridControl^> m_gridControl;
        gcroot<System::Timers::Timer^> m_timer;
    };

    class WinFormWindow : public GrGridWindow
    {
    public:
        WinFormWindow(gcroot<Ntreev::Windows::Forms::Grid::GridControl^> gridControl);

        GrRect    GetSrceenRect() const;
        GrPoint    ClientToScreen(const GrPoint& location) const;
        int     GetMouseWheelScrollLines() const;
        int     GetMouseWheelScrollDelta() const;

        GrSize    GetDragSize() const;
        bool    GetMouseDragEventSupported() const;
        int     GetEscapeKey() const;
        bool    SetCursor(GrCursor cursor);
        GrKeys    GetModifierKeys() const;

        void    OnEditValue(GrEditEventArgs* e);

        GrScroll*   GetHorzScroll() const;
        GrScroll*   GetVertScroll() const;
        GrInvalidator*  GetInvalidator() const;
        GrGridPainter*  GetGridPainter() const;

        GrFont*    GetFont() const;
        GrFont*    GetFont(void* fontData) const;
        GrFont*    GetDefaultFont() const; 

        GrTimer*   CreateTimer();
        void    DestroyTimer(GrTimer* pTimer);

        void    SetFont(System::Drawing::Font^ font);

        //static GrFont* FromManagedFont(System::Drawing::Font^ font);
        //static System::Drawing::Font^ ToManagedFont(GrFont* pFont);

        bool                CanEdit(GrItem* pItem, GrEditingReason reason);

    private:
        gcroot<Ntreev::Windows::Forms::Grid::GridControl^> m_gridControl;
        gcroot<System::Windows::Forms::Cursor^> m_cursorAdd;
        gcroot<System::Windows::Forms::Cursor^> m_cursorRemove;
        gcroot<System::Windows::Forms::Cursor^> m_cursorMove;
        //gcroot<Design::GridValueEditor^> m_valueEditor;

        WinFormScroll  m_horzScroll;
        WinFormScroll  m_vertScroll;
        WinFormInvalidator m_invalidator;
        GrGridPainter*  m_pGridPainter;
        GrFont*    m_pFont;
    };

    ref class WinFormFontManager
    {
    public:
        static GrFont* FromManagedFont(System::Drawing::Font^ font);
        static System::Drawing::Font^ ToManagedFont(GrFont* pFont);

    public:
        static GrFont* m_pDefaultfont;
    };

    class WinFormGridCore : public GrGridCore
    {
    public:
        WinFormGridCore(gcroot<Ntreev::Windows::Forms::Grid::GridControl^> gridControl, GrGridWindow* pGridWindow);

    protected:
        virtual void OnItemMouseEnter(GrItemMouseEventArgs* e);
        virtual void OnItemMouseMove(GrItemMouseEventArgs* e);
        virtual void OnItemMouseLeave(GrItemMouseEventArgs* e);
        virtual void OnItemMouseClick(GrItemMouseEventArgs* e);
        virtual void OnItemMouseDoubleClick(GrItemMouseEventArgs* e);

        virtual void OnRowMouseEnter(GrRowMouseEventArgs* e);
        virtual void OnRowMouseMove(GrRowMouseEventArgs* e);
        virtual void OnRowMouseLeave(GrRowMouseEventArgs* e);

        virtual void PostPaint(GrGridPainter* pPainter, const GrRect& clipRect) const;

    private:
        void    columnList_ColumnMouseDown(GrObject* pSender, GrColumnMouseEventArgs* e);
        void    columnList_ColumnMouseUp(GrObject* pSender, GrColumnMouseEventArgs* e);
        void    columnList_ColumnMouseEnter(GrObject* pSender, GrColumnMouseEventArgs* e);
        void    columnList_ColumnMouseLeave(GrObject* pSender, GrColumnMouseEventArgs* e);
        void    columnList_ColumnMouseMove(GrObject* pSender, GrColumnMouseEventArgs* e);
        void    columnList_ColumnWidthChanged(GrObject* pSender, GrColumnEventArgs* e);
        void    columnList_ColumnFrozenChanged(GrObject* pSender, GrColumnEventArgs* e);

        void    focuser_FocusChanging(GrObject* pSender, GrFocusChangeArgs* e);
        void    focuser_FocusChanged(GrObject* pSender, GrFocusChangeArgs* e);
        void    itemSelector_SelectedRowsChanged(GrObject* pSender, GrEventArgs* e);
        void    itemSelector_SelectedColumnsChanged(GrObject* pSender, GrEventArgs* e);
        void    itemSelector_SelectionChanged(GrObject* pSender, GrEventArgs* e);

    private:
        gcroot<Ntreev::Windows::Forms::Grid::GridControl^> m_gridControl;
    };
} /*namespace Native*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/