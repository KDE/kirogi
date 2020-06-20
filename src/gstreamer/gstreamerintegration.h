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

#pragma once

#include <QLoggingCategory>
#include <QObject>

class QQuickWindow;

struct _GstElement;
struct _GstMessage;
typedef struct _GstElement GstElement;
typedef struct _GstMessage GstMessage;

class GStreamerIntegration : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool playing READ playing WRITE setPlaying NOTIFY playingChanged)
    Q_PROPERTY(QString stringPipeline READ stringPipeline WRITE setStringPipeline NOTIFY stringPipelineChanged)

public:
    explicit GStreamerIntegration(QObject *parent = nullptr);
    ~GStreamerIntegration();

    bool playing() const;
    void setPlaying(bool playing);

    QString stringPipeline() const;
    void setStringPipeline(const QString &pipeline);
    static void init();

    GstElement *videoSink() const
    {
        return m_videoSink;
    }
    GstElement *pipeline() const
    {
        return m_gstPipeline;
    }

    void takeSnapshot();
    static void processPipelineMessage(GstMessage *message);

Q_SIGNALS:
    void playingChanged();
    void stringPipelineChanged();

private:
    void updateGstPipeline();

    bool m_playing;

    QString m_stringPipeline;
    GstElement *m_gstPipeline;
    GstElement *m_videoSink;

    bool m_fallback;
};

Q_DECLARE_LOGGING_CATEGORY(videoLogging)
