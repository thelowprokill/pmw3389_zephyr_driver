zephyr_include_directories(include/zephyr/drivers/sensor)
add_subdirectory_ifdef(CONFIG_PMW3389 drivers/sensor/pixart/pmw3389)
