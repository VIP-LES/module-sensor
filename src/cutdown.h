#pragma once
#include "leos/cyphal/node.h"

#define R1 18
#define R2 19
#define R3 20
#define R4 21

void cutdown_init(leos_cyphal_node_t *node);

void cutdown_task(leos_cyphal_node_t *node);

void cutdown_start();