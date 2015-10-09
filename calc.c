#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/init.h>

int first = 9;
int second = 6;
char operation = '*';

static int calc(struct seq_file* m, void* v) 
{
	int result = 0;
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
			seq_printf(m, "Division by zero\n");
			return 0;
		}
		break;
	}
	seq_printf(m, "%d\n", result);
	return 0;
}

static ssize_t first_write(struct file *file, const char *buf, size_t count, loff_t *data)
{
	printk("first_write\n");
	int i;
	for(i = 0, first = 0; i < count - 1; i++)
		first = first * 10 + buf[i] - '0';
	return count;
}

static ssize_t second_write(struct file *file, const char *buf, size_t count, loff_t *data)
{
	printk("second_write\n");
	int i;
	for(i = 0, second = 0; i < count - 1; i++)
		second = second * 10 + buf[i] - '0';
	return count;
}

static ssize_t operation_write(struct file *file, const char *buf, size_t count, loff_t *data)
{
	printk("operation_write\n");
	operation = buf[0];
	return count;
}

static int result_open(struct inode *inode, struct file *file)
{
	return single_open(file, calc, NULL);
}


static const struct file_operations first_fops = {
	.owner = THIS_MODULE,
	.write = first_write,
};

static const struct file_operations second_fops = {
	.owner = THIS_MODULE, 
	.write = second_write,
	.read = seq_read
};

static const struct file_operations operation_fops = {
	.owner = THIS_MODULE, 
	.write = operation_write,
	.read = seq_read,
};

static const struct file_operations result_fops = {
	.owner = THIS_MODULE, 
	.open = result_open, 
	.read = seq_read,
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
	misc_register(&first_dev);
	misc_register(&second_dev);
	misc_register(&operation_dev);
	misc_register(&result_dev);
	return 0;
}

static void __exit calc_exit(void) {
	misc_deregister(&first_dev);
	misc_deregister(&second_dev);
	misc_deregister(&operation_dev);
	misc_deregister(&result_dev);
}

MODULE_LICENSE("GPL");
MODULE_VERSION("dev");
module_init (calc_init);
module_exit (calc_exit);
