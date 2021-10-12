#pragma once

void* find_table_xsdt(pci::acpi::sdt_header_t* sdt_header, char* signature, int idx);
void* find_table_rsdt(pci::acpi::sdt_header_t* sdt_header, char* signature, int idx);