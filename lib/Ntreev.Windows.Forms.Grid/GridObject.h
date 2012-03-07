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

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    ref class GridControl;

    /// <summary>
    /// GridControl에 종속되어 사용되는 개체를 나타냅니다.
    /// </summary>
    public ref class GridObject abstract
    {
    public: // methods

        /// <summary>
        /// <see cref="GridObject"/>클래스의 새 인스턴스를 초기화합니다.
        /// </summary>
        GridObject();

        /// <summary>
        /// <see cref="GridObject"/>클래스의 새 인스턴스를 초기화합니다.
        /// </summary>
        /// <param name="gridControl">종속될 그리드 컨트롤의 인스턴스입니다.</param>
        GridObject(Ntreev::Windows::Forms::Grid::GridControl^ gridControl);

    public: // properties
        /// <summary>
        /// 종속된 그리드 컨트롤의 인스턴스를 가져옵니다.
        /// </summary>
        /// <returns>
        /// 종속되었다면 그리드 컨트롤의 인스턴스를, 그렇지 않다면 null을 반환합니다.
        /// </returns>
#ifdef _DEBUG
        [System::ComponentModel::CategoryAttribute("Debug")]
#else
        [System::ComponentModel::BrowsableAttribute(false)]
#endif
        property Ntreev::Windows::Forms::Grid::GridControl^ GridControl 
        {
            virtual Ntreev::Windows::Forms::Grid::GridControl^ get() sealed;
        internal:
            void set(Ntreev::Windows::Forms::Grid::GridControl^);
        }

    internal: // methods

        virtual void OnGridControlAttachedInternal();

        virtual void OnGridControlDetachedInternal();

    internal: // properties

        property GrGridCore* GridCore
        {
            GrGridCore* get();
        }

        property GrItemSelector* Selector
        {
            GrItemSelector* get();
        }

        property GrFocuser* Focuser
        {
            GrFocuser* get();
        }

    private: // variables

        GrGridCore* m_pGridCore;
        GrGridPainter* m_pGridPainter;
        GrItemSelector* m_pItemSelector;
        GrFocuser* m_pFocuser;
        Ntreev::Windows::Forms::Grid::GridControl^  m_gridControl;
    };
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/