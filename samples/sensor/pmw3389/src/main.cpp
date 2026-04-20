#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <pmw3389.h>

int main(){
    printk("AS5x47 sample application\n");

    const device *my_sensor = DEVICE_DT_GET(DT_NODELABEL(my_sensor));

    if (!device_is_ready(my_sensor)) {
		printk("my_sensor: device not ready.\n");
		return 0;
	}

    while (true) {        
        sensor_sample_fetch(my_sensor);
        sensor_value v_x{}, v_y{};
        sensor_channel_get(my_sensor,
                           (sensor_channel) sensor_channel_pmw3389::SENSOR_CHAN_PMW3389_DISTANCE_X,
                           &v_x);
        sensor_channel_get(my_sensor,
                           (sensor_channel) sensor_channel_pmw3389::SENSOR_CHAN_PMW3389_DISTANCE_Y,
                           &v_y);
        printk("DELTA_X: %f, DELTA_Y: %f", sensor_value_to_double(&v_x), sensor_value_to_double(&v_y));
    }
}
