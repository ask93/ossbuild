// This file was generated by the Gtk# code generator.
// Any changes made will be lost if regenerated.

namespace GstSharp {

	using System;
	using System.Runtime.InteropServices;

#region Autogenerated code
	[UnmanagedFunctionPointer (CallingConvention.Cdecl)]
	internal delegate bool PadBufferProbeCallbackNative(IntPtr pad, IntPtr buffer, IntPtr user_data);

	internal class PadBufferProbeCallbackInvoker {

		PadBufferProbeCallbackNative native_cb;
		IntPtr __data;
		Gst.GLib.DestroyNotify __notify;

		~PadBufferProbeCallbackInvoker ()
		{
			if (__notify == null)
				return;
			__notify (__data);
		}

		internal PadBufferProbeCallbackInvoker (PadBufferProbeCallbackNative native_cb) : this (native_cb, IntPtr.Zero, null) {}

		internal PadBufferProbeCallbackInvoker (PadBufferProbeCallbackNative native_cb, IntPtr data) : this (native_cb, data, null) {}

		internal PadBufferProbeCallbackInvoker (PadBufferProbeCallbackNative native_cb, IntPtr data, Gst.GLib.DestroyNotify notify)
		{
			this.native_cb = native_cb;
			__data = data;
			__notify = notify;
		}

		internal Gst.PadBufferProbeCallback Handler {
			get {
				return new Gst.PadBufferProbeCallback(InvokeNative);
			}
		}

		bool InvokeNative (Gst.Pad pad, Gst.Buffer buffer)
		{
			bool result = native_cb (pad == null ? IntPtr.Zero : pad.Handle, buffer == null ? IntPtr.Zero : buffer.Handle, __data);
			return result;
		}
	}

	internal class PadBufferProbeCallbackWrapper {

		public bool NativeCallback (IntPtr pad, IntPtr buffer, IntPtr user_data)
		{
			try {
				bool __ret = managed (Gst.GLib.Object.GetObject(pad) as Gst.Pad, Gst.MiniObject.GetObject(buffer) as Gst.Buffer);
				if (release_on_call)
					gch.Free ();
				return __ret;
			} catch (Exception e) {
				Gst.GLib.ExceptionManager.RaiseUnhandledException (e, false);
				return false;
			}
		}

		bool release_on_call = false;
		GCHandle gch;

		public void PersistUntilCalled ()
		{
			release_on_call = true;
			gch = GCHandle.Alloc (this);
		}

		internal PadBufferProbeCallbackNative NativeDelegate;
		Gst.PadBufferProbeCallback managed;

		public PadBufferProbeCallbackWrapper (Gst.PadBufferProbeCallback managed)
		{
			this.managed = managed;
			if (managed != null)
				NativeDelegate = new PadBufferProbeCallbackNative (NativeCallback);
		}

		public static Gst.PadBufferProbeCallback GetManagedDelegate (PadBufferProbeCallbackNative native)
		{
			if (native == null)
				return null;
			PadBufferProbeCallbackWrapper wrapper = (PadBufferProbeCallbackWrapper) native.Target;
			if (wrapper == null)
				return null;
			return wrapper.managed;
		}
	}
#endregion
}