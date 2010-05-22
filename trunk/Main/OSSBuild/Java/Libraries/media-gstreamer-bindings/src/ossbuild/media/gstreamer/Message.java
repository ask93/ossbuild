
package ossbuild.media.gstreamer;

import com.sun.jna.Pointer;
import static ossbuild.media.gstreamer.api.GObject.*;
import static ossbuild.media.gstreamer.api.GStreamer.*;

/**
 *
 * @author David Hoyt <dhoyt@hoytsoft.org>
 */
public class Message extends BaseGObject {
	//<editor-fold defaultstate="collapsed" desc="Variables">
	private MessageType msgType;
	private ossbuild.media.gstreamer.api.GstMessage msg;
	//</editor-fold>

	//<editor-fold defaultstate="collapsed" desc="Initialization">
	Message(Pointer ptr) {
		this.ptr = ptr;
		this.msg = new ossbuild.media.gstreamer.api.GstMessage(ptr);
		this.msgType = MessageType.fromNative(msg.type);
		this.managed = false;
	}

	static Message from(Pointer ptr) {
		return new Message(ptr);
	}
	//</editor-fold>

	//<editor-fold defaultstate="collapsed" desc="Getters">
	public Pointer getSource() {
		return msg.src;
	}

	public MessageType getMessageType() {
		return msgType;
	}
	//</editor-fold>

	//<editor-fold defaultstate="collapsed" desc="ToString">
	@Override
	public String toString() {
		return msgType.name();
	}
	//</editor-fold>

	//<editor-fold defaultstate="collapsed" desc="Dispose">
	@Override
	protected void disposeObject() {
		//TODO: This should be unreffed! But gst_message_unref() has been
		//inlined out of the lib. Calling gst_mini_object_unref() results in
		//a fatal crash. Our options are to find a way to cast it, ensure
		//the function isn't inlined, or live w/ the memory leak.
		//unref();
	}
	//</editor-fold>

	//<editor-fold defaultstate="collapsed" desc="Ref Counting">
	@Override
	public void ref() {
		if (ptr != Pointer.NULL)
			gst_mini_object_ref(ptr);
	}
	
	@Override
	public void unref() {
		if (ptr != Pointer.NULL)
			gst_mini_object_unref(ptr);
	}
	//</editor-fold>
}
