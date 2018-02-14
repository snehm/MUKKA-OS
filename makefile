build :
	cd Sysboot/"stage1"/ && make
	cd Sysboot/"stage2"/ && make
	cd SysCore/Lib/ && make
	cd SysCore/"Hal"/ && make
	cd SysCore/Kernel/ && make

clean:
	cd Sysboot/"stage1"/ && make clean
	cd Sysboot/"stage2"/ && make clean
	cd SysCore/Lib/ && make clean
	cd SysCore/"Hal"/ && make clean
	cd SysCore/Kernel/ && make clean
	sudo rm myfloppy.img
	
install:
	sudo dd if=/dev/zero of=myfloppy.img bs=512 count=2880
	sudo losetup /dev/loop0 myfloppy.img 
	sudo mkdosfs -F 12 /dev/loop0
	mkdir myfloppy
	sudo mount /dev/loop0 myfloppy -t msdos -o "fat=12"
	cd Sysboot/"stage1"/ && make install
	cd Sysboot/"stage2"/ && make install
	cd SysCore/Kernel/ && make install
	sudo umount myfloppy
	sudo rm -r myfloppy 
	sudo losetup -d /dev/loop0
	
	
