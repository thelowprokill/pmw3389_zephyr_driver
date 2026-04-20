# PMW3389 Zephyr Driver Module

## Usage

This is a west module. You can integrate it in your application by adding the project to your west manifest or by specifying it in your `EXTRA_ZEPHYR_MODULES`.

For sensor configuration the `resolution` devicetree setting is provided.
The resolution is specified in counts per inch, and care should be taken to ensure that the
sensor-internal accumulator does not overflow (±32767).
The sensor supports a maximum SPI clock frequency of 2MHz.

```c
&lpspi4 {
    my_sensor: pmw3389@0 {
        compatible = "pixart,pmw3389";
        spi-max-frequency = <2000000>; // 2MHz
        resolution = <200>; // overflow in 0.5s at 8m/s -> 200 CPI
        status = "okay";
        reg = <0>;
    };
};
```

## Example

A full working example for reading sensor values on a Teensy 4.1 can be found [here](samples/sensor/pmw3389/).

## Writing Out-Of-Tree Drivers

Since this was kind of a pain to figure out, here are a few helpful links if you want to do this:

* https://interrupt.memfault.com/blog/building-drivers-on-zephyr
* https://blog.golioth.io/adding-an-out-of-tree-sensor-driver-to-zephyr/
* https://jonasotto.com/posts/zephyr_out_of_tree_driver/
