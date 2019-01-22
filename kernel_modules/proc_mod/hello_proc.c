#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <asm/uaccess.h>
#include <linux/uaccess.h>

int len, temp;

char *hello_str;

ssize_t read_proc(struct file *filp, char *buf, size_t count, loff_t *offp ) 
{

	if(count > temp)
	{
		count = temp;
	}
	temp = temp - count;
	copy_to_user(buf, hello_str, count);
	if(count == 0)
		temp = len;
	return count;
}

struct file_operations proc_fops = {
	read: read_proc
};

static int __init hello_init(void) {
	if ( proc_create("hello_world", 0, NULL, &proc_fops) == 0) {
		printk(KERN_ERR "Unable to register \"Hello, world!\" proc file\n");
        	return -ENOMEM;
	}
	hello_str = "Hello world!\n";
	len = strlen(hello_str);
	temp = len;
	return 0;
}

static void __exit hello_exit(void) {
	remove_proc_entry("hello_world", NULL);
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dmitriy Malyavin");
MODULE_DESCRIPTION("\"Hello, world!\" minimal module");
MODULE_VERSION("proc");
