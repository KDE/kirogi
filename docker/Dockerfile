FROM ubuntu:20.04

# Set correct environment variables
ENV HOME /root
ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update && apt-get install --no-install-recommends -y \
	xvfb \
	x11vnc \
	supervisor \
	fluxbox \
	net-tools \
    novnc \
    net-tools \
    ca-certificates \
    && apt-get clean && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

RUN apt-get update && apt-get install --no-install-recommends -y \
    build-essential \
    cmake extra-cmake-modules \
    make \
    ninja-build \
    g++ \
    clang \
    clazy \
    ccache \
    pkg-config \
    appstream \
    gettext \
    gstreamer1.0-plugins-base libgstreamer1.0-0 libgstreamer-plugins-base1.0-0 gstreamer1.0-qt5 gstreamer1.0-gl \
    libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev \
    libqt5core5a libqt5network5 libqt5positioning5 \
    qtbase5-private-dev libqt5x11extras5-dev libqt5webchannel5-dev qtwebengine5-dev qttools5-dev qt5-image-formats-plugins \
    libqt5gamepad5-dev libqt5gamepad5 \
    qml-module-qtpositioning qml-module-org-kde-kirigami2 qml-module-qtlocation \
    qml-module-qtquick-controls2 qml-module-qtquick-controls qml-module-qtquick2 qml-module-org-kde-kquickcontrolsaddons qml-module-org-kde-games-core qml-module-qtquick-shapes \
    libkf5i18n5 libkf5configgui5 libkf5kirigami2-5 libkf5crash5 libkf5dnssd5 \
    kirigami2-dev libkf5crash-dev libkf5dnssd-dev \
    libkf5i18n-dev libkf5configwidgets-dev \
    xterm \
    && apt-get clean && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

# Docker's supervisor
COPY supervisord.conf /etc/supervisor/conf.d/supervisord.conf

# Set display
ENV DISPLAY :1
ENV LC_ALL C.UTF-8
ENV LANG en_US.UTF-8
ENV LANGUAGE en_US.UTF-8
ENV SCREEN_RESOLUTION 1024x768

# Expose Port (Note: if you change it do it as well in surpervisord.conf)
EXPOSE 8083

CMD ["/usr/bin/supervisord"]
