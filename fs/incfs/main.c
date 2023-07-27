// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright 2018 Google LLC
 */
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>

#include <uapi/linux/incrementalfs.h>

#include "sysfs.h"
#include "vfs.h"

static struct file_system_type incfs_fs_type = {
	.owner = THIS_MODULE,
	.name = INCFS_NAME,
	.mount = incfs_mount_fs,
	.kill_sb = incfs_kill_sb,
	.fs_flags = 0
};


static ssize_t bugfix_inode_eviction_show(struct kobject *kobj,
			 struct kobj_attribute *attr, char *buff)
{
	return snprintf(buff, PAGE_SIZE, "supported\n");
}

static struct kobj_attribute bugfix_inode_eviction_attr =
	__ATTR_RO(bugfix_inode_eviction);

static int __init init_incfs_module(void)
{
	int err = 0;

	err = incfs_init_sysfs();
	if (err)
		return err;

	err = register_filesystem(&incfs_fs_type);
	if (err)
		incfs_cleanup_sysfs();

	return err;
}

static void __exit cleanup_incfs_module(void)
{
	incfs_cleanup_sysfs();
	unregister_filesystem(&incfs_fs_type);
}

module_init(init_incfs_module);
module_exit(cleanup_incfs_module);

MODULE_LICENSE("GPL v2");
MODULE_IMPORT_NS(VFS_internal_I_am_really_a_filesystem_and_am_NOT_a_driver);
MODULE_AUTHOR("Eugene Zemtsov <ezemtsov@google.com>");
MODULE_DESCRIPTION("Incremental File System");
