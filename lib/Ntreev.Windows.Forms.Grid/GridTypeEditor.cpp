#include "StdAfx.h"
#include "GridTypeEditor.h"
#include "GridTextBox.h"
#include "GridListBox.h"
#include "GridCheckBox.h"
#include "GridFlagControl.h"
#include "GridControl.h"


namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Design
{
    EditingReason::EditingReason(System::Drawing::Point pt)
        : data(pt), reasonType(Ntreev::Windows::Forms::Grid::Design::EditingReasonType::Mouse)
    {

    }

    EditingReason::EditingReason(System::Windows::Forms::Keys key)
        : data(key), reasonType(Ntreev::Windows::Forms::Grid::Design::EditingReasonType::Key)
    {

    }

    EditingReason::EditingReason(System::Char charKey)
        : data(charKey), reasonType(Ntreev::Windows::Forms::Grid::Design::EditingReasonType::Char)
    {

    }

    EditingReason::EditingReason(int keyValue)
        : data(keyValue), reasonType(Ntreev::Windows::Forms::Grid::Design::EditingReasonType::Ime)
    {

    }

    EditingReason::EditingReason(const GrEditingReason& reason)
    {
        switch(reason.editingType)
        {
        case GrEditingType_None:
            data = nullptr;
            break;
        case GrEditingType_Mouse:
            data = (System::Drawing::Point)reason.location;
            break;
        case GrEditingType_Key:
            data = (System::Windows::Forms::Keys)reason.key;
            break;
        case GrEditingType_Char:
            data = (System::Char)reason.character;
            break;
        case GrEditingType_Ime:
            data = (int)reason.keyValue;
            break;
        }

        reasonType = (Ntreev::Windows::Forms::Grid::Design::EditingReasonType)reason.editingType;
    }

    GrEditingReason EditingReason::ToNative()
    {
        GrEditingReason reason;

        switch(reasonType)
        {
        case Ntreev::Windows::Forms::Grid::Design::EditingReasonType::Mouse:
            reason.location = this->Location;
            break;
        case Ntreev::Windows::Forms::Grid::Design::EditingReasonType::Key:
            reason.key = (GrKeys)this->Key;
            break;
        case Ntreev::Windows::Forms::Grid::Design::EditingReasonType::Char:
            reason.character = (wchar_t)this->Char;
            break;
        case Ntreev::Windows::Forms::Grid::Design::EditingReasonType::Ime:
            reason.keyValue = this->KeyValue;
            break;
        }

        reason.editingType = (GrEditingType)reasonType;
        return reason;
    }

    EditingReasonType EditingReason::ReasonType::get()
    {
        return reasonType;
    }

    System::Drawing::Point EditingReason::Location::get()
    {
        return (System::Drawing::Point)data;
    }

    System::Windows::Forms::Keys EditingReason::Key::get()
    {
        return (System::Windows::Forms::Keys)data;
    }

    System::Char EditingReason::Char::get()
    {
        return (System::Char)data;
    }

    int EditingReason::KeyValue::get()
    {
        return (int)data;
    }

    System::Object^ EditingReason::Data::get()
    {
        return data;
    }

    TypeEditor^ TypeEditor::FromDataType(System::Type^ dataType)
    {
        using namespace System::ComponentModel;
        using namespace System::Drawing::Design;

        //typeof(IListSource).IsAssignableFrom(typeof(DataTable));
        if(IBindingList::typeid->IsAssignableFrom(dataType) == true)
        {
            return gcnew BindingListTypeEditor(dataType);
        }
        else
        {
            UITypeEditor^ typeEditor = (UITypeEditor^)TypeDescriptor::GetEditor(dataType, UITypeEditor::typeid);
            if(typeEditor != nullptr)
                return gcnew DataTypeEditor(dataType);

            if(dataType == bool::typeid)
            {
                return gcnew BooleanTypeEditor(dataType);
            }
            else if(dataType->IsEnum == true)
            {
                if(dataType->GetCustomAttributes(System::FlagsAttribute::typeid, true)->Length != 0)
                {
                    return gcnew FlagTypeEditor(dataType);
                }
                return gcnew EnumTypeEditor(dataType);
            }
            else if(dataType->IsArray == true)
            {
                return gcnew ListTypeEditor(dataType);
            }
        }
        return gcnew StringTypeEditor(dataType);
    }

    bool StringTypeEditor::CanEdit(ICell^ cell, EditingReason reason)
    {
        switch(reason.ReasonType)
        {
        case EditingReasonType::Ime:
            return true;
        case EditingReasonType::Char:
            return true;
        default:
            return TypeEditor::CanEdit(cell, reason);
        }
    }

    System::Object^ StringTypeEditor::EditValue(IEditorService^ editorService, ICell^ cell, System::Object^ value)
    {
        using namespace System::Windows::Forms;

        GridTextBox^ textBox = gcnew GridTextBox(editorService);

        switch(editorService->ReasonType)
        {
        case EditingReasonType::Ime:
        case EditingReasonType::Key:
        case EditingReasonType::Char:
            break;
        default:
            {
                if(value != nullptr)
                    textBox->Text = value->ToString();
            }
            break;
        }

        textBox->BackColor = cell->BackColor;
        //textBox->BackColor = System::Drawing::Color::Red;
        textBox->ForeColor = cell->ForeColor;
        textBox->Font = cell->Font;
        textBox->Multiline = cell->Multiline;
        textBox->WordWrap = cell->WordWrap;

        if(textBox->Multiline == false)
        {
            textBox->Dock = DockStyle::Fill;
        }
        else
        {
            System::Drawing::Point pt = textBox->GetPositionFromCharIndex(0);

             System::Drawing::Rectangle rect = cell->ClientRectangle;
             rect.X = -pt.X;
             rect.Y = 0;
             rect.Width += pt.X;
             textBox->Bounds = rect;
        }

        //if(textBox->Multiline

        editorService->ShowControl(textBox);
        return textBox->Text;
    }

    System::Object^ EnumTypeEditor::EditValue(IEditorService^ editorService, ICell^ cell, System::Object^ value)
    {
        GridListBox^ listBox = gcnew GridListBox(editorService, this->DataType, value);

        listBox->BackColor = cell->BackColor;
        listBox->ForeColor = cell->ForeColor;
        listBox->Font  = cell->Font;

        listBox->Size = listBox->PreferredSize;

        editorService->DropDownControl(listBox);
        return listBox->Value;;
    }

    System::Object^ FlagTypeEditor::EditValue(IEditorService^ editorService, ICell^ cell, System::Object^ value)
    {
        FlagControl^ flagControl = gcnew FlagControl(editorService, this->DataType);

        flagControl->Value = value;
        flagControl->BackColor = cell->BackColor;
        flagControl->ForeColor = cell->ForeColor;
        flagControl->Font  = cell->Font;

        editorService->DropDownControl(flagControl);
        return flagControl->Value;
    }

    DataTypeEditor::DataTypeEditor(System::Type^ dataType) 
        : TypeEditor(dataType) 
    {
        using namespace System::ComponentModel;
        using namespace System::Drawing::Design;
        m_typeEditor = (UITypeEditor^)TypeDescriptor::GetEditor(dataType, UITypeEditor::typeid);
    }

    System::Object^ DataTypeEditor::EditValue(IEditorService^ editorService, ICell^ /*cell*/, System::Object^ value)
    {
        using namespace System::ComponentModel;
        ITypeDescriptorContext^ context = dynamic_cast<ITypeDescriptorContext^>(editorService->GetService(ITypeDescriptorContext::typeid));
        editorService->IsDropDownResizable = m_typeEditor->IsDropDownResizable;
        return m_typeEditor->EditValue(context, editorService, value);
    }

    EditStyle DataTypeEditor::GetEditStyle()
    {
        using namespace System::Drawing::Design;
        switch(m_typeEditor->GetEditStyle())
        {
        case UITypeEditorEditStyle::DropDown:
            return EditStyle::DropDown;
        case UITypeEditorEditStyle::Modal:
            return EditStyle::Modal;
        default:
            return EditStyle::Control;
        }
    }

    void DataTypeEditor::PaintValue(System::Drawing::Graphics^ graphics, System::Drawing::Rectangle paintRect, ICell^ /*cell*/, System::Object^ value)
    {
        if(value == nullptr || value->ToString() == System::String::Empty)
            return;

        paintRect.Width = 20;
        m_typeEditor->PaintValue(value, graphics, paintRect);
        paintRect.Width--;
        paintRect.Height--;
        graphics->DrawRectangle(System::Drawing::SystemPens::WindowText, paintRect);

        //if(value != nullptr && value->ToString() != System::String::Empty)
        //{
        //    System::Drawing::SolidBrush brush(cell->ForeColor);
        //    graphics->DrawString(System::String::Format("{0}", value), cell->Font, %brush, paintRect.Right, paintRect.Top);
        //}
    }

    System::Object^ BindingListTypeEditor::EditValue(IEditorService^ editorService, ICell^ /*cell*/, System::Object^ value)
    {
        using namespace System::Windows::Forms;

        Ntreev::Windows::Forms::Grid::GridControl^ gridControl = gcnew Ntreev::Windows::Forms::Grid::GridControl();
        gridControl->Dock = DockStyle::Fill;
        gridControl->DataSource = dynamic_cast<System::ComponentModel::IBindingList^>(value);

        editorService->ShowDialog(gridControl);
        return value;
    }

    BooleanTypeEditor::BooleanTypeEditor(System::Type^ dataType)
        : TypeEditor(dataType)
    {
        using namespace System::Windows::Forms::VisualStyles;

        try
        {
            if(System::Windows::Forms::Application::RenderWithVisualStyles == true)
            {
                m_rendererChecked = gcnew VisualStyleRenderer(VisualStyleElement::Button::CheckBox::CheckedNormal);
                m_rendererCheckedHot = gcnew VisualStyleRenderer(VisualStyleElement::Button::CheckBox::CheckedHot);
                m_rendererUnchecked = gcnew VisualStyleRenderer(VisualStyleElement::Button::CheckBox::UncheckedNormal);
                m_rendererUncheckedHot = gcnew VisualStyleRenderer(VisualStyleElement::Button::CheckBox::UncheckedHot);
            }
        }
        catch(System::Exception^)
        {

        }

        //System::Windows::Forms::Application::VisualStyleState
    }

    bool BooleanTypeEditor::CanEdit(ICell^ cell, EditingReason reason)
    {
        return cell->ClientRectangle.Contains(reason.Location);
    }

    System::Object^ BooleanTypeEditor::EditValue(IEditorService^ editorService, ICell^ cell, System::Object^ value)
    {
        GridCheckBox^ checkBox = gcnew GridCheckBox();

        System::Drawing::Rectangle rect = cell->ClientRectangle;

        if(value != nullptr && value->ToString() == bool::TrueString)
            checkBox->Checked = true;
        else
            checkBox->Checked = false;

        //if(editorService->ReasonType != EditingReasonType::Mouse)
        //    checkBox->Checked = !checkBox->Checked;

        checkBox->BackColor = cell->BackColor;
        checkBox->ForeColor = cell->ForeColor;
        checkBox->Font  = cell->Font;
        checkBox->Bounds = rect;
        checkBox->Dock = System::Windows::Forms::DockStyle::Fill;
        checkBox->EditorService = editorService;

        editorService->ShowControl(checkBox);
        return checkBox->Checked;
    }

    void BooleanTypeEditor::PaintValue(System::Drawing::Graphics^ graphics, System::Drawing::Rectangle paintRect, ICell^ cell, System::Object^ value)
    {
        bool checked = false;
        if(value == nullptr)
        {
            checked = false;
        }
        else if(value->GetType() == bool::typeid)
        {
            checked = (bool)value;
        }
        else
        {
            if(value->ToString() == System::Boolean::TrueString)
                checked = true;
            else
                checked = false;
        }

        if(checked == true)
        {
            if(m_rendererChecked != nullptr)
            {
                System::Drawing::Rectangle rectangle = paintRect;
                if(rectangle.Width % 2 == 1)
                    rectangle.Width--;
                if(rectangle.Height % 2 == 1)
                    rectangle.Height--;
                if((cell->State & CellState::Hot) == CellState::Hot)
                    m_rendererCheckedHot->DrawBackground(graphics, rectangle);
                else
                    m_rendererChecked->DrawBackground(graphics, rectangle);
            }
            else
            {
                int x1 = paintRect.Width/2 - 7 + paintRect.Left;
                int y1 = paintRect.Height/2 - 7 + paintRect.Top;
                System::Windows::Forms::ControlPaint::DrawCheckBox(graphics, x1, y1, 13, 13, System::Windows::Forms::ButtonState::Checked);
            }
        }
        else
        {
            if(m_rendererUnchecked != nullptr)
            {
                System::Drawing::Rectangle rectangle = paintRect;
                if(rectangle.Width % 2 == 1)
                    rectangle.Width--;
                if(rectangle.Height % 2 == 1)
                    rectangle.Height--;
                if((cell->State & CellState::Hot) == CellState::Hot)
                    m_rendererUncheckedHot->DrawBackground(graphics, rectangle);
                else
                    m_rendererUnchecked->DrawBackground(graphics, rectangle);
            }
            else
            {
                int x1 = paintRect.Width/2 - 7 + paintRect.Left;
                int y1 = paintRect.Height/2 - 7 + paintRect.Top;
                System::Windows::Forms::ControlPaint::DrawCheckBox(graphics, x1, y1, 13, 13, System::Windows::Forms::ButtonState::Normal);
            }
        }
    }

    ListTypeEditor::ListTypeEditor(System::Type^ dataType)
        : TypeEditor(dataType)
    {

    }

    System::Object^ ListTypeEditor::EditValue(IEditorService^ editorService, ICell^ cell, System::Object^ value)
    {
        System::Drawing::Rectangle rect;
        GridListBox^ listBox = gcnew GridListBox(editorService, this->DataType, value);

        listBox->BackColor = cell->BackColor;
        listBox->ForeColor = cell->ForeColor;
        listBox->Font  = cell->Font;

        listBox->Size = listBox->PreferredSize;

        editorService->DropDownControl(listBox);
        return listBox->SelectedItem;
    }

} /*namespace Design*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/