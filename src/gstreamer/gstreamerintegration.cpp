/*
 * Copyright 2019 Eike Hein <hein@kde.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "gstreamerintegration.h"
#include "videosurface.h"

#include <QQuickItem>
#include <QQuickWindow>
#include <QRunnable>
#include <QStandardPaths>
#include <QDateTime>

#include <gst/gst.h>
#include <gst/gstsample.h>
#include <gst/gstcaps.h>


Q_LOGGING_CATEGORY(videoLogging, "kirogi.videosuppoert.gstreamerintegration")

GStreamerIntegration::GStreamerIntegration(QObject *parent)
    : QObject(parent)
    , m_playing(false)
    , m_gstPipeline(nullptr)
    , m_videoSink(nullptr)
    , m_fallback(true)
{
    updateGstPipeline();
}

GStreamerIntegration::~GStreamerIntegration()
{
    if (m_gstPipeline) {
        gst_element_set_state(m_gstPipeline, GST_STATE_NULL);
    }
}

bool GStreamerIntegration::playing() const
{
    return m_playing;
}

void GStreamerIntegration::setPlaying(bool playing)
{
    if (m_playing != playing) {
        m_playing = playing;

        if (!m_gstPipeline) {
            updateGstPipeline();
        } else if (m_gstPipeline->current_state == GST_STATE_PLAYING && !playing) {
            gst_element_set_state(m_gstPipeline, GST_STATE_PAUSED);
        } else if (m_gstPipeline->current_state != GST_STATE_PLAYING && playing) {
            updateGstPipeline();
        }

        Q_EMIT playingChanged();
    }
}

QString GStreamerIntegration::stringPipeline() const
{
    return m_stringPipeline;
}

void GStreamerIntegration::setStringPipeline(const QString &pipeline)
{
    if (m_stringPipeline != pipeline) {
        m_stringPipeline = pipeline;

        updateGstPipeline();

        Q_EMIT stringPipelineChanged();
    }
}

void GStreamerIntegration::updateGstPipeline()
{
    QString pipeline = m_stringPipeline;

    if (pipeline.isEmpty()) {
        pipeline = QLatin1String("videotestsrc pattern=snow ! video/x-raw,width=800,height=450 !");
    }
    pipeline += QStringLiteral("glupload ! glcolorconvert ! qmlglsink name=sink");

    if (m_gstPipeline) {
        gst_element_set_state(m_gstPipeline, GST_STATE_NULL);
        gst_object_unref(m_gstPipeline);
    }

    // If we fail to build the pipeline, we also fail to load the QML gst plugin
    // and the entire application will crash after that.
    GError *error = nullptr;
    m_gstPipeline = gst_parse_launch(pipeline.toLatin1().data(), &error);
    Q_ASSERT_X(m_gstPipeline, "gstreamer pipeline", QStringLiteral("%0 with pieline: %1").arg(error->message).arg(pipeline).toStdString().c_str());

    m_videoSink = gst_bin_get_by_name(GST_BIN(m_gstPipeline), "sink");
    Q_ASSERT_X(m_videoSink, "gstreamer video sink", "Could not retrieve the video sink.");

}

void GStreamerIntegration::takeSnapshot()
{
    if (!m_videoSink) {
        qCDebug(videoLogging) << "No video, exiting";
        return;
    }

    GstSample *videobuffer = nullptr;
    g_object_get(G_OBJECT(m_videoSink), "last-sample", &videobuffer, nullptr);
    if (!videobuffer) {
        qCDebug(videoLogging) << "No video buffer, exiting";
        return;
    }

    GstCaps *capsStructure = gst_sample_get_caps(videobuffer);
    if (!capsStructure) {
        qCDebug(videoLogging) << "Can't get caps from video buffer, exiting";
        return;
    }

    GstStructure *s = gst_caps_get_structure(capsStructure, 0);
    gint width, height;

    gboolean res = gst_structure_get_int (s, "width", &width)
                && gst_structure_get_int (s, "height", &height);
    if (!res) {
        qCDebug(videoLogging) << "Can't get width or height from caps";
        return;
    }

    GstBuffer *snapbuffer = gst_sample_get_buffer(videobuffer);
    if (!snapbuffer) {
        qCDebug(videoLogging) << "Can't get the sample buffer from the video";
        return;
    }

    GstMapInfo map;
    gst_buffer_map (snapbuffer, &map, GST_MAP_READ);

    uchar* bufferData = reinterpret_cast<uchar*>(map.data);
    QImage::Format imageFormat = QImage::Format_RGB32;
    QImage image(bufferData, width, height, imageFormat);

    QString savePath; // TODO: Add a setting on KConfigXT
    if(savePath.isEmpty()) {
        savePath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
    }
    savePath += "/" + QDateTime::currentDateTime().toString("yyyy-MM-dd_hh.mm.ss") + ".png";

    if (image.save(savePath)) {
        qCDebug(videoLogging) << "Image saved in" << savePath;
    } else {
        qCDebug(videoLogging) << "Error saving image in" << savePath << "please verify your access rights";
    }
}

void GStreamerIntegration::init()
{
    // GStreamer needs the sink to be created before any Qml elements
    // so that the Qml elements are registered in the system. so we create
    // and free it.
    GstElement *sink = gst_element_factory_make ("qmlglsink", nullptr);
    Q_ASSERT_X(sink, "Video Initialization", "Could not find the Qml Gl Sink GStreamer Plugin, please check your installation.");

    gst_object_unref(sink);

    qmlRegisterType<GStreamerIntegration> ("org.kde.kirogi", 1, 0, "VideoReceiver");
    qmlRegisterType<VideoSurface> ("org.kde.kirogi", 1, 0, "VideoSurface");
}

