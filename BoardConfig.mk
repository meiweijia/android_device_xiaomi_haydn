#
# Copyright (C) 2021 The LineageOS Project
#
# SPDX-License-Identifier: Apache-2.0
#

DEVICE_PATH := device/xiaomi/haydn

# Inherit from sm8350-common
include device/xiaomi/sm8350-common/BoardConfigCommon.mk

# Board
TARGET_BOOTLOADER_BOARD_NAME := haydn

# Display
TARGET_SCREEN_DENSITY := 560

# Init
TARGET_INIT_VENDOR_LIB := //$(DEVICE_PATH):libinit_haydn
TARGET_RECOVERY_DEVICE_MODULES := libinit_haydn

# Kernel
TARGET_KERNEL_SOURCE := kernel/xiaomi/haydn
TARGET_KERNEL_CONFIG := vendor/haydn-qgki_defconfig

# Kernel modules
BOOT_KERNEL_MODULES := \
    hwid.ko \
    fts_touch_spi.ko \
    focaltech_touch.ko \
    xiaomi_touch.ko

BOARD_VENDOR_KERNEL_MODULES_LOAD := $(strip $(shell cat $(DEVICE_PATH)/modules.load))
BOARD_VENDOR_RAMDISK_RECOVERY_KERNEL_MODULES_LOAD := $(BOOT_KERNEL_MODULES)

# Partitions
BOARD_DTBOIMG_PARTITION_SIZE := 8388608
BOARD_USERDATAIMAGE_PARTITION_SIZE := 112421842944

# OTA assert
TARGET_OTA_ASSERT_DEVICE := haydn|haydnin

# Include proprietary files
-include vendor/xiaomi/haydn/BoardConfigVendor.mk
-include vendor/xiaomi/haydn-firmware/BoardConfigVendor.mk
