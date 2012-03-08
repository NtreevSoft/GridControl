#pragma once
#include "Types.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
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
        property Ntreev::Windows::Forms::Grid::EditingReasonType ReasonType
        {
            Ntreev::Windows::Forms::Grid::EditingReasonType get(); 
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

    internal: // methods

        EditingReason(const GrEditingReason& reason);

        GrEditingReason ToNative();

    private: // variables

        Ntreev::Windows::Forms::Grid::EditingReasonType reasonType;
        System::Object^ data;
    };
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/