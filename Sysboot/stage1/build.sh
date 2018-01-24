nasm -f bin boot1.asm -o "LOADER.SYS"
sudo dd if=LOADER.SYS of=/dev/loop0
