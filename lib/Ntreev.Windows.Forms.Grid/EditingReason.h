#pragma once
#include "Types.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
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
        property Ntreev::Windows::Forms::Grid::EditingReasonType ReasonType
        {
            Ntreev::Windows::Forms::Grid::EditingReasonType get(); 
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

    internal: // methods

        EditingReason(const GrEditingReason& reason);

        GrEditingReason ToNative();

    private: // variables

        Ntreev::Windows::Forms::Grid::EditingReasonType reasonType;
        System::Object^ data;
    };
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/