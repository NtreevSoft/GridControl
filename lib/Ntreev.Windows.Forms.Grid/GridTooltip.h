//=====================================================================================================================
// Ntreev Grid for .Net 1.1.4324.22060
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
#include "GridObject.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    /// <summary>
    /// 툴팁 사용방법에 대한 메서드를 정의합니다.
    /// </summary>
    public interface class IToolTip
    {
        /// <summary>
        /// 툴팁을 표시합니다.
        /// </summary>
        /// <remarks>
        /// 툴팁의 내용은 현재 마우스 커서 기준으로 표시 됩니다.
        /// </remarks>
        /// <param name="text">툴팁에 표시할 문자열입니다.</param>
        void Show(System::String^ text);

        /// <summary>
        /// 툴팁을 감춥니다.
        /// </summary>
        void Hide();
    };


    private ref class ToolTipItem
    {
    public:
        ToolTipItem();

        void Show(System::String^ text, System::IntPtr handle);
        void Hide();

    private:
        void* m_tooltip;
        bool  m_created;
    };

    ref class ToolTip : IToolTip
    {
    internal:
        ToolTip(GridControl^ gridControl, int count);
    public:
        virtual void Hide();
        virtual void Show(System::String^ text);

    private:
        bool m_showed;
        GridControl^ m_gridControl;

        ref class ToolTipItemCollection : System::Collections::Generic::List<ToolTipItem^>
        {
        public: // methods
            ToolTipItemCollection() : m_previousIndex(0), m_currentIndex(0)
            {

            }

            void MoveNext()
            {
                if(Count == 0)
                    throw gcnew System::NullReferenceException();
                m_previousIndex = m_currentIndex;
                m_currentIndex  = (m_currentIndex + 1) % Count;
            }

        public: // properties
            property ToolTipItem^ Current
            {
                ToolTipItem^ get() { return default[m_currentIndex]; }
            }

            property ToolTipItem^ Previous
            {
                ToolTipItem^ get() { return default[m_previousIndex]; }
            }

        private: // variables
            int m_previousIndex;
            int m_currentIndex;
        } m_toolTips;
    };
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/