include(FindPackageHandleStandardArgs)

set(MAVLINK_SEARCH_PATHS
  /usr
  /usr/local
  /opt/local
)

find_path(MAVLINK_INCLUDE_DIR
    NAMES mavlink_types.h
    PATHS
      ${MAVLINK_SEARCH_PATHS}
    PATH_SUFFIXES
      include
      include/mavlink
      include/mavlink/v1.0
)

find_package_handle_standard_args(MAVLink DEFAULT_MSG MAVLINK_INCLUDE_DIR)

mark_as_advanced(MAVLINK_INCLUDE_DIR)
