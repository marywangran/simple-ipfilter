#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/inet.h>

static struct nf_hook_ops nfho;

#define BLOCK_IP "8.8.8.8"

unsigned int simple_filter_hook(void *priv, struct sk_buff *skb, const struct nf_hook_state *state)
{
    struct iphdr *ip_header = ip_hdr(skb);

    if (ip_header->daddr == in_aton(BLOCK_IP)) 
        return NF_DROP;

    return NF_ACCEPT;
}

static int __init simple_filter_init(void)
{
    nfho.hook = simple_filter_hook;
    nfho.hooknum = NF_INET_LOCAL_OUT;
    nfho.pf = PF_INET;
    nfho.priority = NF_IP_PRI_FIRST;

    nf_register_net_hook(&init_net, &nfho);

    return 0;
}

static void __exit simple_filter_exit(void)
{
    nf_unregister_net_hook(&init_net, &nfho);
}

module_init(simple_filter_init);
module_exit(simple_filter_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("JingLi");
