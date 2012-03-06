#pragma once
#include "GridType.h"
#include "GridCellBase.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Design
{
    /// <summary>
    /// ���� ���� ����� �����մϴ�.
    /// </summary>
    public enum class EditingReasonType : int
    {
        /// <summary>
        /// �߰����� ���� ���� �����մϴ�.
        /// </summary>
        None,

        /// <summary>
        /// ���콺 ��ġ�� �̿��� ������ �����մϴ�.
        /// </summary>
        Mouse,

        /// <summary>
        /// Ű�� �̿��� ������ �����մϴ�.
        /// </summary>
        /// <remarks>
        /// ���ĺ��� �ѱ�Ű���� ������ ����Ű, Ư������, f1 ~ f12Ű ���� �ش�˴ϴ�.
        /// </remarks>
        Key,

        /// <summary>
        /// ���ڿ�Ű ������ �̿��Ͽ� ������ �����մϴ�.
        /// </summary>
        /// <remarks>
        /// �Ϲ������� ���ĺ��� �ش��մϴ�.
        /// </remarks>
        Char,

        /// <summary>
        /// imeŰ ������ �̿��Ͽ� ������ �����մϴ�.
        /// </summary>
        Ime,
    };

    /// <summary>
    /// ���� ���� ����� ��Ÿ���� ��ü�Դϴ�.
    /// </summary>
    public value class EditingReason
    {
    public: // methods
        /// <summary>
        /// ���콺�� ������ ���۵��� �˸��� <see cref="EditingReason"/>Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�.
        /// </summary>
        /// <param name="location">�׸��� ��Ʈ�ѿ� ���� ���콺 Ŀ���� ��ǥ�Դϴ�.</param>
        EditingReason(System::Drawing::Point location);

        /// <summary>
        /// Ű�� ������ ���۵��� �˸��� <see cref="EditingReason"/>Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�.
        /// </summary>
        /// <param name="key">�߰������� ������ Ű �����Դϴ�.</param>
        EditingReason(System::Windows::Forms::Keys key);

        /// <summary>
        /// ���ڿ� Ű�� ������ ���۵��� �˸��� <see cref="EditingReason"/>Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�.
        /// </summary>
        /// <param name="charKey">���ڿ� Ű �����Դϴ�.</param>
        EditingReason(System::Char charKey);

        /// <summary>
        /// ime�� �Է¿� ���ؼ� ������ ���۵��� �˸��� <see cref="EditingReason"/>Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�.
        /// </summary>
        /// <param name="keyValue">Ű�� �����Դϴ�.</param>
        EditingReason(int keyValue);

    public: // properties
        /// <summary>
        /// ���� ���� ����� �����ɴϴ�.
        /// </summary>
        /// <returns>
        /// ���� ���� ����� ��Ÿ���� <see cref="EditingReasonType"/>�Դϴ�.
        /// </returns>
        property Ntreev::Windows::Forms::Grid::Design::EditingReasonType ReasonType
        {
            Ntreev::Windows::Forms::Grid::Design::EditingReasonType get(); 
        }

        /// <summary>
        /// ���콺�� ���� �����ÿ� ���콺 Ŀ���� ��ǥ�� �����ɴϴ�.
        /// </summary>
        property System::Drawing::Point Location
        {
            System::Drawing::Point get();
        }

        /// <summary>
        /// Ű�� ���� �����ÿ� Ű �����͸� �����ɴϴ�.
        /// </summary>
        property System::Windows::Forms::Keys Key
        {
            System::Windows::Forms::Keys get();
        }

        /// <summary>
        /// ���ڿ� Ű�� ���� �����ÿ� ���� �����͸� �����ɴϴ�.
        /// </summary>
        property System::Char Char
        {
            System::Char get(); 
        }

        /// <summary>
        /// ime �Է¿� ���� Ű���� �����ɴϴ�.
        /// </summary>
        property int KeyValue
        {
            int get();
        }

        /// <summary>
        /// �߰����� �����ͷ� ���� �����ÿ� �߰����� �����͸� �����ɴϴ�.
        /// </summary>
        property System::Object^ Data
        {
            System::Object^ get(); 
        }

    public: // variables
        static EditingReason None;

    internal:
        EditingReason(const GrEditingReason& reason);

        GrEditingReason ToNative();

    private: // variables
        Ntreev::Windows::Forms::Grid::Design::EditingReasonType reasonType;
        System::Object^ data;
    };

    /// <summary>
    /// ���� �����ϴ� ����� ��Ÿ���ϴ�.
    /// </summary>
    enum class EditStyle : int
    {
        /// <summary>
        /// ���� ��Ʈ���� ����Ͽ� ���� �����մϴ�.
        /// </summary>
        Control,

        /// <summary>
        /// DropDown�� ��Ʈ���� �������� ���� ������ �� �ֵ��� �մϴ�.
        /// </summary>
        DropDown,

        /// <summary>
        /// Modal ���¸� ����Ͽ� ���� �����մϴ�.
        /// </summary>
        Modal,
    };

    public interface class IEditorService : System::IServiceProvider
    {
        void ShowControl(System::Windows::Forms::Control^ control);
        void DropDownControl(System::Windows::Forms::Control^ control);
        void ShowDialog(System::Windows::Forms::Control^ control);

        void Close();

        property bool IsDropDownResizable
        {
            void set(bool);
        }

        property EditingReasonType ReasonType
        {
            EditingReasonType get();
        }
    };

    public ref class TypeEditor abstract
    {
    public:
        TypeEditor(System::Type^ dataType) : m_dataType(dataType) {}

        virtual bool CanEdit(ICell^ /*cell*/, EditingReason reason)
        {
            if(reason.ReasonType == EditingReasonType::Mouse)
                return true;
            return false; 
        }

        virtual System::Object^ EditValue(IEditorService^ /*editorService*/, ICell^ /*cell*/, System::Object^ value) { return value; }

        virtual void PaintValue(System::Drawing::Graphics^ /*graphics*/, System::Drawing::Rectangle /*paintRect*/, ICell^ /*cell*/, System::Object^ /*value*/) {};

        virtual EditStyle GetEditStyle() { return EditStyle::Control; }

        static TypeEditor^ FromDataType(System::Type^ dataType);

    protected:
        property System::Type^ DataType
        {
            System::Type^ get() { return m_dataType; }
        }

    public:
        property Ntreev::Windows::Forms::Grid::ViewType ViewType
        {
            virtual Ntreev::Windows::Forms::Grid::ViewType get() { return Ntreev::Windows::Forms::Grid::ViewType::Text; }
        }

    private:
        System::Type^ m_dataType;
    };

    ref class StringTypeEditor : TypeEditor
    {
    public:
        StringTypeEditor() : TypeEditor(System::String::typeid) {}

        StringTypeEditor(System::Type^ dataType) : TypeEditor(dataType) {}

        virtual bool CanEdit(ICell^ cell, EditingReason reason) override;

        virtual System::Object^ EditValue(IEditorService^ editorService, ICell^ cell, System::Object^ value) override;

    public:
        static StringTypeEditor Default;
    };

    ref class EnumTypeEditor : TypeEditor
    {
    public:
        EnumTypeEditor(System::Type^ dataType) : TypeEditor(dataType) {}

        virtual System::Object^ EditValue(IEditorService^ editorService, ICell^ cell, System::Object^ value) override;

        virtual EditStyle GetEditStyle() override { return EditStyle::DropDown; }
    };

    ref class FlagTypeEditor : TypeEditor
    {
    public:
        FlagTypeEditor(System::Type^ dataType) : TypeEditor(dataType) {}

        virtual System::Object^ EditValue(IEditorService^ editorService, ICell^ cell, System::Object^ value) override;

        virtual EditStyle GetEditStyle() override { return EditStyle::DropDown; }
    };

    ref class DataTypeEditor : TypeEditor
    {
    public:
        DataTypeEditor(System::Type^ dataType);

        virtual System::Object^ EditValue(IEditorService^ editorService, ICell^ cell, System::Object^ value) override;

        virtual EditStyle GetEditStyle() override;

        virtual void PaintValue(System::Drawing::Graphics^ graphics, System::Drawing::Rectangle paintRect, ICell^ cell, System::Object^ value) override;

    public:
        property Ntreev::Windows::Forms::Grid::ViewType ViewType
        {
            virtual Ntreev::Windows::Forms::Grid::ViewType get() override 
            {
                if(m_typeEditor->GetPaintValueSupported() == true)
                    return Ntreev::Windows::Forms::Grid::ViewType::Icon;
                return Ntreev::Windows::Forms::Grid::ViewType::Text; 
            }
        }

    private:
        System::Drawing::Design::UITypeEditor^ m_typeEditor;
    };

    ref class BindingListTypeEditor : TypeEditor
    {
    public:
        BindingListTypeEditor(System::Type^ dataType) : TypeEditor(dataType) {}

        virtual System::Object^ EditValue(IEditorService^ editorService, ICell^ cell, System::Object^ value) override;

        virtual EditStyle GetEditStyle() override { return EditStyle::Modal; }
    };

    ref class BooleanTypeEditor : TypeEditor
    {
    public:
        BooleanTypeEditor(System::Type^ dataType);

        virtual bool CanEdit(ICell^ cell, EditingReason reason) override;

        virtual System::Object^ EditValue(IEditorService^ editorService, ICell^ cell, System::Object^ value) override;

        virtual void PaintValue(System::Drawing::Graphics^ graphics, System::Drawing::Rectangle paintRect, ICell^ cell, System::Object^ value) override;

    public:
        property Ntreev::Windows::Forms::Grid::ViewType ViewType
        {
            virtual Ntreev::Windows::Forms::Grid::ViewType get() override { return Ntreev::Windows::Forms::Grid::ViewType::Custom; }
        }

    private:
        System::Windows::Forms::VisualStyles::VisualStyleRenderer^ m_rendererChecked;
        System::Windows::Forms::VisualStyles::VisualStyleRenderer^ m_rendererCheckedHot;
        System::Windows::Forms::VisualStyles::VisualStyleRenderer^ m_rendererUnchecked;
        System::Windows::Forms::VisualStyles::VisualStyleRenderer^ m_rendererUncheckedHot;
    };

    ref class ListTypeEditor : TypeEditor
    {
    public:
        ListTypeEditor(System::Type^ dataType);

        virtual System::Object^ EditValue(IEditorService^ editorService, ICell^ cell, System::Object^ value) override;
    };
} /*namespace Design*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/