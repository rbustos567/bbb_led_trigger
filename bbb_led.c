/*
 * bbb_led.c - BeagleBone Black LED Control Character Device Driver
 *
 * Description: Registers a character device (/dev/bbb_led) with Major 240
 * to toggle the onboard USR3 LED. Utilizes the Linux Kernel LED Trigger
 * API to safely control hardware states without GPIO resource contention.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/leds.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("BBB Kernel Dev");
MODULE_DESCRIPTION("LED Control via Kernel LED Trigger API");

#define DEVICE_NAME "bbb_led"
#define MAJOR_NUM 240

static char estado_led = '0';
DEFINE_LED_TRIGGER(led_trig);

static int dev_open(struct inode *inodep, struct file *filep) { return 0; }
static int dev_release(struct inode *inodep, struct file *filep) { return 0; }

static ssize_t dev_read(struct file *filep, char __user *buffer, size_t len, loff_t *offset) {
    char msg[64];
    int msg_len;

    if (*offset > 0) return 0;

    msg_len = snprintf(msg, sizeof(msg), "LED status: %c\n", estado_led);

    if (copy_to_user(buffer, msg, msg_len)) {
        return -EFAULT;
    }

    *offset = msg_len;
    return msg_len;
}

static ssize_t dev_write(struct file *filep, const char __user *buffer, size_t len, loff_t *offset) {
    char val;

    if (copy_from_user(&val, buffer, 1)) {
        return -EFAULT;
    }

    if (val == '1') {
        estado_led = '1';
        led_trigger_event(led_trig, LED_FULL);
        pr_info("BBB_LED: On\n");
    } else if (val == '0') {
        estado_led = '0';
        led_trigger_event(led_trig, LED_OFF);
        pr_info("BBB_LED: Off\n");
    }

    return len;
}

static struct file_operations fops = {
    .open = dev_open,
    .read = dev_read,
    .write = dev_write,
    .release = dev_release,
};

static int __init bbb_led_init(void) {
    int ret;

    ret = register_chrdev(MAJOR_NUM, DEVICE_NAME, &fops);
    if (ret < 0) {
        pr_alert("BBB_LED: Failed while trying to register Major %d\n", MAJOR_NUM);
        return ret;
    }

    // Registrar un nuevo trigger llamado "bbb_control"
    led_trigger_register_simple("bbb_control", &led_trig);

    pr_info("BBB_LED: Module loaded with trigger 'bbb_control'.\n");
    return 0;
}

static void __exit bbb_led_exit(void) {
    led_trigger_unregister_simple(led_trig);
    unregister_chrdev(MAJOR_NUM, DEVICE_NAME);
    pr_info("BBB_LED: Module removed.\n");
}

module_init(bbb_led_init);
module_exit(bbb_led_exit);
