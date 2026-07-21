### 🚀 Usage Instructions for `bbb_led` Kernel Module

#### 1. Build the Kernel Module
Compile the module against your system's kernel headers:
```bash
make

# Insert the compiled kernel module
sudo insmod bbb_led.ko

# Create the character device node with Major 240 and Minor 0
sudo mknod /dev/bbb_led c 240 0

# Set read/write permissions for all users
sudo chmod 666 /dev/bbb_led

echo bbb_control | sudo tee /sys/class/leds/beaglebone:green:usr3/trigger

# Turn ON the USR3 LED
echo "1" > /dev/bbb_led

# Check current LED status
cat /dev/bbb_led

# Turn OFF the USR3 LED
echo "0" > /dev/bbb_led

# Reset LED USR3 trigger back to default (or 'none')
echo none | sudo tee /sys/class/leds/beaglebone:green:usr3/trigger

# Remove the device node from /dev
sudo rm /dev/bbb_led

# Unload the module from kernel memory
sudo rmmod bbb_led
