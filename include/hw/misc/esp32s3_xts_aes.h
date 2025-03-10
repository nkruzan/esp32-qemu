/*
 * ESP32-S3 XTS-AES emulation
 *
 * Copyright (c) 2023 Espressif Systems (Shanghai) Co. Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 or
 * (at your option) any later version.
 */


#pragma once

#include "hw/hw.h"
#include "hw/sysbus.h"
#include "hw/registerfields.h"
#include "hw/nvram/esp32c3_efuse.h"
#include "hw/xtensa/esp32s3_clk.h"

#define TYPE_ESP32S3_XTS_AES "misc.esp32s3.xts_aes"
#define ESP32S3_XTS_AES(obj) OBJECT_CHECK(ESP32S3XtsAesState, (obj), TYPE_ESP32S3_XTS_AES)
#define ESP32S3_XTS_AES_GET_CLASS(obj) OBJECT_GET_CLASS(ESP32S3XtsAesClass, obj, TYPE_ESP32S3_XTS_AES)
#define ESP32S3_XTS_AES_CLASS(klass) OBJECT_CLASS_CHECK(ESP32S3XtsAesClass, klass, TYPE_ESP32S3_XTS_AES)

#define ESP32S3_XTS_AES_PLAIN_REG_CNT 16
#define ESP32S3_XTS_AES_REGS_SIZE (0x60)

/**
 * @brief Status of the Manual Encryption block.
 */
typedef enum {
    XTS_AES_IDLE = 0,
    XTS_AES_BUSY = 1,
    XTS_AES_DONE = 2,
    XTS_AES_RELEASE = 3,
} ESP32S3XtsAesStatus;

typedef struct ESP32S3XtsAesState {
    SysBusDevice parent_obj;
    MemoryRegion iomem;

    uint32_t plaintext[ESP32S3_XTS_AES_PLAIN_REG_CNT];
    uint32_t ciphertext[ESP32S3_XTS_AES_PLAIN_REG_CNT];
    uint32_t linesize;
    uint32_t destination;
    uint64_t physical_addr;
    uint32_t state;

    ESP32S3ClockState *clock;
    ESPEfuseState *efuse;
} ESP32S3XtsAesState;

typedef struct ESP32S3XtsAesClass {
    SysBusDeviceClass parent_class;
    /* Virtual methods */
    bool (*is_ciphertext_spi_visible)(ESP32S3XtsAesState *s);
    bool (*is_flash_enc_enabled)(ESP32S3XtsAesState *s);
    bool (*is_manual_enc_enabled)(ESP32S3XtsAesState *s);
    void (*read_ciphertext)(ESP32S3XtsAesState *s, uint32_t* spi_data_regs, uint32_t* spi_data_size, uint32_t* spi_addr, uint32_t* spi_addr_size);
    void (*decrypt)(ESP32S3XtsAesState *s, uint32_t physical_address, uint8_t * data, uint32_t size);
} ESP32S3XtsAesClass;

REG32(XTS_AES_PLAIN_0_REG, 0x0000)
REG32(XTS_AES_PLAIN_1_REG, 0x0004)
REG32(XTS_AES_PLAIN_2_REG, 0x0008)
REG32(XTS_AES_PLAIN_3_REG, 0x000C)
REG32(XTS_AES_PLAIN_4_REG, 0x0010)
REG32(XTS_AES_PLAIN_5_REG, 0x0014)
REG32(XTS_AES_PLAIN_6_REG, 0x0018)
REG32(XTS_AES_PLAIN_7_REG, 0x001C)
REG32(XTS_AES_PLAIN_8_REG, 0x0020)
REG32(XTS_AES_PLAIN_9_REG, 0x0024)
REG32(XTS_AES_PLAIN_10_REG, 0x0028)
REG32(XTS_AES_PLAIN_11_REG, 0x002C)
REG32(XTS_AES_PLAIN_12_REG, 0x0030)
REG32(XTS_AES_PLAIN_13_REG, 0x0034)
REG32(XTS_AES_PLAIN_14_REG, 0x0038)
REG32(XTS_AES_PLAIN_15_REG, 0x003C)

REG32(XTS_AES_LINESIZE_REG, 0x0040)
    FIELD(XTS_AES_LINESIZE_REG, XTS_AES_LINESIZE, 0, 2)

REG32(XTS_AES_DESTINATION_REG, 0x0044)
    FIELD(XTS_AES_DESTINATION_REG, XTS_AES_DESTINATION, 0, 1)

REG32(XTS_AES_PHYSICAL_ADDRESS_REG, 0x0048)
    FIELD(XTS_AES_PHYSICAL_ADDRESS_REG, XTS_AES_PHYSICAL_ADDRESS, 0, 30)

REG32(XTS_AES_TRIGGER_REG, 0x004C)
    FIELD(XTS_AES_TRIGGER_REG, XTS_AES_TRIGGER, 0, 1)

REG32(XTS_AES_RELEASE_REG, 0x0050)
    FIELD(XTS_AES_RELEASE_REG, XTS_AES_RELEASE, 0, 1)

REG32(XTS_AES_DESTROY_REG, 0x0054)
    FIELD(XTS_AES_DESTROY_REG, XTS_AES_DESTROY, 0, 1)

REG32(XTS_AES_STATE_REG, 0x0058)
    FIELD(XTS_AES_STATE_REG, XTS_AES_STATE, 0, 2)

REG32(XTS_AES_DATE_REG, 0x005C)
    FIELD(XTS_AES_DATE_REG, XTS_AES_DATE, 0, 30)
