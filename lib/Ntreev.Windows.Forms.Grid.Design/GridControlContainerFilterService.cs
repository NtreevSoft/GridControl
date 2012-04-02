#region License
//Ntreev Grid for .Net 2.0.4475.19551
//https://github.com/NtreevSoft/GridControl

//Released under the MIT License.

//Copyright (c) 2010 Ntreev Soft co., Ltd.

//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
//documentation files (the "Software"), to deal in the Software without restriction, including without limitation the 
//rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit 
//persons to whom the Software is furnished to do so, subject to the following conditions:

//The above copyright notice and this permission notice shall be included in all copies or substantial portions of the 
//Software.

//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
//WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
//COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
//OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#endregion


using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using System.Collections;

namespace Ntreev.Windows.Forms.Grid.Design
{
    class GridControlContainerFilterService : ContainerFilterService
    {
        ContainerFilterService existService = null;

        public GridControlContainerFilterService()
        {

        }

        public GridControlContainerFilterService(ContainerFilterService existService)
        {
            this.existService = existService;
        }
        
        public override ComponentCollection FilterComponents(ComponentCollection components)
        {
            if (this.existService != null)
            {
                components = this.existService.FilterComponents(components);
            }

            List<IComponent> list = new List<IComponent>(components.Count);
            List<Column> columns = new List<Column>(components.Count);

            foreach (IComponent item in components)
            {
                if (item is Column == false)
                {
                    list.Add(item);
                }
                else
                {
                    Column column = item as Column;
                    if (column.Index < 0)
                    {
                        list.Add(item);
                    }
                    else
                    {
                        int index = list.IndexOf(column.GridControl);
                        if (index < 0)
                        {
                            list.Add(item);
                        }
                        else
                        {
                            columns.Add(column);
                        }
                    }
                }
            }

            foreach (Column item in columns)
            {
                int index = list.IndexOf(item.GridControl);
                list.Insert(index, item);
            }
            
            return new ComponentCollection(list.ToArray());
        }
    }
}