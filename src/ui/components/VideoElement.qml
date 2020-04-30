import QtQuick 2.12
import org.kde.kirogi.video 0.1 as KirogiVideo

Item {
    Image {
        source: "fallback.jpg"
        smooth: true
        visible: !videoSurface.playing
        fillMode: Image.PreserveAspectCrop
        anchors.fill: parent
    }

    Connections {
        target: kirogi

        onCurrentVehicleChanged: {
            videoSurface.playing = kirogi.currentVehicle != null;
        }

        onCurrentPageChanged: {
            videoSurface.playing = (kirogi.currentPage == page || kirogi.currentVehicle);
        }
    }

    KirogiVideo.VideoReceiver {
        id: videoReceiver
        stringPipeline: kirogi.currentVehicle ? kirogi.currentVehicle.videoSource : ""
    }

    KirogiVideo.VideoSurface {
        id: videoSurface
        videoReceiver: videoReceiver

        width: parent.width
        height: kirogiSettings.stretchVideo ? parent.height : parent.width / 1.77
    }
}