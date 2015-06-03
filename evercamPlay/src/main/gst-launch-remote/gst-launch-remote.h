/* GStreamer
 *
 * Copyright (C) 2014 Sebastian Dröge <sebastian@centricular.com>
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
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef __GST_LAUNCH_REMOTE_H__
#define __GST_LAUNCH_REMOTE_H__

#include <gio/gio.h>
#include <gst/gst.h>
#include <gst/video/video.h>

#define PORT 9123

#ifdef __ANDROID__
#include <android/log.h>

#undef GST_DEBUG
#define GST_DEBUG(...) __android_log_print(ANDROID_LOG_INFO, "gstreamer-debug", __VA_ARGS__);
#endif

typedef struct {
  gpointer app;
  void (*set_error) (const gchar *message, gint code, gpointer app);
  void (*set_message) (const gchar *message, gpointer app);
  void (*set_current_position) (gint position, gint duration, gpointer app);
  void (*initialized) (gpointer app);
  void (*media_size_changed) (gint width, gint height, gpointer app);
  void (*on_video_loaded) (gpointer app);
} GstLaunchRemoteAppContext;

typedef struct {
  GThread *thread;
  GMainContext *context;
  GMainLoop *main_loop;

  guintptr window_handle;

  gboolean initialized;

  gchar *pipeline_string;
  gchar *username;
  gchar *password;
  GstElement *pipeline;
  GstElement *video_sink;
  GstState target_state;
  gboolean is_live;
  gchar *last_message;
  guint64 tcp_timeout;

  GstClock *net_clock;
  GstClockTime base_time;

  GSocketService *service;
  GSocketConnection *connection;
  GDataInputStream *distream;
  GOutputStream *ostream;
  GSocket *debug_socket;

  GstLaunchRemoteAppContext app_context;
} GstLaunchRemote;

/* Set callbacks manually as required */
GstLaunchRemote * gst_launch_remote_new               (const GstLaunchRemoteAppContext *ctx);
void              gst_launch_remote_free              (GstLaunchRemote * self);
void              gst_launch_remote_play              (GstLaunchRemote * self);
void              gst_launch_remote_pause             (GstLaunchRemote * self);
void              gst_launch_remote_stop              (GstLaunchRemote * self);
void              gst_launch_remote_set_window_handle (GstLaunchRemote * self, guintptr handle);
void              gst_launch_remote_call_set_pipeline (GstLaunchRemote * self, const gchar * pipeline_string);
// src stuff
void              gst_launch_remote_set_username (GstLaunchRemote * self, const gchar * username);
void              gst_launch_remote_set_password (GstLaunchRemote * self, const gchar * password);

#endif
