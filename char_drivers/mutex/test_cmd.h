#ifndef __TEST_CMD_H__
#define __TEST_CMD_H__


struct ioctl_data{
	unsigned int size;
	char buf[100];

};

#define TEST_MAGIC 'x'
#define TEST_MAX_NR 3


#define TEST_CLEAR  _IO(TEST_MAGIC,1)
#define TEST_OFFSET _IO(TEST_MAGIC,2)
//#define TEST_KBUF   _IO(TEST_MAGIC,3)
#define TEST_KBUF   _IOW(TEST_MAGIC,3,struct ioctl_data)

#endif
