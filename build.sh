#!/bin/bash

build() {
    west init keyboard_fw -m https://github.com/crohloff/keyboard68_fw.git
    cd keyboard_fw
    west update
    west build -p=auto -b nrf52840_module keyboard_fw/
}

"$1"
