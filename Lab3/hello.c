#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include <linux/errno.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>


MODULE_AUTHOR("Oleksandr Tkachenko <sashatkach4026@gmail.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Lab3");
MODULE_LICENSE("Dual BSD/GPL");

static unsigned int param = 1;
// Access for users to read-only
module_param(param, uint, 0444);
MODULE_PARM_DESC(param, " How many times to display 'Hello, world!'");

struct node {
	struct list_head list_node;
	ktime_t timestamp;
};

static LIST_HEAD(Head_Node);

static int __init hello_init(void)
{

	int i;

	BUG_ON(param > 10);

	if (param == 0 || (param >= 5 && param <= 10)) {
		pr_warn("Wrong number of hello.\n");
	}

	for (i = 0; i < param; i++) {
		struct node *linkedlist = kmalloc(sizeof(*linkedlist), GFP_KERNEL);

		// Forcing a bug
		if(i == 5)
			linkedlist = 0;

		linkedlist->timestamp = ktime_get();
		list_add(&linkedlist->list_node, &Head_Node);
		printk(KERN_EMERG "Hello, world!\n");
	}
	return 0;
}

static void __exit hello_exit(void)
{
	struct node *linkedlist, *tmp;

	list_for_each_entry_safe(linkedlist, tmp, &Head_Node, list_node) {
		ktime_t duration = ktime_sub(ktime_get(), linkedlist->timestamp);
        
		pr_info("Elapsed time: %lld\n", ktime_to_ns(duration));
		list_del(&linkedlist->list_node);
		kfree(linkedlist);
	}
}

module_init(hello_init);
module_exit(hello_exit);
