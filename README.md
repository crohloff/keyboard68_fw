# Zephyr based - Keyboard FW
[![Build Status](https://travis-ci.com/crohloff/keyboard68_fw.svg?branch=master)](https://travis-ci.com/crohloff/keyboard68_fw)

## Zephyr OS
[Zephyr Project](https://www.zephyrproject.org)

## Prerequisites
- use virtualenv
- `west -V` should be 0.7.2 or higher
	- if lower run `pip install -U west`

## Sync
- `west init keyboard_fw -m https://github.com/crohloff/keyboard68_fw.git`
- `cd keyboard_fw`
- `west update`

## Additional Dependencies
- `pip install -r zephyr/scripts/requirements.txt`

## Build
- `west build -p=auto -b nrf52840_module keyboard_fw/`
