//=====================================================================================================================
// Ntreev Grid for .Net 2.0.4464.32161
// https://github.com/NtreevSoft/GridControl
// 
// Released under the MIT License.
// 
// Copyright (c) 2010 Ntreev Soft co., Ltd.
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
// documentation files (the "Software"), to deal in the Software without restriction, including without limitation the 
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit 
// persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the 
// Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
// WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//=====================================================================================================================


#pragma once
#include "GrGridBase.h"
#include "GrGridEvent.h"
#include "GrGridPainter.h"

class GrItem;

enum GrEditingType
{
    GrEditingType_None,

    GrEditingType_Mouse,
    GrEditingType_Key,
    GrEditingType_Char,
    GrEditingType_Ime,
};

struct GrEditingReason
{
    GrEditingReason();
    GrEditingReason(wchar_t character, bool ime);
    GrEditingReason(GrKeys key);
    GrEditingReason(GrPoint location);

    GrEditingType editingType;
    wchar_t character;
    GrKeys key;
    GrPoint location;
    int keyValue;
};

class GrEditEventArgs : public GrEventArgs
{
public:
    GrEditEventArgs(GrItem* pItem, GrEditingReason reason);

    GrItem* GetItem() const;
    GrEditingReason GetReason() const;
    bool GetHandled() const;
    void SetHandled(bool b);

private:
    GrItem* m_pItem;
    GrEditingReason m_reason;
    bool m_handled;
};

class GrElapsedEventArgs : public GrEventArgs
{
public:
    GrElapsedEventArgs(time_t signalTime);

    time_t GetSignalTime() const;

private:
    time_t m_signalTime;
};

class GrInvalidator
{
public:
    virtual void Invalidate() = 0;
    virtual void Invalidate(int x, int y, int width, int height) = 0;

    virtual void Lock() = 0;
    virtual void Unlock() = 0;
    virtual void Reset() = 0;

    virtual bool IsInvalidated() const = 0;

    void Invalidate(const GrRect& rect);
};

class GrScroll : public GrObject
{
public:
    int ValidateValue(int value) const;
    bool DoScroll(int value);

    virtual int GetValue() const = 0;
    virtual void SetValue(int value) = 0;
    virtual int GetSmallChange() const = 0;
    virtual void SetSmallChange(int value) = 0;
    virtual int GetLargeChange() const = 0;
    virtual void SetLargeChange(int value) = 0;
    virtual int GetMaximum() const = 0;
    virtual void SetMaximum(int value) = 0;
    virtual int GetMinimum() const = 0;
    virtual void SetMinimum(int value) = 0;
    virtual bool GetVisible() const = 0;
    virtual void SetVisible(bool value) = 0;
};

class GrTimer : public GrObject
{
    typedef GrEvent<GrElapsedEventArgs, GrTimer> _GrElpasedEvent;
public:
    virtual void Start() = 0;
    virtual void Stop() = 0;
    virtual void SetInterval(time_t interval) = 0;

public:
    _GrElpasedEvent Elapsed;

protected:
    void InvokeElapsed(time_t signalTime);
};

enum GrCursor
{
    GrCursor_Default,
    GrCursor_No,
    GrCursor_Wait,
    GrCursor_Add,
    GrCursor_Remove,
    GrCursor_Move,
    GrCursor_HSplit,
    GrCursor_VSplit,
    GrCursor_SizeWE,
    GrCursor_SizeNS,

    GrCursor_Unknown,
};

class GrGridWindow : public GrObject
{
public:
    virtual GrRect GetSrceenRect() const = 0;
    virtual GrPoint ClientToScreen(const GrPoint& location) const = 0;
    virtual int GetMouseWheelScrollLines() const = 0;
    virtual int GetMouseWheelScrollDelta() const = 0;
    virtual GrSize GetDragSize() const = 0;
    virtual GrKeys GetModifierKeys() const = 0;
    virtual bool SetCursor(int cursor) = 0;

    virtual GrScroll* GetHorzScroll() const = 0;
    virtual GrScroll* GetVertScroll() const = 0;
    virtual GrInvalidator* GetInvalidator() const = 0;
    virtual GrGridPainter* GetGridPainter() const = 0;

    virtual GrFont* GetFont() const = 0;
    virtual GrFont* GetFont(void* fontData) const = 0;
    virtual GrFont* GetDefaultFont() const = 0;

    virtual GrTimer* CreateTimer() = 0;
    virtual void DestroyTimer(GrTimer* pTimer) = 0;

    virtual void OnEditValue(GrEditEventArgs* e) = 0;

    virtual GrSelectionType GetSelectionType() const;
    virtual GrSelectionRange GetSelectionRange() const;

    virtual void OnMouseDown(const GrPoint& location);
    virtual void OnMouseMove(const GrPoint& location, bool pressed);
    virtual void OnMouseUp(const GrPoint& location);
    virtual void OnMouseClick(const GrPoint& location);
    virtual void OnMouseDoubleClick(const GrPoint& location);
    virtual void OnMouseWheel(const GrPoint& location, int delta);
    virtual void OnMouseLeave();
    virtual void OnMouseEnter();
    virtual void OnKeyDown(GrKeys key);
    virtual void OnKeyUp(GrKeys key);
    virtual void OnGotFocus();
    virtual void OnLostFocus();
    virtual void OnSizeChanged(const GrRect& rect);
    virtual void OnUpdate();
    virtual void OnPaint(void* painterDevice, const GrRect& clipping);
    virtual void OnFontChanged();

    virtual bool CanEdit(GrItem* pItem, GrEditingReason reason);

protected:
    virtual void OnGridCoreAttached();

public:
    GrRect ClientToScreen(const GrRect& rect) const;
};