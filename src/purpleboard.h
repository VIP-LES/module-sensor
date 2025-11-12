#pragma once
#include "leos/purpleboard.h"
#include "leos/cyphal/node.h"

#define PB_I2C_BLOCK i2c0
#define PB_PIN_SDA 16
#define PB_PIN_SCL 17
#define PB_DATA_INTERVAL_MS 1000

void purpleboard_task(leos_purpleboard_t *pb, leos_cyphal_node_t *node);