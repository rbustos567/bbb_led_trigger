# BeagleBone Black LED Control Kernel Module (`bbb_led`)

A Linux character device driver (`/dev/bbb_led`) designed for the BeagleBone Black (TI AM3358). It exposes a user-space file interface to control onboard LEDs without hardware resource conflicts by integrating directly with the Linux Kernel LED Trigger Subsystem (`led_trigger_event`).

---

## 🛠️ Features

* **Safe User-Kernel Data Transfer:** Uses `copy_from_user` and `copy_to_user` to interact safely across protection rings.
* **Kernel Trigger Integration:** Bypasses direct GPIO pin lockups by utilizing the kernel's native `led_trigger` framework.
* **Safe Buffer Operations:** Utilizes `snprintf` to prevent stack buffer overflow conditions inside the kernel space.

---

## 🚀 Getting Started

### 1. Build the Kernel Module
Compile the driver against your running kernel headers:
```bash
make
```

### 2. Load the Module & Create Device Node
Insert the compiled module into kernel space, create its associated character device node, and set access permissions:
\`\`\`bash
# Insert module into the kernel
sudo insmod bbb_led.ko

# Create the character device node (Major 240, Minor 0)
sudo mknod /dev/bbb_led c 240 0

# Grant read/write permissions
sudo chmod 666 /dev/bbb_led
\`\`\`

### 3. Bind the LED Trigger
Assign the driver's custom trigger (`bbb_control`) to the USR3 LED:
\`\`\`bash
echo bbb_control | sudo tee /sys/class/leds/beaglebone:green:usr3/trigger
\`\`\`

---

## 🧪 Usage & Testing

Interact with the hardware device via standard user-space file operations:

\`\`\`bash
# Turn ON the USR3 LED
echo "1" > /dev/bbb_led

# Check current LED status
cat /dev/bbb_led

# Turn OFF the USR3 LED
echo "0" > /dev/bbb_led
\`\`\`

---

## 🧹 Cleanup & Unload

Restore default system states, remove the device node, and unload the kernel module:

\`\`\`bash
# Reset LED USR3 trigger back to default
echo none | sudo tee /sys/class/leds/beaglebone:green:usr3/trigger

# Remove device node
sudo rm /dev/bbb_led

# Unload kernel module
sudo rmmod bbb_led

# Clean build artifacts
make clean
\`\`\`
