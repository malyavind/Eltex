#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/pci.h>
#include <linux/skbuff.h>
#include <linux/inet.h>
#include <linux/ip.h>
#include <net/ip.h>
#include <linux/tcp.h>
#include <linux/icmp.h>

volatile int port = 3344;

struct nf_hook_ops nf_incoming, nf_outgoing;
struct sk_buff *skbf;
struct iphdr *iph;
struct udphdr *udph;

static unsigned int main_hook(void *priv, struct sk_buff *skb,
 const struct nf_hook_state *state) {
	struct iphdr *iph;
	struct udphdr *udph;
	iph = (struct iphdr *) skb_network_header(skb);
	if(iph->protocol == IPPROTO_UDP) {
		udph = (struct udphdr *) skb_transport_header(skb);
		if(ntohs(udph->dest) == port || ntohs(udph->source) == port) {
			printk(KERN_INFO "drop\n");
			return NF_DROP;
		}
	}
	return NF_ACCEPT;
}
static ssize_t x_show( struct class *class, struct class_attribute *attr, char *buf ) {
	printk(KERN_INFO "read %d\n", strlen( buf ) );
	return sprintf(buf, "%d/n", port);
}

static ssize_t x_store( struct class *class, struct class_attribute *attr, const char *buf, size_t count ) {
	printk(KERN_INFO "write %d\n" , count );
	sscanf(buf, "%d", &port);
	return count;
}

#undef VERIFY_OCTAL_PERMISSIONS
#define VERIFY_OCTAL_PERMISSIONS(perms) (perms)

#define CLASS_ATTR(_name, _mode, _show, _store) struct class_attribute class_attr_fw = __ATTR(fw, 0666, &x_show, &x_store); 
CLASS_ATTR( fw, 0666, &x_show, &x_store);

static struct class *fw_class;

int __init x_init(void) {
	int res;
	fw_class = class_create( THIS_MODULE, "fw-class" );
	if( IS_ERR( fw_class ) ) printk(KERN_INFO "bad class create\n" );
	res = class_create_file( fw_class, &class_attr_fw );
	printk(KERN_INFO "'fw' module initialized\n");
	
	nf_incoming.hook = main_hook;
	nf_incoming.pf = PF_INET;
	nf_incoming.hooknum = NF_INET_LOCAL_IN;
	nf_incoming.priority = NF_IP_PRI_FIRST;
	nf_register_net_hook(&init_net, &nf_incoming); //hook registered
	nf_outgoing.hook = main_hook;
	nf_outgoing.pf = PF_INET;
	nf_outgoing.hooknum = NF_INET_LOCAL_OUT;
	nf_outgoing.priority = NF_IP_PRI_FIRST;
	nf_register_net_hook(&init_net, &nf_outgoing); //hook registered
	
	
	printk(KERN_INFO "FireWall loaded\n");
	
	return 0;
}

void x_cleanup(void) {
	class_remove_file( fw_class, &class_attr_fw );
	class_destroy(fw_class);
	nf_unregister_net_hook(&init_net, &nf_incoming);
	nf_unregister_net_hook(&init_net, &nf_outgoing);
	return;
}

module_init( x_init );
module_exit( x_cleanup );
MODULE_LICENSE( "GPL" );
