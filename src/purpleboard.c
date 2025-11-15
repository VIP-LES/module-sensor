#include "leos/log.h"
#include "purpleboard.h"
#include <leos/sensors/Temp_0_1.h>
#include <leos/sensors/Pressure_0_1.h>
#include <leos/sensors/UVLight_0_1.h>
#include <leos/sensors/AirQuality_0_1.h>

#define PB_I2C_BLOCK i2c0
#define PB_PIN_SDA 16
#define PB_PIN_SCL 17
#define PB_DATA_INTERVAL_MS 1000

const char* location_name = "purpleboard";

inline float c_to_k(float c) {
    return c + 273.15;
}

void publish_temperature(float temp_c, leos_cyphal_node_t *node) {
    LOG_INFO("Temperature: %.2f @ %s", temp_c, location_name);
    static CanardTransferID temp_tid = 0;
    leos_sensors_Temp_0_1 temp = {
        .board_ms = to_ms_since_boot(get_absolute_time()),
        .temperature.kelvin = c_to_k(temp_c)
    };
    temp.location.value.count = strlen(location_name);
    memcpy(temp.location.value.elements, location_name, temp.location.value.count);

    uint8_t buffer[leos_sensors_Temp_0_1_SERIALIZATION_BUFFER_SIZE_BYTES_];
    size_t size = sizeof(buffer);
    if (leos_sensors_Temp_0_1_serialize_(&temp, buffer, &size)) {
        LOG_DEBUG("Failed to serialize the temperature message. Skipping.");
        return;
    }
    const struct CanardTransferMetadata md = {
        .port_id = leos_sensors_Temp_0_1_FIXED_PORT_ID_,
        .priority = CanardPriorityNominal,
        .remote_node_id = CANARD_NODE_ID_UNSET,
        .transfer_id = temp_tid++,
        .transfer_kind = CanardTransferKindMessage
    };
    const struct CanardPayload p = {
        .data = buffer,
        .size = size
    };
    if (leos_cyphal_push(node, &md, p) == LEOS_CYPHAL_OK) {
        LOG_DEBUG("Couldn't push temp msg");
        return;
    }
}

void publish_pressure(float pressure_mb, leos_cyphal_node_t *node) {
    LOG_INFO("Pressure: %.2f @ %s", pressure_mb, location_name);
    static CanardTransferID pressure_tid = 0;
    leos_sensors_Pressure_0_1 pressure = {
        .board_ms = to_ms_since_boot(get_absolute_time()),
        .pressure.pascal = pressure_mb * 100
    };
    pressure.location.value.count = strlen(location_name);
    memcpy(pressure.location.value.elements, location_name, pressure.location.value.count);

    uint8_t buffer[leos_sensors_Pressure_0_1_SERIALIZATION_BUFFER_SIZE_BYTES_];
    size_t size = leos_sensors_Pressure_0_1_SERIALIZATION_BUFFER_SIZE_BYTES_;
    if (leos_sensors_Pressure_0_1_serialize_(&pressure, buffer, &size)) {
        LOG_DEBUG("Failed to serialize the pressure message. Skipping.");
        return;
    }
    const struct CanardTransferMetadata md = {
        .port_id = leos_sensors_Pressure_0_1_FIXED_PORT_ID_,
        .priority = CanardPriorityNominal,
        .remote_node_id = CANARD_NODE_ID_UNSET,
        .transfer_id = pressure_tid++,
        .transfer_kind = CanardTransferKindMessage
    };
    const struct CanardPayload p = {
        .data = buffer,
        .size = size
    };
    if (leos_cyphal_push(node, &md, p) == LEOS_CYPHAL_OK) {
        LOG_DEBUG("Couldn't push pressure msg");
        return;
    }
}

void publish_uv(uint32_t uv, leos_cyphal_node_t *node) {
    LOG_INFO("UV index: %d @ %s", uv, location_name);
    static CanardTransferID uv_tid = 0;
    leos_sensors_UVLight_0_1 uv_packet = {
        .board_ms = to_ms_since_boot(get_absolute_time()),
        .uvi = uv
    };
    uv_packet.location.value.count = strlen(location_name);
    memcpy(uv_packet.location.value.elements, location_name, uv_packet.location.value.count);

    uint8_t buffer[leos_sensors_UVLight_0_1_SERIALIZATION_BUFFER_SIZE_BYTES_];
    size_t size = sizeof(buffer);
    if (leos_sensors_UVLight_0_1_serialize_(&uv_packet, buffer, &size))
    {
        LOG_DEBUG("Failed to serialize the uv message. Skipping.");
        return;
    }
    const struct CanardTransferMetadata md = {
        .port_id = leos_sensors_UVLight_0_1_FIXED_PORT_ID_,
        .priority = CanardPriorityNominal,
        .remote_node_id = CANARD_NODE_ID_UNSET,
        .transfer_id = uv_tid++,
        .transfer_kind = CanardTransferKindMessage};
    const struct CanardPayload p = {
        .data = buffer,
        .size = size};
    if (leos_cyphal_push(node, &md, p) == LEOS_CYPHAL_OK)
    {
        LOG_DEBUG("Couldn't push uv msg");
        return;
    }
}

void publish_air(leos_purpleboard_readings_t *readings, leos_cyphal_node_t *node) {
    LOG_INFO("Air Readings: PM10=%d, PM2.5=%d, AQI_2.5_US=%d @ %s", readings->pm10_env, readings->pm25_env, readings->aqi_pm25_us, location_name);
    static CanardTransferID aq_tid = 0;
    leos_sensors_AirQuality_0_1 aq = {
        .board_ms = to_ms_since_boot(get_absolute_time()),
        .aqi_pm100_us = readings->aqi_pm100_us,
        .aqi_pm25_us = readings->aqi_pm25_us,
        .pm100_env = readings->pm100_env,
        .pm25_env = readings->pm25_env,
        .pm10_env = readings->pm10_env

    };
    aq.location.value.count = strlen(location_name);
    memcpy(aq.location.value.elements, location_name, aq.location.value.count);

    uint8_t buffer[leos_sensors_AirQuality_0_1_SERIALIZATION_BUFFER_SIZE_BYTES_];
    size_t size = sizeof(buffer);
    if (leos_sensors_AirQuality_0_1_serialize_(&aq, buffer, &size))
    {
        LOG_DEBUG("Failed to serialize the air quality message. Skipping.");
        return;
    }
    const struct CanardTransferMetadata md = {
        .port_id = leos_sensors_AirQuality_0_1_FIXED_PORT_ID_,
        .priority = CanardPriorityNominal,
        .remote_node_id = CANARD_NODE_ID_UNSET,
        .transfer_id = aq_tid++,
        .transfer_kind = CanardTransferKindMessage};
    const struct CanardPayload p = {
        .data = buffer,
        .size = size};
    if (leos_cyphal_push(node, &md, p) == LEOS_CYPHAL_OK)
    {
        LOG_DEBUG("Couldn't push aq msg");
        return;
    }
}

void purpleboard_task(leos_purpleboard_t *pb, leos_cyphal_node_t *node) {
    if (!node) return;
    static absolute_time_t next_run = 0;
    absolute_time_t now = get_absolute_time();
    if (now < next_run) return;
    next_run = make_timeout_time_ms(PB_DATA_INTERVAL_MS);

    LOG_TRACE("Collecting purpleboard measurements");
    leos_purpleboard_readings_t data;
    leos_purpleboard_result_t err = leos_purpleboard_read(pb, &data);
    if (err != PB_OK && err != PB_SENSOR_READ_DEGRADED) {
        LOG_ERROR("Failed to read from purpleboard sensors");
        return;
    }

    if (data.temperature_c != -1) {
        publish_temperature(data.temperature_c, node);
    }
    if (data.pressure_mb != -1) {
        publish_pressure(data.pressure_mb, node);
    }
    if (data.uvs != -1) {
        publish_uv(data.uvs, node);
    }
    // THIS IS A UINT32 SO THIS IS A BUG!!!! @TODO
    if (data.pm10_env != -1) {
        publish_air(&data, node);
    }
}