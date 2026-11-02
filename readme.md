# POPI's Firmware for QMK keyboards

Personal keymaps on top of upstream [QMK](https://github.com/qmk/qmk_firmware) (0.34.7, November 2026):

| Keyboard | Keymap | Files |
|---|---|---|
| System76 Launch (launch_1) | `popi_keymap` | `keyboards/system76/launch_1/keymaps/popi_keymap/` |
| GMMK Pro rev1 ANSI | `gourdo1` | `keyboards/gmmk/pro/rev1/ansi/keymaps/gourdo1/`, `users/gourdo1/` |

Prebuilt images in the repository root: `system76_launch_1_popi_keymap.hex`, `gmmk_pro_rev1_ansi_gourdo1.bin`.

## Build and flash

```bash
qmk config user.qmk_home=$PWD
qmk git-submodule
qmk flash -kb system76/launch_1 -km popi_keymap
qmk flash -kb gmmk/pro/rev1/ansi -km gourdo1
```

Nix provides the CLI and both toolchains: `nix shell nixpkgs#qmk`.

OpenSUSE Tumbleweed instead:

```bash
python3 -m pip install --user qmk
sudo zypper install cross-arm-none-gcc13 cross-avr-gcc7 dfu-programmer dfu-util avrdude-devel avr-libc
qmk setup
```

compile_commands.json: `qmk compile --compiledb -kb system76/launch_1 -km popi_keymap`

## Updating to a newer QMK

```bash
git fetch upstream
git rebase upstream/master
```
