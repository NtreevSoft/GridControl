using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Library.Grid
{
	public abstract class GrCollectionBase<T> : IReadOnlyList<T>
	{
		private readonly List<T> items;

        protected GrCollectionBase()
        {
            this.items = new List<T>();
        }

        protected GrCollectionBase(int capacity)
        {
            this.items = new List<T>(capacity);
        }

        protected GrCollectionBase(IEnumerable<T> collection)
        {
            this.items = new List<T>(collection);
        }

        public T this[int index]
        {
            get { return this.items[index]; }
        }

        public int Count
        {
            get { return this.items.Count; }
        }

        public IEnumerator<T> GetEnumerator()
        {
            throw new NotImplementedException();
        }

        System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
        {
            throw new NotImplementedException();
        }
    }
}
