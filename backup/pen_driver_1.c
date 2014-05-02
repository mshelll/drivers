#include <linux/module.h>
#include <linux/kernel.h>  
#include <linux/usb.h>

#define EP_OUT 0x01
#define EP_IN  0x82
#define MAX_PKT_SIZE 512 

struct usb_device *device;
struct usb_class_driver class;
static unsigned char bulk_buf[MAX_PKT_SIZE];

static int pen_open(struct inode *i, struct file *f)
{
  return 0;;
}

static int pen_close(struct inode *i, struct file *f)
{
  return 0;
}

static ssize_t pen_read(struct file *f, char __user *buf, size_t cnt, loff_t *off)
{
  retval=usb_bulk_msg(device, usb_rvcbulkpipe(device, BULK_EP_IN), bulk_buf, MAX_PKT_SIZE, &read_cnt, 5000);
  if(retval)
  {
    printk(KERN_ERR"bulk message read returned :%d",retval);
    return retval;
  }
  if(copy_to_user( buf, bulk_buf, MIN(cnt, read_cnt) ) )
  {
    return -EFAULT;
  }
  return MIN(cnt, read_cnt);
}

static ssize_t pen_write(struct file *f, const char __user *buf, size_t cnt, loff_t *off)
{
  int retval;
  int write_cnt = MIN(cnt, MAX_PKT_SIZE) ;
  if(copy_from_user( bulk_buf, buf, MIN(cnt, MAX_PKT_SIZE) ) )
  {
    return -EFAULT
  }
  
  retval=usb_bulk_msg(device, usb_sndbulkpipe(device, BULK_EP_OUT), bulk_buf, MIN(cnt, MAX_PKT_SIZE), &write_cnt, 5000);
  if(retval)
  {
    printk(KERN_ERR"bulk message write returned :%d",retval);
    return retval;
  }
  
  return write_cnt;
}

static struct file_operations fops = 
{
  .open=pen_open;
  .close=pen_close;
  .read=pen_read;
  .write=pen_write
};
// usb-core will call probe for each interface of the detected device

static int pen_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
  int retval;
  struct usb_host_interface *iface_desc;
  struct usb_endpoint_descriptor *endpoint;
  int i;
  
  iface_desc = interface->cur_altsetting;
  printk(KERN_INFO"Pen i/f %d now probed \n",iface_desc->desc.bInterfaceNumber);
  printk(KERN_INFO" Vendor :%04X\n Product :%04X\n",id->idVendor, id->idProduct);
  printk(KERN_INFO" Number of End Points   :%02X\n",iface_desc->desc.bNumEndpoints);
  printk(KERN_INFO" Interface Class        :%02X\n",iface_desc->desc.bInterfaceClass);
  
  for(i=0; i<iface_desc->desc.bNumEndpoints; i++)
   {
    endpoint = &iface_desc->endpoint[i].desc;
    printk(KERN_INFO"\n\n   End point          :%d",i);
    printk(KERN_INFO"\n		End point Address  :%02X",endpoint->bEndpointAddress);
    printk(KERN_INFO"\n		End point Atributes:%02X",endpoint->bmAttributes);
    printk(KERN_INFO"\n		End point pcketsize:%d\n",endpoint->wMaxPacketSize);
  
   }
   
   device = interface_to_usbdev(interface);
   
   class.name = "usb/pendr";
   class.fops = &fops;
   if((retval = usb_register_dev( interface, &class ) ) < 0 )
   {
     printk(KERN_INFO"\nerror in device registering") ;
   }
   else
   {
     printk(KERN_INFO"\nMinor obtained: %d", interface->minor);
   }
   return 0;  // usb-core will call probe for each interface of the detected device, return of 0 means the interface has been registered
}  

static void pen_disconnect(struct usb_interface *interface)
{
  printk(KERN_INFO"\n\nPen drive interface removed : %d\n\n",interface->cur_altsetting->desc.bInterfaceNumber);
}

static struct usb_device_id pen_table[] =
{
 /*
 mechanism for a driver to specify its device to the USB core.
 We are creating a table of type struct usb_device_id....by specifying <vendor id, product id> pair using the USB_DEVICE() macro
 There are many alternative macros for USB_DEVICE()...and sometimes more than one is used....to link with USB Core....
 So that single driver can be used for multiple devices....
 */
 {USB_DEVICE(0x058F, 0x6387) },
 {}
};

MODULE_DEVICE_TABLE(usb, pen_table);
/*
Module is another name for the device driver.This guy generates 2 variables which is extracted by user space depmod
and put globally in /lib/modules/<filename>

This enables the automatic loading of modules when plugged
*/

static struct usb_driver pen_driver =
{
   .name = "pen_driver",
   .probe = pen_probe,
   .id_table =pen_table,
   .disconnect =pen_disconnect,
};

static int __init pen_init(void)
{
  return usb_register(&pen_driver);
}

static void __exit pen_exit(void)
{
  usb_deregister(&pen_driver);
}
module_init(pen_init);
module_exit(pen_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Anil Kumar Pugalia <email_at_sarika-pugs_dot_com>");
MODULE_DESCRIPTION("USB Pen Registration Driver");
