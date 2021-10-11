OBJDIR = lib
BUILDDIR = bin

QEMUFLAGS_BIOS = -machine q35 -smp 4 -drive file=foxos.img -m 1G -cpu qemu64 -serial stdio -soundhw pcspk -netdev user,id=u1,hostfwd=tcp::9999-:9999 -device e1000,netdev=u1 -object filter-dump,id=f1,netdev=u1,file=dump.dat


FOX_GCC_PATH=/usr/local/foxos-x86_64_elf_gcc

build: kernel-headers src/lai
	@mkdir $(BUILDDIR) -p
	@mkdir $(OBJDIR) -p

	make -C src

img: build foxos.img

	sh disk.sh $(FOX_GCC_PATH)

run-bios: img
	qemu-system-x86_64 $(QEMUFLAGS_BIOS)

clean:
	@rm -r $(BUILDDIR)
	@rm -r $(OBJDIR)
	@mkdir $(BUILDDIR)
	@mkdir $(OBJDIR)

.PHONY: build

kernel-headers:
	wget https://github.com/TheUltimateFoxOS/FoxOS-kernel/releases/download/latest/kernel-headers.zip -O kernel-headers.zip
	unzip kernel-headers.zip -d kernel-headers
	rm kernel-headers.zip

foxos.img:
	wget https://github.com/TheUltimateFoxOS/FoxOS/releases/download/latest/foxos.img -O foxos.img

src/lai:	
	git clone https://github.com/managarm/lai.git src/lai