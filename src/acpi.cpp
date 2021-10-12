#include <pci/acpi.h>

void* find_table_xsdt(pci::acpi::sdt_header_t* sdt_header, char* signature, int idx) {
	int entries = (sdt_header->length - sizeof(pci::acpi::sdt_header_t)) / 8;

	int count = 0;

	for(int t = 0; t < entries; t++) {
		pci::acpi::sdt_header_t* new_header = (pci::acpi::sdt_header_t*) * (uint64_t*) ((uint64_t) sdt_header + sizeof(pci::acpi::sdt_header_t) + (t * 8));
		for(int i = 0; i < 4; i++) {
			if(new_header->signature[i] != signature[i]) {
				break;
			}
			if(i == 3) {
				if (count++ == idx) {
					return new_header;
				} else {
					break;
				}
			}
		}
	}
	
	return 0;
}

void* find_table_rsdt(pci::acpi::sdt_header_t* sdt_header, char* signature, int idx) {
	int entries = (sdt_header->length - sizeof(pci::acpi::sdt_header_t)) / 4;

	int count = 0;

	for(int t = 0; t < entries; t++) {
		pci::acpi::sdt_header_t* new_header = (pci::acpi::sdt_header_t*) (uint64_t) * (uint32_t*) ((uint64_t) sdt_header + sizeof(pci::acpi::sdt_header_t) + (t * 4));
		for(int i = 0; i < 4; i++) {
			if(new_header->signature[i] != signature[i]) {
				break;
			}
			if(i == 3) {
				if (count++ == idx) {
					return new_header;
				} else {
					break;
				}
			}
		}
	}

	return 0;
}