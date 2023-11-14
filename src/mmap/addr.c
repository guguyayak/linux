#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/statvfs.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <error.h>

#define BUF_ARRAY_LEN	1024
#define MMAP_BUF_SIZE	(512 * 1024)

typedef struct mmap_buf {
	int 	inited;
	void 	*buf_array[BUF_ARRAY_LEN];
} mmap_buf;

mmap_buf g_mmap_buf;

char buffer[MMAP_BUF_SIZE];

static int do_mmap()
{
	int rc		= 0;
	int i		= 0;
	int j		= 0;
	int mmap_fd	= -1;

	mmap_fd = open("/dev/dfsmap", O_RDWR);
	if (mmap_fd < 0)
	{
		printf("open dfsmap failed\n");
		return -1;
	}

	for (i = 0; i < BUF_ARRAY_LEN; i++)
	{
		void *data_buf = mmap(0, MMAP_BUF_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, mmap_fd, 0);
		if (data_buf == NULL)
		{
			rc = -1;
			goto mmap_fini;
		}
		else
		{
			memcpy(buffer, data_buf, MMAP_BUF_SIZE);
			g_mmap_buf.buf_array[i] = data_buf;
			buffer[MMAP_BUF_SIZE - 1] = '\0';
			printf("buffer:%s\n", buffer);
		}
	}
	
	g_mmap_buf.inited = 1;

	close(mmap_fd);
	return rc;

mmap_fini:
	for (j = 0; j < i; j++)
	{
		munmap(g_mmap_buf.buf_array[j], MMAP_BUF_SIZE);
	}
	close(mmap_fd);
        return rc;
}

int main()
{
	do_mmap();
	return 0;
}
