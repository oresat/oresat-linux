# GPIO-SIM
For working with Simulated GPIOs

## Setup

### Build and install the linux Kernel from source
Adds the GPIO-SIM kernel module and its dependencies
- Debian
```bash
./kernel_build_debian.sh
```
### Add udev rules giving user permissions for GPIO
```bash
./gpio_rules.sh
```
## USE

### Add simulated GPIO chips and lanes to configfs
- Parses from the device-tree
- Must be run on each boot, consider adding to ~/.bashrc
```bash
sudo ./setup_sims.sh
```
### remove simulated GPIO chips
```bash
sudo ./clean_sims.sh
```
