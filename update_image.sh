mkdir -p isodir/boot/grub

cp tinyos.bin isodir/boot/tinyos.bin
cp grub.cfg isodir/boot/grub/grub.cfg

grub-mkrescue -o tinyos.iso isodir