#include "leos/log.h"
#include "config.h"
#include "leos/mcp251xfd.h"
#include "leos/cyphal/node.h"
#include "module_setup.h"
#include "pico/stdlib.h"
#include "leos/purpleboard.h"
#include "purpleboard.h"
#include "cutdown.h"
#include <stdio.h>

void main() {
    // --- INITIALIZE MODULE ---
    leos_log_init_console(ULOG_INFO_LEVEL);
    MCP251XFD dev;
    leos_cyphal_node_t node;
    if (init_module(&dev, &node) < 0) {
        LOG_ERROR("A critical communications error has occured. This node is offline.");
        return;
    }

    // Your setup code goes here

    leos_purpleboard_t *pb = NULL;
    leos_purpleboard_result_t pb_err = leos_purpleboard_init(PB_I2C_BLOCK, PB_PIN_SDA, PB_PIN_SCL, &pb);
    if (pb_err > PB_SENSOR_NO_DETECT) {
        LOG_ERROR("The purpleboard sensors failed to initialize, not detected.");
    }


    // After finishing initialization, set our mode to operational
    node.mode.value = uavcan_node_Mode_1_0_OPERATIONAL;
    // Turn on board LED to indicate setup success.
    gpio_put(PICO_DEFAULT_LED_PIN, 1);

    // --- MAIN LOOP ---
    LOG_INFO("Entering main loop...");
    while (true)
    {
        leos_mcp251xfd_task(&dev);
        leos_cyphal_task(&node);

        purpleboard_task(pb, &node);
        cutdown_task(&node);
    }
}