#include "StdAfx.h"
#include "GridColumnUITypeEditor.h"
#include "GridControl.h"
#include "GridCell.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
	ref class TypeDescriptorContext : System::ComponentModel::ITypeDescriptorContext
	{
	public:
		TypeDescriptorContext(object^ instance, _GridControl^ gridControl, _PropertyDescriptor^ propertyDescriptor)
			: m_instance(instance), m_gridControl(gridControl), m_propertyDescriptor(propertyDescriptor)
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
				return m_propertyDescriptor;
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
		if(m_typeEditor == nullptr)
		{
			if(this->PropertyDescriptor == nullptr)
			{
				m_typeEditor = (System::Drawing::Design::UITypeEditor^)System::ComponentModel::TypeDescriptor::GetEditor(DataType, System::Drawing::Design::UITypeEditor::typeid);
			}
			else
			{
				m_typeEditor = (System::Drawing::Design::UITypeEditor^)this->PropertyDescriptor->GetEditor(System::Drawing::Design::UITypeEditor::typeid);
			}
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

		_Rectangle rect = GridControl->FocusedCell->DisplayRectangle;
		_Point location(rect.Left, rect.Bottom);
		location = GridControl->PointToScreen(location);

		TypeDescriptorContext^ typeDescriptorContext = gcnew TypeDescriptorContext(GridControl->FocusedRow->Component, GridControl, this->PropertyDescriptor);
		object^ value = UITypeEditor->EditValue(typeDescriptorContext, gcnew WindowsFormsEditorService(owner, location), e->Value);

		
		if(object::Equals(value, e->Value) == false)
			e->Value = value;
		e->SuppressEditing = true;
	}
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/