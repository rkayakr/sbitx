# Quick Start

This page summarizes installation and upgrade guidance for the sBitx 64-bit image.

## Backup your data first

Before installing or upgrading, back up your existing `sbitx/data` and `sbitx/web` folders to preserve logbook data, hardware calibration, and user settings.

### Recommended backup method

Use **sBITX EZ Data** from sBitx Toolbox to copy critical data files to a USB drive.

### Manual backup

```console
cd $HOME && mv sbitx sbitx_orig
```

Restore after installation:

```console
cd $HOME && cp -r sbitx_orig/web/*.mc sbitx/web/ && cp -r sbitx_orig/data/* sbitx/data/
```

## Installation & upgrades

Detailed installation and upgrade instructions are maintained in the project wiki:

- [How to install or upgrade your sBitx application](https://github.com/drexjj/sbitx/wiki/How-to-install-or-upgrade-your-sBitx-application)

## Download the 64-bit image

A preconfigured Raspberry Pi 4/5 image is available for 32GB SD card or USB drive installation.

- [Download the latest 64-bit image](https://github.com/drexjj/sbitx/releases)

The image includes sBITX Toolbox and additional ham radio tools.
