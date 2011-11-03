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


#include "StdAfx.h"
#include "GridColumnUITypeEditor.h"
#include "GridControl.h"
#include "GridCell.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Columns
{
	ref class InternalTypeDescriptorContext : System::ComponentModel::ITypeDescriptorContext
	{
	public:
		InternalTypeDescriptorContext(object^ instance, _GridControl^ gridControl)
			: m_instance(instance), m_gridControl(gridControl), m_propertyDescriptor(nullptr)
		{

		}

		virtual void OnComponentChanged()
		{
			
		}

		virtual bool OnComponentChanging()
		{
			return false;
		}

		property System::ComponentModel::IContainer^ Container
		{
			virtual System::ComponentModel::IContainer^ get()
			{
				return m_gridControl->Container;
			}
		}

		property object^ Instance
		{
			virtual object^ get()
			{
				return m_instance;
			}
		}

		property _PropertyDescriptor^ PropertyDescriptor
		{
			virtual _PropertyDescriptor^ get()
			{
				return nullptr;
			}
		}

		virtual object^ GetService(System::Type^ serviceType)
		{
			object^ service = m_gridControl->GetInternalService(serviceType);
			return service;
		}

	private:
		object^ m_instance;
		_GridControl^ m_gridControl;
		_PropertyDescriptor^ m_propertyDescriptor;
	};

	ColumnUITypeEditor::ColumnUITypeEditor()
	{
		
	}

	System::Drawing::Design::UITypeEditor^ ColumnUITypeEditor::UITypeEditor::get()
	{
		using namespace System::ComponentModel;

		if(m_typeEditor == nullptr)
		{
			m_typeEditor = (System::Drawing::Design::UITypeEditor^)TypeDescriptor::GetEditor(this->DataType, System::Drawing::Design::UITypeEditor::typeid);
		}

		return m_typeEditor;
	}

	bool ColumnUITypeEditor::PaintValueSupported::get()
	{
		if(this->UITypeEditor == nullptr)
			return false;
 		return UITypeEditor->GetPaintValueSupported();
	}

	void ColumnUITypeEditor::PaintValue(_Graphics^ graphics, _Rectangle renderRect, _Rectangle clipRect, IStyle^ /*style*/, object^ value)
	{
		UITypeEditor->PaintValue(gcnew System::Drawing::Design::PaintValueEventArgs(nullptr, value, graphics, clipRect));
		renderRect.Width--;
		renderRect.Height--;
		graphics->DrawRectangle(System::Drawing::Pens::Black, renderRect);
		//m_typeEditor->PaintValue(value, graphic, renderRect);
	}

	void ColumnUITypeEditor::OnEditValue(EditValueEventArgs^ e)
	{
		using namespace System::ComponentModel;
		using namespace System::Drawing::Design;
		using namespace System::Windows::Forms;

		if(this->UITypeEditor == nullptr)
			return;

		Form^ owner = GridControl->FindForm();
		while(owner->ParentForm != nullptr)
			owner = owner->ParentForm;

		if(dynamic_cast<IBindingList^>(e->Value) != nullptr)
		{
			IBindingList^ bindingList = dynamic_cast<IBindingList^>(e->Value);
			
			Form form;
			_GridControl^ gridControl = gcnew _GridControl();
			gridControl->Dock = DockStyle::Fill;
			gridControl->DataSource = bindingList;

			form.Controls->Add(gridControl);

			form.ShowDialog(owner);
			e->CancelEdit = true;
		}
		else
		{
			_Rectangle rect = GridControl->FocusedCell->DisplayRectangle;
			_Point location(rect.Left, rect.Bottom);
			location = GridControl->PointToScreen(location);


			InternalTypeDescriptorContext^ typeDescriptorContext = gcnew InternalTypeDescriptorContext(GridControl->FocusedRow->Component, GridControl);
			object^ value = UITypeEditor->EditValue(typeDescriptorContext, gcnew WindowsFormsEditorService(owner, location), e->Value);

			if(object::Equals(value, e->Value) == false)
				e->Value = value;
		}
		e->SuppressEditing = true;
	}
} /*namespace Columns*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/