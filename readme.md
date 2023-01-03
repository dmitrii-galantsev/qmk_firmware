# POPI's Firmware for QMK keyboards

Compile and flash system76's Launch

`qmk flash --keyboard system76/launch_1 --keymap popi_keymap`

OpenSUSE Tumbleweed setup and dependencies

```bash
python3 -m pip install --user qmk
sudo zypper install cross-arm-none-gcc13 cross-avr-gcc7 dfu-programmer dfu-util avrdude-devel avr-libc
qmk setup
# there might be a warning about uname rules
# follow the instructions given by qmk to resolve
```

Get compile_commands.json

`bear -- qmk flash --keyboard system76/launch_1 --keymap popi_keymap`
