# OreSat Linux updater (onboard)
The onboard OreSat side will untar the tar of packages and install the packages listed in instuctions.conf for the A8 it is currently running on.

### To use
`python3 updater.py path-to-tar-file device` 
where device can be: GPS, StarTracker, OreSatLive, or CFC

### Status
- [ ] Package managment
    - [x] Untar
    - [x] Installs all packages in tar
    - [ ] Installs packages based off of instructions.conf
    - [ ] Removes packages based off of instructions.conf
    - [ ] Output results of the updater in result.txt
- [ ] Source code
- [ ] Kernel updates
