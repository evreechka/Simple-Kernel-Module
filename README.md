# Simple-Kernel-Module
A kernel-side program collects information about the dentry and address_space structures and passes it to the user level

**Task**

Develop a set of user-level and yard-level programs that collects information on the kernel side and transmits the information to the user level, and outputs it in a human-readable form. 

**Requrements**

1. The user-level program takes the command line argument(s) as input to identify the required path to the target structure from the system tables, transfers to the kernel level, gets information from that structure, and prints the structure to the standard output. 
2. The loaded kernel module accepts the request through the interface specified in the task, determines the path to the target structure according to the passed request and returns the result to the user level.
3. The transfer interface between the user program and the kernel: *debugfs* - debug file system /sys/kernel/debug, passing parameters via writing to a file.
4. Target structures: *dentry* and *address_space*

**Launch**

1. call make
2. sudo insmod *.ko
3. sudo ./user \<path_name\>
4. sudo rmmod *.ko
  
**Example**

![Example](https://vk.com/doc460964428_623332939?hash=52da7a86e5f03f2d02&dl=5fac65266142135c87)
