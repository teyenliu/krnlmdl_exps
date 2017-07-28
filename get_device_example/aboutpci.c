#include <linux/init.h>
#include <linux/module.h>
#include <linux/pci.h>

MODULE_LICENSE("Dual BSD/GPL");

#define    OUR_SATA_VENDOR_ID    0x8086
#define    OUR_SATA_PROD_ID    0x2829

static int aboutpci_init(void)
{
    //int iobase;
    //int iobase_end;
    int i;
    u8 data_byte = 0;
    //u16 data_one_word;

    struct pci_dev *dev = NULL;

    //Finding the device by Vendor/Device ID Pair
    dev = pci_get_device(OUR_SATA_VENDOR_ID, OUR_SATA_PROD_ID, dev);
    if (dev != NULL) {
        printk(KERN_ALERT "Our SATA HBA found!\n");
        if ( dev->dma_mask == DMA_BIT_MASK(64) )
            printk(KERN_ALERT "64-bit addressing capable!\n");
        else if ( dev->dma_mask == DMA_BIT_MASK(32) )
            printk(KERN_ALERT "32-bit addressing capable!\n");
        /* Bus-specific parameters. For a PCI NIC, it looks as follows */
        printk(KERN_ALERT "Use pci_read_config_byte() to print bytes in configuration space");
        for(i = 0; i < 256; i++) {
            pci_read_config_byte(dev, i, &data_byte);
            printk(KERN_ALERT " %X ", data_byte);
        }
    }
    else
        printk(KERN_ALERT "Our SATA HBA Not found!\n");

    //Finding the device by its class code
    dev = NULL;
    dev = pci_get_class(PCI_CLASS_STORAGE_SATA_AHCI, dev);
    if (dev != NULL) {
        printk(KERN_ALERT "SATA HBA Class device found!\n");
        printk(KERN_ALERT "Device Vendor ID: 0x%X\n", dev->vendor);
        printk(KERN_ALERT "Device Product ID: 0x%X\n", dev->device);
     
       /* Bus-specific parameters. For a PCI NIC, it looks as follows */
       //iobase = pci_resource_start(dev, 1);
       //iobase_end = iobase + pci_resource_len(dev, 1);
       //printk(KERN_ALERT "Device class bar0 from: 0x%X to 0x%X\n", iobase, iobase_end);
    }
    else
        printk(KERN_ALERT "SATA HBA Class device Not found!\n");

    return 0;
}

static void aboutpci_exit(void)
{
    printk(KERN_ALERT "Goodbye, pci hackers\n");
}

module_init(aboutpci_init);
module_exit(aboutpci_exit);

