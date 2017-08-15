#include <linux/init.h>
#include <linux/module.h>
#include <linux/pci.h>

MODULE_LICENSE("Dual BSD/GPL");

#define    OUR_SATA_VENDOR_ID    0x14e4
#define    OUR_SATA_PROD_ID    0x165f


void print_addr_func(u8 *src, int size) {
    int i;
    if (size < 0) {
        printk(KERN_ALERT "The size should be greater than 0!\n");
        return;
    }
    
    for(i = 0; i < size; i++) {
        if (! (i & 15))
            printk(KERN_ALERT " %02x:", i);
        printk(KERN_ALERT " %02x", src[i]);
        if ((i & 15) == 15)
            printk(KERN_ALERT "\n");
    }
}


static int aboutpci_init(void)
{
    u8 config_arr[256];
    //int iobase;
    //int iobase_end;
    int i;
    //u8 data_byte = 0;
    //u32 pio_start, pio_end, pio_flags, pio_len = 0;
    unsigned long mmio_start, mmio_end, mmio_flags, mmio_len, ioaddr;
    //u16 data_one_word;
    unsigned int *base_addr, *base_addr_0;

    struct pci_dev *pdev = NULL;

    //Finding the device by Vendor/Device ID Pair
    pdev = pci_get_device(OUR_SATA_VENDOR_ID, OUR_SATA_PROD_ID, pdev);
    if (pdev != NULL) {
        printk(KERN_ALERT "Our SATA HBA found!\n");
        if ( pdev->dma_mask == DMA_BIT_MASK(64) )
            printk(KERN_ALERT "64-bit addressing capable!\n");
        else if ( pdev->dma_mask == DMA_BIT_MASK(32) )
            printk(KERN_ALERT "32-bit addressing capable!\n");
        /* Bus-specific parameters. For a PCI NIC, it looks as follows */
        printk(KERN_ALERT "Use pci_read_config_byte() to print bytes in configuration space\n");
        for(i = 0; i < 256; i++) {
            pci_read_config_byte(pdev, i, &config_arr[i]);
            //printk(KERN_ALERT " %02X ", config_arr[i]);
        }
        print_addr_func(config_arr, 256);

        printk(KERN_ALERT "Use pci_resource_XXX() to access BAR 0\n");
        mmio_start = pci_resource_start (pdev, 0);
        mmio_end = pci_resource_end (pdev, 0);
        mmio_flags = pci_resource_flags (pdev, 0);
        mmio_len = pci_resource_len (pdev, 0);
        
        printk(KERN_ALERT "MMIO region size of BAR 1 is :%lu\n", mmio_len);
	printk(KERN_ALERT "MMIO region base addr is %x\n", mmio_start);

        /* make sure PCI base addr 1 is MMIO */
	if (!(mmio_flags & IORESOURCE_MEM)) {
	    printk(KERN_ALERT, "region #1 not an MMIO resource, aborting\n");
	}
        
        // Get BAR0's address
        /* ioremap MMIO region */
	ioaddr = ioremap(mmio_start, mmio_len);
	if (ioaddr == NULL) {
	    printk(KERN_ALERT "MMIO region is rrror!! \n");
	}   
	printk(KERN_ALERT "MMIO Remap addr is %x\n", ioaddr);
        // print out the MMIO region content from remap addr (virtual address)
        print_addr_func(ioaddr, 16 /* part of mmio_len */);
    }
    else
        printk(KERN_ALERT "Our SATA HBA Not found!\n");

    //Finding the device by its class code
    pdev = NULL;
    pdev = pci_get_class(PCI_CLASS_STORAGE_SATA_AHCI, pdev);
    if (pdev != NULL) {
        printk(KERN_ALERT "SATA HBA Class device found!\n");
        printk(KERN_ALERT "Device Vendor ID: 0x%X\n", pdev->vendor);
        printk(KERN_ALERT "Device Product ID: 0x%X\n", pdev->device);
     
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

