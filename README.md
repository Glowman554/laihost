# FoxOS-module

This is a template for a FoxOS-module.  

## Building

```bash
make build
```

This will download the kernel headers and compile the module. The final module will be placed in the `bin/module.o` file.  

## Making an img file

On linux:
```bash
make img
```
On MacOS:
```bash
make mac-img
```

## Development

```bash
make run-bios
```

This command can be used to run the module in a virtual machine.
