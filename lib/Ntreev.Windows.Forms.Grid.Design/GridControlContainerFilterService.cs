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
