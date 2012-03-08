//=====================================================================================================================
// Ntreev Grid for .Net 2.0.0.0
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


#include "stdafx.h"

using namespace System;
using namespace System::Reflection;
using namespace System::Runtime::CompilerServices;
using namespace System::Runtime::InteropServices;
using namespace System::Security::Permissions;

//
// 어셈블리의 일반 정보는 다음 특성 집합을 통해 제어됩니다.
// 어셈블리와 관련된 정보를 수정하려면
// 이 특성 값을 변경하십시오.
//
[assembly:AssemblyTitleAttribute("Ntreev.Windows.Forms.Grid")];
[assembly:AssemblyDescriptionAttribute("")];
[assembly:AssemblyConfigurationAttribute("")];
[assembly:AssemblyCompanyAttribute("NtreevSoft Co.,Ltd.")];
[assembly:AssemblyProductAttribute("GridControl")];
[assembly:AssemblyCopyrightAttribute("Copyright (c) NtreevSoft Co.,Ltd. 2010")];
[assembly:AssemblyTrademarkAttribute("")];
[assembly:AssemblyCultureAttribute("")];

//
// 어셈블리의 버전 정보는 다음 네 가지 값으로 구성됩니다.
//
// 주 버전
// 부 버전
// 빌드 번호
// 수정 버전
//
// 모든 값을 지정하거나 아래와 같이 '*'를 사용하여 빌드 번호 및 수정 버전이 자동으로
// 지정되도록 할 수 있습니다.

[assembly:AssemblyVersionAttribute("2.0.*")];
//[assembly:AssemblyVersionAttribute("1.1.*")];

[assembly:ComVisible(false)];

[assembly:CLSCompliantAttribute(true)];

[assembly:SecurityPermission(SecurityAction::RequestMinimum, UnmanagedCode = true)];

[assembly:System::Runtime::CompilerServices::InternalsVisibleTo("Ntreev.Windows.Forms.Grid.Design, "
                                                                "PublicKey=00240000048000009400000006020000002400005253413100040000"
                                                                "010001006345cf79e806cbeefceea2a6963996ae219a19a62530c54dadf6d153ac"
                                                                "3d5469e5cc2e73520f18da28cfd0966171e825369b52aee496bddff0c8aa146115"
                                                                "b55b3a2af80349841f0e3e57ed7c49cbf4cdb092f76723beaac70cdb7aba8f314c"
                                                                "d781a803205a424c50ef31a76edae25dc2afd846d260b6bc03ca4218e620a2bda8")];

[assembly:System::Runtime::CompilerServices::InternalsVisibleTo("GridControlLibraryTest, "
                                                                "PublicKey=00240000048000009400000006020000002400005253413100040000"
                                                                "010001006345cf79e806cbeefceea2a6963996ae219a19a62530c54dadf6d153ac"
                                                                "3d5469e5cc2e73520f18da28cfd0966171e825369b52aee496bddff0c8aa146115"
                                                                "b55b3a2af80349841f0e3e57ed7c49cbf4cdb092f76723beaac70cdb7aba8f314c"
                                                                "d781a803205a424c50ef31a76edae25dc2afd846d260b6bc03ca4218e620a2bda8")];