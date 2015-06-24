//using Ntreev.Library.Grid;
//using System;
//using System.Collections;
//using System.Collections.Generic;
//using System.ComponentModel;
//using System.Drawing;
//using System.Drawing.Design;
//using System.Linq;
//using System.Text;
//using System.Threading.Tasks;

//namespace Ntreev.Windows.Forms.Grid
//{
//    [Editor("Ntreev.Windows.Forms.Grid.Design.StyleColorCollectionEditor, Ntreev.Windows.Forms.Grid.Design, Version=2.0.4510.20986, Culture=neutral, PublicKeyToken=7a9d7c7c4ba5dfca", typeof(UITypeEditor))]
//    public class StyleColorCollection : GridObject, IList<Color>, IList
//    {
//        private Color m_defaultColor;
//        private List<GrColor> m_s;

//        internal StyleColorCollection(List<GrColor> container, Color color)
//        {
//            m_s = container;
//            m_defaultColor = color;
//        }

//        public void RemoveAt(int index)
//        {
//            Invalidate();
//            m_s.RemoveAt(index);
//        }

//        public void Insert(int index, Color item)
//        {
//            {
//                Invalidate();
//                m_s.Insert(index, item);
//            }
//        }

//        public void CopyTo(Color[] array, int arrayIndex)
//        {
//            (m_s as ICollection).CopyTo(array, arrayIndex);
//        }

//        public void Clear()
//        {
//            Invalidate();
//            m_s.Clear();
//        }

//        public void Add(Color item)
//        {
//            m_s.Add(item);
//            if (this.GridControl != null)
//                this.GridControl.Invalidate();
//        }

//        public int IndexOf(Color item)
//        {
//            return m_s.IndexOf(item);
//        }

//        public bool Remove(Color item)
//        {
//            Invalidate();
//            return m_s.Remove(item);
//        }

//        public bool Contains(Color item)
//        {
//            return m_s.Contains(item);
//        }

//        public Color this[int index]
//        {
//            get
//            {
//                return m_s[index];
//            }
//            set
//            {
//                m_s[index] = item;
//                Invalidate();
//            }
//        }

//        public int Count
//        {
//            get
//            {
//                return m_s.Count;
//            }
//        }

//        public bool IsReadOnly
//        {
//            get
//            {
//                return m_s.IsReadOnly;
//            }
//        }

        

//        internal Color DefaultColor
//        {
//            get
//            {
//                return m_defaultColor;
//            }
//        }

//        private void Invalidate()
//        {
//            if (this.GridControl != null)
//                this.GridControl.Invalidate();
//        }

//        IEnumerator IEnumerable.GetEnumerator()
//        {
//            return (m_s as IEnumerable).GetEnumerator();
//        }
//        IEnumerator<Color> IEnumerable<Color>.GetEnumerator()
//        {
//            return (m_s as IEnumerable<Color>).GetEnumerator();
//        }

//        void IList.Remove(object item);
//        void IList.Insert(int index, object item);
//        void ICollection.CopyTo(Array array, int arrayIndex);
//        int IList.IndexOf(object item);
//        int IList.Add(object item);
//        bool IList.Contains(object item);


//        object IList.this[int index]
//        {
//            get;
//            set;
//        }

//        object ICollection.SyncRoot
//        {
//            get;
//        }

//        bool IList.IsFixedSize
//        {
//            get;
//        }

//        bool ICollection.IsSynchronized
//        {
//            get;
//        }
//    }
//}
