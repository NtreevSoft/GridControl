//=====================================================================================================================
// Ntreev Grid for .Net 1.0
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
#include "GridColumn.h"
#include "ColumnUITypeEditorForm.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
	ref class WindowsFormsEditorService : System::Windows::Forms::Design::IWindowsFormsEditorService, System::IServiceProvider
    {
		typedef System::Windows::Forms::Form		_Form;
		typedef Ntreev::Windows::Forms::Grid::Private::ColumnUITypeEditorForm _DropDownForm;
		typedef System::Windows::Forms::DialogResult _DialogResult;

	public:
		WindowsFormsEditorService(_Form^ parentForm, _Point location)
        {
            _parentForm = parentForm;
			_location = location;
        }

        virtual void CloseDropDown()
        {
			_dropDown->Controls->Clear();
            _dropDown->Close();
			_parentForm->Activate();
        }

        virtual void DropDownControl(_Control^ control)
        {
			// Control::PrefferedSize가 정상적인 값이 넘어오지 않음. 
			// 아래 함수를 호출하면 원하는 값이 넘어옴.
			delete control->CreateGraphics();

			_dropDown = gcnew _DropDownForm(control);
			_dropDown->SuspendLayout();
			_dropDown->Controls->Add(control);

			_dropDown->Location = _location;
			_Size size = control->Size;
			size.Width  += 2;
			size.Height += 2;

			_dropDown->Size = size;
			control->Dock = System::Windows::Forms::DockStyle::Fill;
			control->Visible = true;
		
			_dropDown->ResumeLayout(true);
            _dropDown->ShowDialog();
        }

		virtual _DialogResult ShowDialog(_Form^ dialog)
        {
			return dialog->ShowDialog();
        }


        virtual object^ GetService(_Type^ serviceType)
        {
			if (serviceType == IWindowsFormsEditorService::typeid)
                return this;
            return nullptr;
        }

	private:
		_Form^			_parentForm;
        _DropDownForm^	_dropDown;
		_Point			_location;
    };

	public ref class ColumnUITypeEditor : Column, IValuePainter
	{
		typedef System::Drawing::Design::UITypeEditor _UITypeEditor;
	public: // methods
		ColumnUITypeEditor();

	public: // properties
		property _EditingType EditingType
		{ 
			virtual _EditingType get() override sealed { return _EditingType::Etc; } 
		}

		property bool PaintValueSupported
		{
			virtual bool get() sealed = IValuePainter::PaintValueSupported::get;
		}

		virtual void PaintValue(_Graphics^ graphics, _Rectangle renderRect, _Rectangle clipRect, IStyle^ style, object^ value);

	protected:
		virtual void OnEditValue(EditValueEventArgs^ e) override;

	private:
		property _UITypeEditor^ UITypeEditor
		{
			_UITypeEditor^ get();
		}

	private:
		_UITypeEditor^ m_typeEditor;
	};
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/