# GPIO-SIM
For working with the GPIO-SIM Linux Kernel

## Setup

### Build and install the linux Kernel from source
- Debian
```bash
./kernel_build_debian.sh
```
### Add udev rules giving user permissions for GPIO
```bash
./gpio_rules.sh
```
## USE

### Add simulated GPIO chips and lanes
- Parses from the device-tree
- Must be run on each boot, consider adding to ~/.bashrc
```bash
./setup_sims.sh
```
### remove simulated GPIO chips
```bash
./clean_sims.sh
```



