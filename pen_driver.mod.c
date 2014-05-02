#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x35ec255d, "module_layout" },
	{ 0x985f683d, "usb_deregister" },
	{ 0x4f99f09f, "usb_register_driver" },
	{ 0x362ef408, "_copy_from_user" },
	{ 0x2f287f0d, "copy_to_user" },
	{ 0x89b5c527, "usb_bulk_msg" },
	{ 0x50eedeb8, "printk" },
	{ 0xa07534a5, "usb_register_dev" },
	{ 0xe1fe0b58, "usb_deregister_dev" },
	{ 0xb4390f9a, "mcount" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("usb:v058Fp6387d*dc*dsc*dp*ic*isc*ip*");

MODULE_INFO(srcversion, "CF750C82514A68403A22B83");
