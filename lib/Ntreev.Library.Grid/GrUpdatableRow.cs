using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public abstract class GrUpdatableRow : GrRow
    {
        public virtual bool ShouldUpdate()
        {
            return false;
        }

        public void Update()
        {
            this.Update(false);
        }

        public virtual void Update(bool force)
        {

        }

        public virtual int GetUpdatePriority()
        {
            return int.MaxValue;
        }

        public virtual bool ShouldClip(GrRect displayRect, int horizontal, int vertical)
        {
            return false;
        }

        public virtual void Clip(GrRect displayRect, int horizontal, int vertical)
        {

        }

        public virtual int GetClipPriority()
        {
            return int.MaxValue;
        }

        protected override void OnFitted()
        {
            base.OnFitted();

            if (this.GridCore == null)
                return;
            this.GridCore.GetRootRow().SetFitChanged();
        }

        protected override void OnHeightChanged()
        {
            base.OnHeightChanged();

            if (this.GridCore == null)
                return;
            this.GridCore.GetRootRow().SetHeightChanged();
            this.GridCore.Invalidate();
        }
    }
}
