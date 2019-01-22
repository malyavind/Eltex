#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <linux/pci.h>

#define LEN_MSG 160
static char buf_msg[ LEN_MSG + 1 ] = "Hello World!\n";

static ssize_t x_show( struct class *class, struct class_attribute *attr, char *buf ) {

   strcpy( buf, buf_msg );
   printk( "read %d\n", strlen( buf ) );
   return strlen( buf );
}

static ssize_t x_store( struct class *class, struct class_attribute *attr, const char *buf, size_t count ) {
   printk( "write %d\n" , count );
   strncpy( buf_msg, buf, count );
   buf_msg[ count ] = '\0';
   return count;
}

#undef VERIFY_OCTAL_PERMISSIONS
#define VERIFY_OCTAL_PERMISSIONS(perms) (perms)

#define CLASS_ATTR(_name, _mode, _show, _store) struct class_attribute class_attr_hello = __ATTR(hello, 0666, &x_show, &x_store); 
CLASS_ATTR( hello, 0666, &x_show, &x_store);

static struct class *hello_class;

int __init x_init(void) {
   int res;
   hello_class = class_create( THIS_MODULE, "hello-class" );
   if( IS_ERR( hello_class ) ) printk( "bad class create\n" );
   res = class_create_file( hello_class, &class_attr_hello );
   printk("'hello' module initialized\n");
   return 0;
}

void x_cleanup(void) {
   class_remove_file( hello_class, &class_attr_hello );
   class_destroy( hello_class );
   return;
}

module_init( x_init );
module_exit( x_cleanup );
MODULE_LICENSE( "GPL" );
