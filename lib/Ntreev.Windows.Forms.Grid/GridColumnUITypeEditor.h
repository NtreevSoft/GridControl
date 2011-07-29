﻿#pragma once
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