using System;
using System.Collections;
using System.Runtime.InteropServices;
using Gst.GLib;
using Gst;
using Gst.Interfaces;

namespace Gst.CorePlugins {
#region Autogenerated code
	[GTypeName ("GstFileSrc")]
	public sealed class FileSrc : Gst.Base.BaseSrc {
		public FileSrc (IntPtr raw) : base (raw) { }

		[DllImport("libgstreamer-0.10.dll") ]
		static extern IntPtr gst_element_factory_make (IntPtr element, IntPtr name);

		public FileSrc (string name) : base (IntPtr.Zero) {
			IntPtr native_name = Gst.GLib.Marshaller.StringToPtrGStrdup (name);
			IntPtr native_element = Gst.GLib.Marshaller.StringToPtrGStrdup ("filesrc");
			Raw = gst_element_factory_make (native_element, native_name);
			Gst.GLib.Marshaller.Free (native_name);
			Gst.GLib.Marshaller.Free (native_element);
			if (Raw == IntPtr.Zero)
				throw new Exception ("Failed to instantiate element \"filesrc\"");
		}

		public FileSrc () : this ((string) null) { }

		public static FileSrc Make (string name) {
			return Gst.ElementFactory.Make ("filesrc", name) as FileSrc;
		}

		public static FileSrc Make () { return Make (null); } 

		[Gst.GLib.Property ("name")]
		public string Name {
			get {
				Gst.GLib.Value val = GetProperty ("name");
				string ret = (string) val.Val;
				val.Dispose ();
				return ret;
			}
			set {
				Gst.GLib.Value val = new Gst.GLib.Value (this, "name");
				val.Val = value;
				SetProperty ("name", val);
				val.Dispose ();
			}
		}

		[Gst.GLib.Property ("blocksize")]
		public ulong Blocksize {
			get {
				Gst.GLib.Value val = GetProperty ("blocksize");
				ulong ret = (ulong) val.Val;
				val.Dispose ();
				return ret;
			}
			set {
				Gst.GLib.Value val = new Gst.GLib.Value (this, "blocksize");
				val.Val = value;
				SetProperty ("blocksize", val);
				val.Dispose ();
			}
		}

		[Gst.GLib.Property ("num-buffers")]
		public int NumBuffers {
			get {
				Gst.GLib.Value val = GetProperty ("num-buffers");
				int ret = (int) val.Val;
				val.Dispose ();
				return ret;
			}
			set {
				Gst.GLib.Value val = new Gst.GLib.Value (this, "num-buffers");
				val.Val = value;
				SetProperty ("num-buffers", val);
				val.Dispose ();
			}
		}

		[Gst.GLib.Property ("typefind")]
		public bool Typefind {
			get {
				Gst.GLib.Value val = GetProperty ("typefind");
				bool ret = (bool) val.Val;
				val.Dispose ();
				return ret;
			}
			set {
				Gst.GLib.Value val = new Gst.GLib.Value (this, "typefind");
				val.Val = value;
				SetProperty ("typefind", val);
				val.Dispose ();
			}
		}

		[Gst.GLib.Property ("do-timestamp")]
		public bool DoTimestamp {
			get {
				Gst.GLib.Value val = GetProperty ("do-timestamp");
				bool ret = (bool) val.Val;
				val.Dispose ();
				return ret;
			}
			set {
				Gst.GLib.Value val = new Gst.GLib.Value (this, "do-timestamp");
				val.Val = value;
				SetProperty ("do-timestamp", val);
				val.Dispose ();
			}
		}

		[Gst.GLib.Property ("location")]
		public string Location {
			get {
				Gst.GLib.Value val = GetProperty ("location");
				string ret = (string) val.Val;
				val.Dispose ();
				return ret;
			}
			set {
				Gst.GLib.Value val = new Gst.GLib.Value (this, "location");
				val.Val = value;
				SetProperty ("location", val);
				val.Dispose ();
			}
		}

		[Gst.GLib.Property ("fd")]
		public int Fd {
			get {
				Gst.GLib.Value val = GetProperty ("fd");
				int ret = (int) val.Val;
				val.Dispose ();
				return ret;
			}
			set {
				Gst.GLib.Value val = new Gst.GLib.Value (this, "fd");
				val.Val = value;
				SetProperty ("fd", val);
				val.Dispose ();
			}
		}

		[Gst.GLib.Property ("mmapsize")]
		public ulong Mmapsize {
			get {
				Gst.GLib.Value val = GetProperty ("mmapsize");
				ulong ret = (ulong) val.Val;
				val.Dispose ();
				return ret;
			}
			set {
				Gst.GLib.Value val = new Gst.GLib.Value (this, "mmapsize");
				val.Val = value;
				SetProperty ("mmapsize", val);
				val.Dispose ();
			}
		}

		[Gst.GLib.Property ("sequential")]
		public bool Sequential {
			get {
				Gst.GLib.Value val = GetProperty ("sequential");
				bool ret = (bool) val.Val;
				val.Dispose ();
				return ret;
			}
			set {
				Gst.GLib.Value val = new Gst.GLib.Value (this, "sequential");
				val.Val = value;
				SetProperty ("sequential", val);
				val.Dispose ();
			}
		}

		[Gst.GLib.Property ("touch")]
		public bool Touch {
			get {
				Gst.GLib.Value val = GetProperty ("touch");
				bool ret = (bool) val.Val;
				val.Dispose ();
				return ret;
			}
			set {
				Gst.GLib.Value val = new Gst.GLib.Value (this, "touch");
				val.Val = value;
				SetProperty ("touch", val);
				val.Dispose ();
			}
		}

		[Gst.GLib.Property ("use-mmap")]
		public bool UseMmap {
			get {
				Gst.GLib.Value val = GetProperty ("use-mmap");
				bool ret = (bool) val.Val;
				val.Dispose ();
				return ret;
			}
			set {
				Gst.GLib.Value val = new Gst.GLib.Value (this, "use-mmap");
				val.Val = value;
				SetProperty ("use-mmap", val);
				val.Dispose ();
			}
		}


#endregion
	}

}