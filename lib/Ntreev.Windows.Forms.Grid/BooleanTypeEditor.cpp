#pragma once
#include "stdafx.h"
#include "BooleanTypeEditor.h"
#include "CheckBox.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Design
{
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


    bool BooleanTypeEditor::CanEdit(Ntreev::Windows::Forms::Grid::ICell^ cell, Ntreev::Windows::Forms::Grid::EditingReason reason)
    {
        return cell->ClientRectangle.Contains(reason.Location);
    }

    System::Object^ BooleanTypeEditor::EditValue(Ntreev::Windows::Forms::Grid::Design::IEditorService^ editorService, Ntreev::Windows::Forms::Grid::ICell^ cell, System::Object^ value)
    {
        Ntreev::Windows::Forms::Grid::Design::Controls::CheckBox^ checkBox = gcnew Ntreev::Windows::Forms::Grid::Design::Controls::CheckBox(editorService);

        System::Drawing::Rectangle rect = cell->ClientRectangle;

        if(value != nullptr && value->ToString() == bool::TrueString)
            checkBox->Checked = true;
        else
            checkBox->Checked = false;

        checkBox->BackColor = cell->BackColor;
        checkBox->ForeColor = cell->ForeColor;
        checkBox->Font = cell->Font;
        checkBox->Bounds = rect;
        checkBox->Dock = System::Windows::Forms::DockStyle::Fill;

        editorService->ShowControl(checkBox);
        return checkBox->Checked;
    }

    void BooleanTypeEditor::PaintValue(System::Drawing::Graphics^ graphics, System::Drawing::Rectangle paintRect, Ntreev::Windows::Forms::Grid::ICell^ cell, System::Object^ value)
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

    Ntreev::Windows::Forms::Grid::ViewType BooleanTypeEditor::ViewType::get()
    {
        return Ntreev::Windows::Forms::Grid::ViewType::Custom; 
    }

} /*namespace Design*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/