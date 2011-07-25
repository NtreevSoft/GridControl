﻿// VsPkg.cs : Implementation of ToolboxInstaller
//

using System;
using System.Diagnostics;
using System.Globalization;
using System.Runtime.InteropServices;
using System.ComponentModel.Design;
using Microsoft.Win32;
using Microsoft.VisualStudio.Shell.Interop;
using Microsoft.VisualStudio.OLE.Interop;
using Microsoft.VisualStudio.Shell;
using System.Reflection;
using Ntreev.Windows.Forms.Grid;
using System.Drawing.Design;

namespace NtreevSoft.ToolboxInstaller
{
    /// <summary>
    /// This is the class that implements the package exposed by this assembly.
    ///
    /// The minimum requirement for a class to be considered a valid package for Visual Studio
    /// is to implement the IVsPackage interface and register itself with the shell.
    /// This package uses the helper classes defined inside the Managed Package Framework (MPF)
    /// to do it: it derives from the Package class that provides the implementation of the 
    /// IVsPackage interface and uses the registration attributes defined in the framework to 
    /// register itself and its components with the shell.
    /// </summary>
    // This attribute tells the registration utility (regpkg.exe) that this class needs
    // to be registered as package.
    [PackageRegistration(UseManagedResourcesOnly = true)]
    // A Visual Studio component can be registered under different regitry roots; for instance
    // when you debug your package you want to register it in the experimental hive. This
    // attribute specifies the registry root to use if no one is provided to regpkg.exe with
    // the /root switch.
    [DefaultRegistryRoot("Software\\Microsoft\\VisualStudio\\9.0")]
    // This attribute is used to register the informations needed to show the this package
    // in the Help/About dialog of Visual Studio.
    [InstalledProductRegistration(false, "#110", "#112", "1.0", IconResourceID = 400)]
    // In order be loaded inside Visual Studio in a machine that has not the VS SDK installed, 
    // package needs to have a valid load key (it can be requested at 
    // http://msdn.microsoft.com/vstudio/extend/). This attributes tells the shell that this 
    // package has a load key embedded in its resources.
    [ProvideLoadKey("Standard", "1.0", "Toolbox Installer", "NtreevSoft", 1)]
    [Guid(GuidList.guidToolboxInstallerPkgString)]
    [ProvideToolboxItems(1, true)]
    public sealed class ToolboxInstallerPackage : Package
    {
        /// <summary>
        /// Default constructor of the package.
        /// Inside this method you can place any initialization code that does not require 
        /// any Visual Studio service because at this point the package object is created but 
        /// not sited yet inside Visual Studio environment. The place to do all the other 
        /// initialization is the Initialize method.
        /// </summary>
        public ToolboxInstallerPackage()
        {
            Trace.WriteLine(string.Format(CultureInfo.CurrentCulture, "Entering constructor for: {0}", this.ToString()));

            this.ToolboxInitialized += new EventHandler(ToolboxInstallerPackage_ToolboxInitialized);
            this.ToolboxUpgraded += new EventHandler(ToolboxInstallerPackage_ToolboxUpgraded);
        }

        void ToolboxInstallerPackage_ToolboxInitialized(object sender, EventArgs e)
        {
            Assembly assembly = Assembly.GetAssembly(typeof(GridControl));

            if (assembly == null)
                return;

            IToolboxService toolboxService = GetService(typeof(IToolboxService)) as IToolboxService;
            
            foreach (ToolboxItem item in ToolboxService.GetToolboxItems(assembly, assembly.CodeBase))
            {
                toolboxService.AddToolboxItem(item, "NtreevSoft Controls");    
            }
        }

        void ToolboxInstallerPackage_ToolboxUpgraded(object sender, EventArgs e)
        {
            
        }
       

        /////////////////////////////////////////////////////////////////////////////
        // Overriden Package Implementation
        #region Package Members

        /// <summary>
        /// Initialization of the package; this method is called right after the package is sited, so this is the place
        /// where you can put all the initilaization code that rely on services provided by VisualStudio.
        /// </summary>
        protected override void Initialize()
        {
            Trace.WriteLine (string.Format(CultureInfo.CurrentCulture, "Entering Initialize() of: {0}", this.ToString()));
            base.Initialize();

        }
        #endregion

    }
}