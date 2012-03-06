#pragma once
#include "GridType.h"
#include "GridCellBase.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Design
{
    /// <summary>
    /// 편집 시작 방식을 지정합니다.
    /// </summary>
    public enum class EditingReasonType : int
    {
        /// <summary>
        /// 추가적인 정보 없이 편집합니다.
        /// </summary>
        None,

        /// <summary>
        /// 마우스 위치를 이용해 편집을 시작합니다.
        /// </summary>
        Mouse,

        /// <summary>
        /// 키를 이용해 편집을 시작합니다.
        /// </summary>
        /// <remarks>
        /// 알파벳과 한글키등을 제외한 숫자키, 특수문자, f1 ~ f12키 등이 해당됩니다.
        /// </remarks>
        Key,

        /// <summary>
        /// 문자열키 정보를 이용하여 편집을 시작합니다.
        /// </summary>
        /// <remarks>
        /// 일반적으로 알파벳에 해당합니다.
        /// </remarks>
        Char,

        /// <summary>
        /// ime키 정보를 이용하여 편집을 시작합니다.
        /// </summary>
        Ime,
    };

    /// <summary>
    /// 편집 시작 방식을 나타내는 개체입니다.
    /// </summary>
    public value class EditingReason
    {
    public: // methods
        /// <summary>
        /// 마우스로 편집이 시작됨을 알리는 <see cref="EditingReason"/>클래스의 새 인스턴스를 초기화합니다.
        /// </summary>
        /// <param name="location">그리드 컨트롤에 대한 마우스 커서의 좌표입니다.</param>
        EditingReason(System::Drawing::Point location);

        /// <summary>
        /// 키로 편집이 시작됨을 알리는 <see cref="EditingReason"/>클래스의 새 인스턴스를 초기화합니다.
        /// </summary>
        /// <param name="key">추가적으로 제공할 키 정보입니다.</param>
        EditingReason(System::Windows::Forms::Keys key);

        /// <summary>
        /// 문자열 키로 편집이 시작됨을 알리는 <see cref="EditingReason"/>클래스의 새 인스턴스를 초기화합니다.
        /// </summary>
        /// <param name="charKey">문자열 키 정보입니다.</param>
        EditingReason(System::Char charKey);

        /// <summary>
        /// ime에 입력에 의해서 편집이 시작됨을 알리는 <see cref="EditingReason"/>클래스의 새 인스턴스를 초기화합니다.
        /// </summary>
        /// <param name="keyValue">키값 정보입니다.</param>
        EditingReason(int keyValue);

    public: // properties
        /// <summary>
        /// 편집 시작 방식을 가져옵니다.
        /// </summary>
        /// <returns>
        /// 편집 시작 방식을 나타내는 <see cref="EditingReasonType"/>입니다.
        /// </returns>
        property Ntreev::Windows::Forms::Grid::Design::EditingReasonType ReasonType
        {
            Ntreev::Windows::Forms::Grid::Design::EditingReasonType get(); 
        }

        /// <summary>
        /// 마우스로 인한 편집시에 마우스 커서의 좌표를 가져옵니다.
        /// </summary>
        property System::Drawing::Point Location
        {
            System::Drawing::Point get();
        }

        /// <summary>
        /// 키로 인한 편집시에 키 데이터를 가져옵니다.
        /// </summary>
        property System::Windows::Forms::Keys Key
        {
            System::Windows::Forms::Keys get();
        }

        /// <summary>
        /// 문자열 키로 인한 편집시에 문자 데이터를 가져옵니다.
        /// </summary>
        property System::Char Char
        {
            System::Char get(); 
        }

        /// <summary>
        /// ime 입력에 의한 키값을 가져옵니다.
        /// </summary>
        property int KeyValue
        {
            int get();
        }

        /// <summary>
        /// 추가적인 데이터로 인한 편집시에 추가적인 데이터를 가져옵니다.
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
    /// 셀을 편집하는 방식을 나타냅니다.
    /// </summary>
    enum class EditStyle : int
    {
        /// <summary>
        /// 단일 컨트롤을 사용하여 셀을 편집합니다.
        /// </summary>
        Control,

        /// <summary>
        /// DropDown에 컨트롤을 부착시켜 셀을 편집할 수 있도록 합니다.
        /// </summary>
        DropDown,

        /// <summary>
        /// Modal 형태를 사용하여 셀을 편집합니다.
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