/*
 * Copyright 2020 Tomaz Cananbrava <tcanabrava@kde.org>
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

#pragma once

#include <QQuickItem>

class GStreamerIntegration;

/* This is a fake surface that controls the video.
 *
 * Because of the way that OpenGL works, and because how GStreamer
 * is created, we need to play / pause the pipeline in the rendering
 * thread, that means that this element is the actual responsible
 * of starting the video. Other classes can request the video to start,
 * but never directly.
 *
 * The 'VideoItem' element is a GstGlVideoItem, that's the real paint surface.
 * This class plug things together and controls playing / pausing / screenshooting, etc.
 */
class VideoSurface : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(GStreamerIntegration *videoReceiver WRITE setVideoReceiver READ videoReceiver NOTIFY videoReceiverChanged)
    Q_PROPERTY(bool playing WRITE setPlaying READ playing NOTIFY playingChanged)

public:
    VideoSurface(QQuickItem *parent = nullptr);
    virtual ~VideoSurface();

    /* This is the Video Receiver, that controls the creation of the pipeline and a few other helper functions */
    GStreamerIntegration *videoReceiver() const;
    Q_SLOT void setVideoReceiver(GStreamerIntegration *videoReceiver);
    Q_SIGNAL void videoReceiverChanged(GStreamerIntegration *videoReceiver);

    bool playing() const;
    Q_SLOT void setPlaying(bool value);
    Q_SIGNAL void playingChanged(bool value);

    /* This method sets the pipeline state to playing as soon as Qt allows it */
    Q_INVOKABLE void startVideo();
    Q_INVOKABLE void pauseVideo();

    /* update paint node usually is a method that should trigger a painting on the OpenGL surface
    in our case, it's where we set the gstreamer pipeline state to playing / stop / pause.
    */
    QSGNode *updatePaintNode(QSGNode *node, UpdatePaintNodeData *) override;

private:
    /**
     * @brief Create GST Video widget
     *
     */
    void createVideoItem();

    QQuickItem *_videoItem;
    GStreamerIntegration *_videoReceiver;
    bool _shouldStartVideo;
    bool _shouldPauseVideo;
    bool _playing;
};
