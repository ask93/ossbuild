/*
 * GStreamer
 * Copyright (C) 2008 Filippo Argiolas <filippo.argiolas@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/**
 * SECTION:element-glfilterblur
 *
 * Blur with 9x9 separable convolution.
 *
 * <refsect2>
 * <title>Examples</title>
 * |[
 * gst-launch videotestsrc ! glupload ! glfilterblur ! glimagesink
 * ]|
 * FBO (Frame Buffer Object) and GLSL (OpenGL Shading Language) are required.
 * </refsect2>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "gstglfilterblur.h"
#include <gstgleffectssources.h>

#define GST_CAT_DEFAULT gst_gl_filterblur_debug
GST_DEBUG_CATEGORY_STATIC (GST_CAT_DEFAULT);

#define DEBUG_INIT(bla)							\
  GST_DEBUG_CATEGORY_INIT (gst_gl_filterblur_debug, "glfilterblur", 0, "glfilterblur element");

GST_BOILERPLATE_FULL (GstGLFilterBlur, gst_gl_filterblur, GstGLFilter,
    GST_TYPE_GL_FILTER, DEBUG_INIT);

static void gst_gl_filterblur_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec);
static void gst_gl_filterblur_get_property (GObject * object, guint prop_id,
    GValue * value, GParamSpec * pspec);
static void gst_gl_filter_filterblur_reset (GstGLFilter * filter);

static void gst_gl_filterblur_init_shader (GstGLFilter * filter);
static gboolean gst_gl_filterblur_filter (GstGLFilter * filter,
    GstGLBuffer * inbuf, GstGLBuffer * outbuf);
static void gst_gl_filterblur_hcallback (gint width, gint height, guint texture,
    gpointer stuff);
static void gst_gl_filterblur_vcallback (gint width, gint height, guint texture,
    gpointer stuff);


static void
gst_gl_filterblur_init_resources (GstGLFilter * filter)
{
  GstGLFilterBlur *filterblur = GST_GL_FILTERBLUR (filter);

  glGenTextures (1, &filterblur->midtexture);
  glBindTexture (GL_TEXTURE_RECTANGLE_ARB, filterblur->midtexture);
  glTexImage2D (GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGBA8,
      filter->width, filter->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri (GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri (GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri (GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S,
      GL_CLAMP_TO_EDGE);
  glTexParameteri (GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T,
      GL_CLAMP_TO_EDGE);
}

static void
gst_gl_filterblur_reset_resources (GstGLFilter * filter)
{
  GstGLFilterBlur *filterblur = GST_GL_FILTERBLUR (filter);

  glDeleteTextures (1, &filterblur->midtexture);
}

static void
gst_gl_filterblur_base_init (gpointer klass)
{
  GstElementClass *element_class = GST_ELEMENT_CLASS (klass);

  gst_element_class_set_details_simple (element_class, "Gstreamer OpenGL Blur",
      "Filter/Effect", "Blur with 9x9 separable convolution",
      "Filippo Argiolas <filippo.argiolas@gmail.com>");
}

static void
gst_gl_filterblur_class_init (GstGLFilterBlurClass * klass)
{
  GObjectClass *gobject_class;

  gobject_class = (GObjectClass *) klass;
  gobject_class->set_property = gst_gl_filterblur_set_property;
  gobject_class->get_property = gst_gl_filterblur_get_property;

  GST_GL_FILTER_CLASS (klass)->filter = gst_gl_filterblur_filter;
  GST_GL_FILTER_CLASS (klass)->display_init_cb =
      gst_gl_filterblur_init_resources;
  GST_GL_FILTER_CLASS (klass)->display_reset_cb =
      gst_gl_filterblur_reset_resources;
  GST_GL_FILTER_CLASS (klass)->onInitFBO = gst_gl_filterblur_init_shader;
  GST_GL_FILTER_CLASS (klass)->onReset = gst_gl_filter_filterblur_reset;
}

static void
gst_gl_filterblur_init (GstGLFilterBlur * filterblur,
    GstGLFilterBlurClass * klass)
{
  filterblur->shader0 = NULL;
  filterblur->shader1 = NULL;
  filterblur->midtexture = 0;
  /* gaussian kernel (well, actually vector), size 9, standard
   * deviation 3.0 */
  /* FIXME: eventually make this a runtime property */
  fill_gaussian_kernel (filterblur->gauss_kernel, 7, 3.0);
}

static void
gst_gl_filter_filterblur_reset (GstGLFilter * filter)
{
  GstGLFilterBlur *filterblur = GST_GL_FILTERBLUR (filter);

  //blocking call, wait the opengl thread has destroyed the shader
  gst_gl_display_del_shader (filter->display, filterblur->shader0);

  //blocking call, wait the opengl thread has destroyed the shader
  gst_gl_display_del_shader (filter->display, filterblur->shader1);
}

static void
gst_gl_filterblur_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec)
{
  /* GstGLFilterBlur *filterblur = GST_GL_FILTERBLUR (object); */

  switch (prop_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static void
gst_gl_filterblur_get_property (GObject * object, guint prop_id,
    GValue * value, GParamSpec * pspec)
{
  /* GstGLFilterBlur *filterblur = GST_GL_FILTERBLUR (object); */

  switch (prop_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static void
gst_gl_filterblur_init_shader (GstGLFilter * filter)
{
  GstGLFilterBlur *blur_filter = GST_GL_FILTERBLUR (filter);

  //blocking call, wait the opengl thread has compiled the shader
  gst_gl_display_gen_shader (filter->display, 0, hconv7_fragment_source,
      &blur_filter->shader0);

  //blocking call, wait the opengl thread has compiled the shader
  gst_gl_display_gen_shader (filter->display, 0, vconv7_fragment_source,
      &blur_filter->shader1);
}

static gboolean
gst_gl_filterblur_filter (GstGLFilter * filter, GstGLBuffer * inbuf,
    GstGLBuffer * outbuf)
{
  GstGLFilterBlur *filterblur = GST_GL_FILTERBLUR (filter);

  gst_gl_filter_render_to_target (filter, inbuf->texture,
      filterblur->midtexture, gst_gl_filterblur_hcallback, filterblur);

  gst_gl_filter_render_to_target (filter, filterblur->midtexture,
      outbuf->texture, gst_gl_filterblur_vcallback, filterblur);

  return TRUE;
}

static void
gst_gl_filterblur_hcallback (gint width, gint height, guint texture,
    gpointer stuff)
{
  GstGLFilter *filter = GST_GL_FILTER (stuff);
  GstGLFilterBlur *filterblur = GST_GL_FILTERBLUR (filter);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();

  gst_gl_shader_use (filterblur->shader0);

  glActiveTexture (GL_TEXTURE1);
  glEnable (GL_TEXTURE_RECTANGLE_ARB);
  glBindTexture (GL_TEXTURE_RECTANGLE_ARB, texture);
  glDisable (GL_TEXTURE_RECTANGLE_ARB);

  gst_gl_shader_set_uniform_1i (filterblur->shader0, "tex", 1);
  gst_gl_shader_set_uniform_1fv (filterblur->shader0, "kernel", 7,
      filterblur->gauss_kernel);

  gst_gl_filter_draw_texture (filter, texture);
}


static void
gst_gl_filterblur_vcallback (gint width, gint height, guint texture,
    gpointer stuff)
{
  GstGLFilter *filter = GST_GL_FILTER (stuff);
  GstGLFilterBlur *filterblur = GST_GL_FILTERBLUR (filter);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();

  gst_gl_shader_use (filterblur->shader1);

  glActiveTexture (GL_TEXTURE1);
  glEnable (GL_TEXTURE_RECTANGLE_ARB);
  glBindTexture (GL_TEXTURE_RECTANGLE_ARB, texture);
  glDisable (GL_TEXTURE_RECTANGLE_ARB);

  gst_gl_shader_set_uniform_1i (filterblur->shader1, "tex", 1);
  gst_gl_shader_set_uniform_1fv (filterblur->shader1, "kernel", 7,
      filterblur->gauss_kernel);

  gst_gl_filter_draw_texture (filter, texture);
}
