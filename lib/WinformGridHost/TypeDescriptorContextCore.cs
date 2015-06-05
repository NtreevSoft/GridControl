using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;

namespace Ntreev.Windows.Forms.Grid
{
    class TypeDescriptorContextCore : ITypeDescriptorContext
    {
        private readonly Column m_column;
        private readonly Row m_row;

        public TypeDescriptorContextCore(Cell cell)
        {
            m_column = cell.Column;
            m_row = cell.Row;
        }

        public object GetService(Type serviceType)
        {
            IServiceProvider serviceProvider = m_column as IServiceProvider;
            if (serviceProvider != null)
            {
                return serviceProvider.GetService(serviceType);
            }

            return null;
        }

        public void OnComponentChanged()
        {

        }

        public bool OnComponentChanging()
        {
            return false;
        }

        public IContainer Container
        {
            get
            {
                ISite site = m_column as ISite;
                if (site != null)
                {
                    return site.Container;
                }

                return null;
            }
        }

        public object Instance
        {
            get
            {
                return m_row;
            }
        }

        public PropertyDescriptor PropertyDescriptor
        {
            get
            {
                return m_column.PropertyDescriptor;
            }
        }
    }
}
