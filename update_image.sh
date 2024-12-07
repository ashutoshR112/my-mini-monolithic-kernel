#!/bin/bash

# Create directories.
mkdir -p isodir/boot/grub

# Copy kernel and GRUB configuration to the appropriate locations.
cp tinyos.bin isodir/boot/tinyos.bin
cp grub.cfg isodir/boot/grub/grub.cfg

# Create the ISO image with GRUB.
grub-mkrescue -o tinyos.iso isodir
