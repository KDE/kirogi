/*
 * Copyright 2020 Tomaz Canabrava <tcanabrava@kde.org>
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

#include "videosurface.h"
#include "gstreamerintegration.h"

#include <gst/gst.h>

Q_LOGGING_CATEGORY(surfaceLogging, "kirogi.videosuppoert.videosurface")

VideoSurface::VideoSurface(QQuickItem *parent)
    : QQuickItem(parent)
    , _videoItem(nullptr)
    , _videoReceiver(nullptr)
    , _shouldStartVideo(false)
    , _shouldPauseVideo(false)
{
    Q_INIT_RESOURCE(gstreamer);

    // This flag is needed so the item will call updatePaintNode.
    setFlag(ItemHasContents);
}

VideoSurface::~VideoSurface()
{
    if (_videoReceiver) {
        auto *sink = _videoReceiver->videoSink();
        auto *pipeline = _videoReceiver->pipeline();

        if (pipeline) {
            gst_element_set_state(pipeline, GST_STATE_NULL);
        }
        if (sink) {
            g_object_set(sink, "widget", nullptr, nullptr);
        }
    }
}

void VideoSurface::pauseVideo()
{
    _shouldPauseVideo = true;
    update();
}

void VideoSurface::startVideo()
{
    createVideoItem();

    if (!_videoItem || !_videoReceiver) {
        qDebug() << "Can't start the video yet";
        return;
    }

    auto *pipeline = _videoReceiver->pipeline();
    if (!pipeline) {
        qCDebug(surfaceLogging) << "It appears that we don't have a pipeline yet. Probably a misconfiguration in the vehicle plugins.";
        return;
    }

    auto *sink = _videoReceiver->videoSink();
    if (!sink) {
        qCDebug(surfaceLogging) << "Could not retrieve the video sink, can't start the video.";
        return;
    }

    /* the qtqmlsink needs a property named 'Widget' to be set,
     * then it will send data to that widget directly without our intervention
     * this widget is the GstGlVideoItem we need to create in Qml
     */
    GObject *videoSinkHasWidget = nullptr;
    g_object_get(sink, "widget", videoSinkHasWidget, nullptr);
    if (!videoSinkHasWidget) {
        g_object_set(sink, "widget", _videoItem, nullptr);
    }

    _shouldStartVideo = true;
    update();
}

void VideoSurface::setVideoReceiver(GStreamerIntegration *videoReceiver)
{
    if (!videoReceiver) {
        return;
    }

    if (_videoReceiver != videoReceiver) {
        _videoReceiver = videoReceiver;
        Q_EMIT videoReceiverChanged(_videoReceiver);
    }
    startVideo();
}

GStreamerIntegration *VideoSurface::videoReceiver() const
{
    return _videoReceiver;
}

QSGNode *VideoSurface::updatePaintNode(QSGNode *node, UpdatePaintNodeData *data)
{
    Q_UNUSED(data)
    if (!_shouldStartVideo && !_shouldPauseVideo) {
        return node;
    }

    Q_ASSERT_X(_videoReceiver, "video receiver", "No video receiver, and trying to start video.");
    auto *pipeline = _videoReceiver->pipeline();
    Q_ASSERT_X(pipeline, "gstreamer pipeline", "No pipeline, and trying to start video");

    // Do not try to play an already playing pipeline, pause it first.
    if (_shouldStartVideo) {
        qCInfo(surfaceLogging) << "Setting the state to PLAYING";
        gst_element_set_state(pipeline, GST_STATE_PLAYING);
    } else if (_shouldPauseVideo) {
        qCInfo(surfaceLogging) << "Setting the state to PAUSED";
        gst_element_set_state(pipeline, GST_STATE_PAUSED);
    }
    _shouldStartVideo = false;
    _shouldPauseVideo = false;
    return node;
}

bool VideoSurface::playing() const
{
    return _playing;
}

void VideoSurface::setPlaying(bool value)
{
    if (_playing == value) {
        return;
    }
    _playing = value;
    if (_playing) {
        startVideo();
    } else {
        pauseVideo();
    }
    Q_EMIT playingChanged(_playing);
}

void VideoSurface::createVideoItem()
{
    // Video widget already exist, abort
    if (_videoItem) {
        return;
    }

    QQmlEngine *engine = qmlEngine(this);
    if (!engine) {
        qCWarning(surfaceLogging) << "No qml engine to load visualization.";
        return;
    }

    // Create GST video widget
    QQmlComponent component(engine, QStringLiteral("qrc:/video/GstVideo.qml"), this);
    _videoItem = qobject_cast<QQuickItem *>(component.create());
    if (!_videoItem) {
        qCCritical(surfaceLogging) << "Failed to load QML component.";
        qCDebug(surfaceLogging) << "Component status:" << component.status();
        if (component.isError()) {
            qCDebug(surfaceLogging) << "Error list:" << component.errors();
        }
        return;
    }

    // Set this item as parent to user anchors
    _videoItem->setParentItem(this);
    qvariant_cast<QObject *>(_videoItem->property("anchors"))->setProperty("fill", QVariant::fromValue<VideoSurface *>(this));
}
