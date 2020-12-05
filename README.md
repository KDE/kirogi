# Kirogi

Kirogi is a ground control application for drones. Take to the skies, open source-style.

[![Flatpak](https://binary-factory.kde.org/buildStatus/icon?job=Kirogi_x86_64_flatpak&subject=Flatpak)](https://binary-factory.kde.org/view/Android/job/Kirogi_x86_64_flatpak/lastBuild)
[![Android](https://binary-factory.kde.org/buildStatus/icon?job=Kirogi_android&subject=Android)](https://binary-factory.kde.org/view/Android/job/Kirogi_android/lastBuild)

### Screenshot

![A screenshot of Kirogi's in-flight UI](https://kirogi.org/assets/img/kirogi.png "Kirogi's in-flight UI")

### Features

* Direct flight controls
* Fly by touch on a Navigation Map
* Trigger vehicle actions (e.g. flips, trim)
* Gamepad/joypad support
* Live video
* Configure flight parameters (speed, altitude limits)
* Support for Parrot (Anafi, Bebop 2) and Ryze Tello drones (more planned)

## FAQ

### How is Kirogi different from other ground control applications?

Created from scratch in February 2019, Kirogi benefits from a lot of prior art. It aims to provide the following unique combination of features:

* **End-user focus.** As a [KDE application](https://manifesto.kde.org/), Kirogi aims to be a polished experience that is fun and easy to use for novices and experts alike, with special emphasis on a premium direct control experience.

* **Multi-vendor.** Kirogi aims to have wide out-of-the-box support for many vehicles by different manufacturers. Users should be able to use different vehicles without needing to switch ground control software.

* **Highly portable.** Kirogi aims to support many different mobile and desktop platforms. Users should be able to switch platforms without needing to switch ground control software.

* **Freedom.** Kirogi will always be freely-licensed. Users should have control over and be able to extend and modify their ground control on their own.

* **Open governance.** Created and maintained by the [KDE community](https://manifesto.kde.org/), an organization with more than two decades of experience in hosting free software projects.

### Will Kirogi support my drone?

The Kirogi project is currently in its early stages. For now it supports the following drones:

* Parrot Anafi
* Parrot Bebop 2
* Ryze Tello

Additionally, support for the [MAVLink](https://en.wikipedia.org/wiki/MAVLink) protocol is in early development, enabling control of many drones natively speaking this protocol. To build with MAVLink support, the upstream library needs to be installed or the `MAVLINK_INCLUDE_DIR` environment variable defined.

Support for additional vehicles, also by other manufacturers, is very much a goal. If your vehicle is not on the list yet, check back later!

Vehicle support is subject to availability for development and testing. If you are a manufacturer and would like to see Kirogi support your hardware, please [contact the board of directors of KDE e.V.](https://ev.kde.org/contact.php) for this and any other business inquiries.

### Can Kirogi download photos and videos from my drone?

Not yet. This feature is considered high priority and will be added before version 1.0.

### Does Kirogi support mission planning and upload?

Not yet. A mission planning mode is a future goal.

### Do I need a touchscreen to use Kirogi?

A touchscreen is not required to use Kirogi. It has basic support for two-stick gamepad/joypad controllers (it was tested with an Xbox One controller).

### Does Kirogi support the Parrot Skycontroller peripherals?

Not yet. There is basic support for two-stick gamepad/joypad controllers, however.

### What does the name mean?

The Kirogi project was started in South Korea. Kirogi (written "기러기" in the native writing system) is the Korean word for wild geese.

_Wild_ geese are known as expert flyers. Kirogi's mascot is a farm goose with an adventurous spirit, who flies just as well thanks to the use of superior technology.

## Technology and License

### Does Kirogi support [MAVLink](https://en.wikipedia.org/wiki/MAVLink)?

Support for the [MAVLink](https://en.wikipedia.org/wiki/MAVLink) protocol is in early development, enabling control of many drones natively speaking this protocol. To build with MAVLink support, the upstream library needs to be installed or the `MAVLINK_INCLUDE_DIR` environment variable defined.

### What technology does Kirogi use?

Kirogi is written in C++14 and QML. It uses the [Kirigami](https://www.kde.org/products/kirigami/) UI toolkit (part of [KDE Frameworks](https://www.kde.org/products/frameworks/)) and Qt Quick, supported on a diverse array of mobile and desktop platforms.

### What is the license?

Kirogi's library in src/lib/ and bundled vehicle support plugins in src/plugins/ are licensed under LGPL v2.1 (or later), see COPYING.LIB.

The frontend (UI and other business logic) is licensed under GPL v2 (or later), see COPYING.

The app icon in data/icons/ is licensed under LGPL v2.1 (or later), see COPYING.LIB.

The mascot artwork in src/ui/assets/ is licensed under CC-BY-SA 4.0, see COPYING.MASCOT.

### I have a supported Parrot drone. Do I need the Parrot SDK to use Kirogi?

The Parrot SDK is not required to use Kirogi. It contains its own implementation of the protocol used to operate Parrot brand drones.
