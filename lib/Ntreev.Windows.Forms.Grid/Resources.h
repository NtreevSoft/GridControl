//=====================================================================================================================
// Ntreev Grid for .Net 2.0.5190.32793
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

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Properties
{
    ref class Resources
    {
        static Resources()
        {
            m_resourceManager = gcnew System::Resources::ResourceManager("GridControl.Resources", System::Reflection::Assembly::GetExecutingAssembly());

            m_rowEditing = dynamic_cast<System::Drawing::Bitmap^>(m_resourceManager->GetObject("RowEditing"));
            m_rowFocused = dynamic_cast<System::Drawing::Bitmap^>(m_resourceManager->GetObject("RowFocused"));
            m_insertionRow = dynamic_cast<System::Drawing::Bitmap^>(m_resourceManager->GetObject("InsertionRow"));
            m_insertionRowFocused = dynamic_cast<System::Drawing::Bitmap^>(m_resourceManager->GetObject("InsertionRowFocused"));
            m_errorBitmap = dynamic_cast<System::Drawing::Bitmap^>(m_resourceManager->GetObject("Error"));
        }

    public:
        static System::String^ GetString(System::String^ name)
        {
            return m_resourceManager->GetString(name, System::Windows::Forms::Application::CurrentCulture);
        }

    public: // properties bitmap

        static property System::Drawing::Bitmap^ RowEditing
        {
            System::Drawing::Bitmap^ get() { return m_rowEditing; } 
        }

        static property System::Drawing::Bitmap^ RowFocused
        {
            System::Drawing::Bitmap^ get() { return m_rowFocused; } 
        }

        static property System::Drawing::Bitmap^ InsertionRow
        {
            System::Drawing::Bitmap^ get() { return m_insertionRow; } 
        }

        static property System::Drawing::Bitmap^ InsertionRowFocused
        {
            System::Drawing::Bitmap^ get() { return m_insertionRowFocused; } 
        }

        static property System::Drawing::Bitmap^ Error
        {
            System::Drawing::Bitmap^ get() { return m_errorBitmap; }
        }

    public: // properties string

        static property System::String^ DefaultColumnName
        { 
            System::String^ get() { return GetString("DefaultColumnName"); }
        }

        static property System::String^ DefaultTooltip
        { 
            System::String^ get() { return GetString("DefaultTooltip"); }
        }

        static property System::String^ SetValueException
        { 
            System::String^ get() { return GetString("SetValueException"); }
        }

        static property System::String^ RemoveInsertionRowException
        {
            System::String^ get() { return GetString("RemoveInsertionRowException"); }
        }

        static property System::String^ NoColumnException 
        { 
            System::String^ get() { return GetString("NoColumnException"); }
        }

        static property System::String^ Width
        {
            System::String^ get() { return GetString("Width"); }
        }

        static property System::String^ Height
        {
            System::String^ get() { return GetString("Height"); }
        }

        static property System::String^ DataSourceInitializingFailed
        {
            System::String^ get() { return GetString("DataSourceInitializingFailed"); }
        }

    private: // variables

        static System::Resources::ResourceManager^ m_resourceManager;

        static System::Drawing::Bitmap^ m_rowEditing;
        static System::Drawing::Bitmap^ m_rowFocused;
        static System::Drawing::Bitmap^ m_insertionRow;
        static System::Drawing::Bitmap^ m_insertionRowFocused;
        static System::Drawing::Bitmap^ m_errorBitmap;
    };
} /*namespace Properties*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/

typedef Ntreev::Windows::Forms::Grid::Properties::Resources _Resources;