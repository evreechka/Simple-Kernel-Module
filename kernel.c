#include <linux/module.h> // included for all kernel modules
#include <linux/kernel.h> // included for KERN_INFO
#include <linux/init.h> // included for __init and __exit macros
#include <linux/mount.h>
#include <linux/path.h>
#include <linux/namei.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/dcache.h>
#include <linux/debugfs.h>
#include <linux/seq_file.h>

#define BUF_SIZE 1024

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A simple module of getting DENTRY and ADDRESS_SPACE structures");
MODULE_AUTHOR("Mari Mastyukina");
MODULE_VERSION("1.0");

struct dentry *kmod_root;
struct path current_path;
struct dentry * info_file_struct;
struct dentry * dentry_saved_struct;
struct dentry * as_saved_struct;
struct debugfs_blob_wrapper dentry_wrapper;
struct debugfs_blob_wrapper as_wrapper;
char buffer[BUF_SIZE];
char * buffer_ptr;
ssize_t file_write_operation(struct file *file, const char __user *userbuf, size_t length, loff_t *offset) {
	printk(KERN_INFO "Start write the params\n");
	char path_name[BUF_SIZE];
	int read_char_count = 0;
	int i = 0;
	for (i = 0; i < length && i < BUF_SIZE; i++) {
		get_user(buffer[i], userbuf + i);
	}
	printk(KERN_INFO "Start write path");
	read_char_count = sscanf(buffer, "%s", path_name);
	printk(KERN_INFO "End write path");
	if (path_name == NULL) {
		printk(KERN_WARNING "Error path name\n");
		return length;
	} 
	printk("Path: %s", path_name);
	int status = kern_path(path_name, LOOKUP_PARENT, &current_path);
	if (status) {
		printk(KERN_INFO "Error to find path %s\n", path_name);
		return length;
	}
	dentry_wrapper.data = (void*)current_path.dentry;
	dentry_wrapper.size = sizeof(struct dentry);
	dentry_saved_struct = debugfs_create_blob("dentry_info", 0664, kmod_root, &dentry_wrapper);

	as_wrapper.data = (void*)current_path.dentry->d_inode->i_mapping;
	as_wrapper.size = sizeof(struct address_space);
	as_saved_struct = debugfs_create_blob("address_space_info", 0664, kmod_root, &as_wrapper);

	printk("DENTRY:\n");
	printk("Dentry name: %s\n", current_path.dentry->d_name.name);
	printk("Dentry parent name: %s\n", current_path.dentry->d_parent->d_name.name);
	printk("Dentry inode UID: %d\n", current_path.dentry->d_inode->i_uid);
	printk("Dentry inode GID: %d\n", current_path.dentry->d_inode->i_gid);
	printk("Dentry element flags: %d\n", current_path.dentry->d_flags);	
	printk("ADDRESS_SPACE:\n");
	printk("Address space pages: %d\n", current_path.dentry->d_inode->i_mapping->nrpages);
	printk("Address space flags: %d\n", current_path.dentry->d_inode->i_mapping->flags);
	printk("Address space host: %d\n", current_path.dentry->d_inode->i_mapping->host->i_uid);
	printk("Count od memory area with flag VM_SHARED: %d\n", current_path.dentry->d_inode->i_mapping->i_mmap_writable);
	return i;
}
ssize_t file_read_operation(struct file *file, char __user *userbuf, size_t length, loff_t *offset){
	printk(KERN_INFO "Start get the params\n");
	buffer_ptr = buffer;
	int read_bytes_count = 0;

	if (!*(buffer_ptr + *offset)) {
		*offset = 0;
		return 0;
	}
    buffer_ptr += *offset;
    while(length && *buffer_ptr) {
    	put_user(*(buffer_ptr++), userbuf++);
    	length--;
    	read_bytes_count++;
    }
    *offset += read_bytes_count;
    return read_bytes_count;
}
static struct file_operations fops = {
    .read = file_read_operation,
    .write = file_write_operation
};

static int __init lab2_init(void) {
	printk(KERN_INFO "Init module\n");
	kmod_root = debugfs_create_dir("lab2_OS_2", NULL);
	
	info_file_struct = debugfs_create_file("info", 0644, kmod_root, NULL, &fops);
	return 0;
}

static void __exit lab2_exit(void) {
	debugfs_remove_recursive(kmod_root);
	printk(KERN_INFO "End of module work\n");
}

module_init(lab2_init);
module_exit(lab2_exit);