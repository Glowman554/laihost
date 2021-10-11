#include <kmod.h>
#include <driver/serial.h>
#include <pci/acpi.h>

#include <sci_handler.h>

#include <stivale2.h>

extern "C" {
	#include <lai/core.h>
	#include <lai/helpers/sci.h>
	#include <acpispec/tables.h>
}

int acpi_get_sci_irq(void) {
	acpi_fadt_t* result = nullptr;

	stivale2_struct_tag_rsdp* rsdp_tag = stivale2_tag_find<stivale2_struct_tag_rsdp>(global_bootinfo, STIVALE2_STRUCT_TAG_RSDP_ID);

	pci::acpi::rsdp2_t* rsdp = (pci::acpi::rsdp2_t*) ((uint64_t) rsdp_tag->rsdp);

	if (rsdp->xsdt_address != 0) {
		pci::acpi::sdt_header_t* xsdt = (pci::acpi::sdt_header_t*) (((pci::acpi::rsdp2_t*) rsdp_tag->rsdp)->xsdt_address);

		result = (acpi_fadt_t*) pci::acpi::find_table_xsdt(xsdt, (char*) "FACP");
	} else {
		pci::acpi::sdt_header_t* rsdt = (pci::acpi::sdt_header_t*) (uint64_t) (((pci::acpi::rsdp2_t*) rsdp_tag->rsdp)->rsdt_address);

		result = (acpi_fadt_t*) pci::acpi::find_table_rsdt(rsdt, (char*) "FACP");
	}

	return result->sci_irq;
}

void init() {
	stivale2_struct_tag_rsdp* rsdp_tag = stivale2_tag_find<stivale2_struct_tag_rsdp>(global_bootinfo, STIVALE2_STRUCT_TAG_RSDP_ID);

	pci::acpi::rsdp2_t* rsdp = (pci::acpi::rsdp2_t*) ((uint64_t) rsdp_tag->rsdp);

	lai_set_acpi_revision(rsdp->revision);

	//lai_enable_tracing(LAI_TRACE_OP | LAI_TRACE_IO);
	lai_create_namespace();

	int irq = acpi_get_sci_irq();

	driver::global_serial_driver->printf("SCI IRQ: %d\n", irq);

	SciHandler* handler = new SciHandler(irq + 0x20);
}

define_module("laihost", init);
