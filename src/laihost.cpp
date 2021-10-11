extern "C" {
	#include <lai/host.h>
}

#include <paging/page_table_manager.h>
#include <driver/serial.h>
#include <interrupts/panic.h>
#include <memory/heap.h>
#include <port.h>
#include <pci/pci.h>

#include <scheduling/hpet/hpet.h>
#include <scheduling/pit/pit.h>

#include <stivale2.h>
#include <apic/apic.h>

#include <acpispec/tables.h>

extern "C" {
	void* laihost_map(size_t address, size_t count) {
		for (int i = 0; i < count / 0x1000; i++) {
			g_page_table_manager.map_memory((void*) (address + i * 0x1000), (void*) (address + i * 0x1000));
		}

		return (void*) address;
	}

	void laihost_unmap(void* pointer, size_t count) {
		driver::global_serial_driver->printf("WARNING: laihost_unmap: %x %d not implemented!\n", pointer, count);
	}

	void laihost_log(int level, const char* msg) {
		switch (level) {
			case LAI_WARN_LOG:
				driver::global_serial_driver->printf("WARNING: %s\n", msg);
				break;
			
			case LAI_DEBUG_LOG:
				driver::global_serial_driver->printf("DEBUG: %s\n", msg);
				break;
			
			default:
				driver::global_serial_driver->printf("UNKNOWN: %s\n", msg);
				break;
		}
	}

	__attribute__((noreturn)) void laihost_panic(const char* msg) {
		interrupts::Panic((char*) msg).do_it(nullptr);
		while (true) {
			asm volatile("cli; hlt");
		}
	}

	void* laihost_malloc(size_t size) {
		return malloc(size);
	}

	void* laihost_realloc(void *oldptr, size_t newsize, size_t oldsize) {
		return realloc(oldptr, oldsize, newsize);
	}

	void laihost_free(void *ptr, size_t size) {
		free(ptr);
	}

	void laihost_outb(uint16_t port, uint8_t val) {
		outb(port, val);
	}

	void laihost_outw(uint16_t port, uint16_t val) {
		outw(port, val);
	}

	void laihost_outd(uint16_t port, uint32_t val) {
		outl(port, val);
	}

	uint8_t laihost_inb(uint16_t port) {
		return inb(port);
	}

	uint16_t laihost_inw(uint16_t port) {
		return inw(port);
	}

	uint32_t laihost_ind(uint16_t port) {
		return inl(port);
	}

	void laihost_pci_writeb(uint16_t seg, uint8_t bus, uint8_t slot, uint8_t fun, uint16_t offset, uint8_t val) {
		laihost_panic("laihost_pci_writeb not implemented!");
	}

	void laihost_pci_writew(uint16_t seg, uint8_t bus, uint8_t slot, uint8_t fun, uint16_t offset, uint16_t val) {
		laihost_panic("laihost_pci_writew not implemented!");
	}

	void laihost_pci_writed(uint16_t seg, uint8_t bus, uint8_t slot, uint8_t fun, uint16_t offset, uint32_t val) {
		pci::pci_write(bus, slot, fun, offset, val);
	}

	uint8_t laihost_pci_readb(uint16_t seg, uint8_t bus, uint8_t slot, uint8_t fun, uint16_t offset) {
		laihost_panic("laihost_pci_readb not implemented!");
	}

	uint16_t laihost_pci_readw(uint16_t seg, uint8_t bus, uint8_t slot, uint8_t fun, uint16_t offset) {
		laihost_panic("laihost_pci_readw not implemented!");
	}

	uint32_t laihost_pci_readd(uint16_t seg, uint8_t bus, uint8_t slot, uint8_t fun, uint16_t offset) {
		return pci::pci_read(bus, slot, fun, offset);
	}


	void laihost_sleep(uint64_t ms) {
		if (hpet::is_available()) {
			hpet::sleep(ms);
		} else {
			PIT::sleep(ms);
		}
	}

	uint64_t laihost_timer(void) {
		laihost_panic("laihost_timer not implemented! What is that even?");
	}

	void laihost_handle_amldebug(lai_variable_t* var) {
		driver::global_serial_driver->printf("DEBUG: laihost_handle_amldebug with %x\n", var);
	}

	int laihost_sync_wait(struct lai_sync_state *sync, unsigned int val, int64_t timeout) {
		driver::global_serial_driver->printf("WARNING: laihost_sync_wait not implemented!\n");
		return -1;
	}

	void laihost_sync_wake(struct lai_sync_state *sync) {
		driver::global_serial_driver->printf("WARNING: laihost_sync_wake not implemented!\n");
	}

	void* laihost_scan(const char *sig, size_t index) {

		void* result = nullptr;

		stivale2_struct_tag_rsdp* rsdp_tag = stivale2_tag_find<stivale2_struct_tag_rsdp>(global_bootinfo, STIVALE2_STRUCT_TAG_RSDP_ID);

		pci::acpi::rsdp2_t* rsdp = (pci::acpi::rsdp2_t*) ((uint64_t) rsdp_tag->rsdp);

		if (rsdp->xsdt_address != 0) {
			pci::acpi::sdt_header_t* xsdt = (pci::acpi::sdt_header_t*) (((pci::acpi::rsdp2_t*) rsdp_tag->rsdp)->xsdt_address);

			if (memcmp(sig, "DSDT", 4) == 0) {
				acpi_fadt_t* fadt = (acpi_fadt_t*) pci::acpi::find_table_xsdt(xsdt, (char*) "FACP");
				return (void*) (uint64_t) fadt->dsdt;
			}

			result = pci::acpi::find_table_xsdt(xsdt, (char*) sig);
		} else {
			pci::acpi::sdt_header_t* rsdt = (pci::acpi::sdt_header_t*) (uint64_t) (((pci::acpi::rsdp2_t*) rsdp_tag->rsdp)->rsdt_address);

			if (memcmp(sig, "DSDT", 4) == 0) {
				acpi_fadt_t* fadt = (acpi_fadt_t*) pci::acpi::find_table_rsdt(rsdt, (char*) "FACP");
				return (void*) (uint64_t) fadt->dsdt;
			}
			
			result = pci::acpi::find_table_rsdt(rsdt, (char*) sig);
		}

		driver::global_serial_driver->printf("WARNING: search result for %s is %x", sig, result);

		return result;
	}
}
