#pragma once
#include "leos/purpleboard.h"
#include "leos/cyphal/node.h"

#define PB_I2C_BLOCK i2c1
#define PB_PIN_SDA 14
#define PB_PIN_SCL 15
#define PB_DATA_INTERVAL_MS 1000

void purpleboard_task(leos_purpleboard_t *pb, leos_cyphal_node_t *node);