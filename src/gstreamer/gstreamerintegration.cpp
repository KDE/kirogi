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

#include <QQuickItem>
#include <QQuickWindow>
#include <QRunnable>

class SetPlaying : public QRunnable
{
public:
    SetPlaying(GstElement *);
    ~SetPlaying();

    void run() override;

private:
    GstElement *m_pipeline;
};

SetPlaying::SetPlaying(GstElement *pipeline)
{
    m_pipeline = pipeline ? static_cast<GstElement *>(gst_object_ref(pipeline)) : NULL;
}

SetPlaying::~SetPlaying()
{
    if (m_pipeline) {
        gst_object_unref(m_pipeline);
    }
}

void SetPlaying::run()
{
    if (m_pipeline) {
        gst_element_set_state(m_pipeline, GST_STATE_PLAYING);
    }
}

GStreamerIntegration::GStreamerIntegration(QObject *parent)
    : QObject(parent)
    , m_playing(false)
    , m_gstPipeline(nullptr)
    , m_fallback(true)
    , m_window(nullptr)
{
    updateGstPipeline();
}

GStreamerIntegration::~GStreamerIntegration()
{
    if (m_gstPipeline) {
        gst_element_set_state(m_gstPipeline, GST_STATE_NULL);
        gst_object_unref(m_gstPipeline);
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

        emit playingChanged();
    }
}

QString GStreamerIntegration::pipeline() const
{
    return m_pipeline;
}

void GStreamerIntegration::setPipeline(const QString &pipeline)
{
    if (m_pipeline != pipeline) {
        m_pipeline = pipeline;

        updateGstPipeline();

        emit pipelineChanged();
    }
}

void GStreamerIntegration::setWindow(QQuickWindow *window)
{
    m_window = window;
    updateGstPipeline();
}

void GStreamerIntegration::updateGstPipeline()
{
    QString pipeline = m_pipeline;

    if (pipeline.isEmpty()) {
        pipeline = QLatin1String("videotestsrc pattern=snow ! video/x-raw,width=800,height=450 ! glupload ! qmlglsink name=sink");
    }

    if (m_gstPipeline) {
        gst_element_set_state(m_gstPipeline, GST_STATE_NULL);
        gst_object_unref(m_gstPipeline);
    }

    // If we fail to build the pipeline, we also fail to load the qml gst plugin
    // and the entire application will crash after that
    GError *error = nullptr;
    m_gstPipeline = gst_parse_launch(pipeline.toLatin1().data(), &error);
    Q_ASSERT_X(m_gstPipeline, "gstreamer pipeline", QStringLiteral("%0 with pieline: %1").arg(error->message).arg(pipeline).toStdString().c_str());

    GstElement *sink = gst_bin_get_by_name(GST_BIN(m_gstPipeline), "sink");

    if (m_window) {
        auto videoOutput = m_window->findChild<QQuickItem *>("videoOutput");
        if (videoOutput) {
            g_object_set(sink, "widget", videoOutput, NULL);

            if (m_playing) {
                m_window->scheduleRenderJob(new SetPlaying(m_gstPipeline), QQuickWindow::BeforeSynchronizingStage);
            }
        }
    }

    gst_object_unref(sink);
}
