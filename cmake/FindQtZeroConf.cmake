#
# Copyright 2019 Eike Hein <hein@kde.org>
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) version 3, or any
# later version accepted by the membership of KDE e.V. (or its
# successor approved by the membership of KDE e.V.), which shall
# act as a proxy defined in Section 6 of version 3 of the license.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library.  If not, see <http://www.gnu.org/licenses/>.

if (QTZEROCONF_INCLUDE_DIR AND QTZEROCONF_LIBRARIES)
    set (QTZEROCONF_FOUND TRUE)
else (QTZEROCONF_INCLUDE_DIR AND QTZEROCONF_LIBRARIES)
    find_library(QTZEROCONF_LIBRARIES NAMES QtZeroConf)
    find_path(QTZEROCONF_INCLUDE_DIR NAMES QtZeroConf/qzeroconf.h)

    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(QtZeroConf DEFAULT_MSG QTZEROCONF_LIBRARIES QTZEROCONF_INCLUDE_DIR)
endif (QTZEROCONF_INCLUDE_DIR AND QTZEROCONF_LIBRARIES)

mark_as_advanced(QTZEROCONF_LIBRARIES QTZEROCONF_INCLUDE_DIR)
