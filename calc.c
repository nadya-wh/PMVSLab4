#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL");

static long first = 9;
static long second = 6;
static long result = 0;
static char operation = '*';

static int already_sent = 0;


static char *msg = NULL;


static ssize_t first_write(struct file *file, const char *buf, size_t count, loff_t *data)
{
	memset(msg, 0, 32);
	copy_from_user(msg, buf, count);

	printk("first_write_before_kstrtol, %ld\n", first);
	kstrtol(msg, 10, &first);
	printk("first_write_after_kstrtol, %ld\n", first);

	return count;
}

static ssize_t second_write(struct file *file, const char *buf, size_t count, loff_t *data)
{
	memset(msg, 0, 32);
	copy_from_user(msg, buf, count);

	printk("second_write_before_kstrtol, %ld\n", second);
	kstrtol(msg, 10, &second);
	printk("second_write_after_kstrtol, %ld\n", second);

	return count;
}

static ssize_t operation_write(struct file *file, const char *buf, size_t count, loff_t *data)
{
	memset(msg, 0, 32);
	copy_from_user(msg, buf, count);

	printk("operation_write\n");
	operation = msg[0];

	return count;
}

static ssize_t my_read(struct file *filp, char *buff, size_t len, loff_t *off)
{
	printk("first: %ld, second: %ld\n", first, second);
	switch(operation) {
	case '+': 
		result = first + second;
		break;
	case '-':
		result = first - second;
		break;
	case '*':
		result = first * second;
		break;
	case '/':
		if (second != 0) {
			result = first / second;		
		} else {
			result = -1;
		}
		break;
	}
	memset(msg, 0, 32);
	snprintf(msg, 32, "%ld\n", result);
	copy_to_user(buff, msg, len);
	if(already_sent) {
		already_sent = 0;
		return 0;
	}
	already_sent = 1;
	return 32;
}

static const struct file_operations first_fops = {
	.owner = THIS_MODULE,
	.write = first_write,
};

static const struct file_operations second_fops = {
	.owner = THIS_MODULE, 
	.write = second_write,
};

static const struct file_operations operation_fops = {
	.owner = THIS_MODULE, 
	.write = operation_write,
};

static const struct file_operations result_fops = {
	.owner = THIS_MODULE, 
	.read = my_read,
};


static struct miscdevice first_dev = {
	MISC_DYNAMIC_MINOR, 
	"first", 
	&first_fops
};

static struct miscdevice second_dev = {
	MISC_DYNAMIC_MINOR, 
	"second", 
	&second_fops
};

static struct miscdevice operation_dev = {
	MISC_DYNAMIC_MINOR, 
	"operation", 
	&operation_fops
};

static struct miscdevice result_dev = {
	MISC_DYNAMIC_MINOR, 
	"result", 
	&result_fops
};

static int __init calc_init(void) {
	msg = (char *)kmalloc(32, GFP_KERNEL);
	if(msg != NULL)
		printk("malloc allocator address: 0x%p\n", msg);

	misc_register(&first_dev);
	misc_register(&second_dev);
	misc_register(&operation_dev);
	misc_register(&result_dev);
	return 0;
}

static void __exit calc_exit(void) {
	if(msg)
		kfree(msg);

	misc_deregister(&first_dev);
	misc_deregister(&second_dev);
	misc_deregister(&operation_dev);
	misc_deregister(&result_dev);
}

module_init (calc_init);
module_exit (calc_exit);
