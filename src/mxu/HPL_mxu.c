#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define MXU_BASE_ADDR                    0x10002000
#define MXU_START_CNT_ADDR               (MXU_BASE_ADDR + 0x80)
#define MXU_RESET_CNT_ADDR               (MXU_BASE_ADDR + 0x84)
#define MXU_STOP_CNT_ADDR                (MXU_BASE_ADDR + 0x88)
#define MXU_SHOW_CNT_ADDR                (MXU_BASE_ADDR + 0x8c)
#define MXU_SEL_DEFAULT_CNT_ADDR         (MXU_BASE_ADDR + 0x90)
#define MXU_SEL_PDFACT_CNT_ADDR          (MXU_BASE_ADDR + 0x94)
#define MXU_SEL_PDUPDATE_CNT_ADDR        (MXU_BASE_ADDR + 0x98)
#define MXU_SEL_PDUPDATE_DGEMM_CNT_ADDR  (MXU_BASE_ADDR + 0x9c)
#define MXU_SEL_PDUPDATE_DTRSM_CNT_ADDR  (MXU_BASE_ADDR + 0xa0)

#define MAP_SIZE (1UL << 12)    // 4k page
#define MAP_MASK (MAP_SIZE - 1)

static inline void writel(uint32_t addr, uint32_t val)
{
	int memfd;
	void *mapped_base;
	void *mapped_dev_base;

	memfd = open("/dev/mem", O_RDWR | O_SYNC);
	if (memfd < 0) {
		printf("open /dev/mem fail, errno=%s\n", strerror(errno));
		return;
	}

	mapped_base = mmap(0, MAP_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, memfd, addr & ~MAP_MASK);
	if (mapped_base == MAP_FAILED) {
		printf("mmap fail, addr=0x%x, map_addr=0x%lx, errno=%s\n", addr, addr & ~MAP_MASK, strerror(errno));
		close(memfd);
		return;
	}
	mapped_dev_base = mapped_base + (addr & MAP_MASK);

	*((volatile uint32_t *)(uintptr_t)mapped_dev_base) = val;

	munmap(mapped_base, MAP_SIZE);
	close(memfd);
}

void mxu_start_insn_cnt(void)
{
   //printf("start counter\n");
   writel(MXU_START_CNT_ADDR, 1);
}

void mxu_reset_insn_cnt(void)
{
   //printf("reset counter\n");
   writel(MXU_RESET_CNT_ADDR, 1);
}

void mxu_stop_insn_cnt(void)
{
   //printf("stop counter\n");
   writel(MXU_STOP_CNT_ADDR, 1);
}

void mxu_show_insn_cnt(void)
{
   //printf("show counter\n");
   writel(MXU_SHOW_CNT_ADDR, 1);
}
void mxu_select_default_insn_cnt(void)
{
   //printf("select cnt default\n");
   writel(MXU_SEL_DEFAULT_CNT_ADDR, 1);
}

void mxu_select_pdfact_insn_cnt(void)
{
   //printf("select cnt pdfact\n");
   writel(MXU_SEL_PDFACT_CNT_ADDR, 1);
}

void mxu_select_pdupdate_insn_cnt(void)
{
   //printf("select cnt pdupdate\n");
   writel(MXU_SEL_PDUPDATE_CNT_ADDR, 1);
}

void mxu_select_pdupdate_dgemm_insn_cnt(void)
{
   writel(MXU_SEL_PDUPDATE_DGEMM_CNT_ADDR, 1);
}

void mxu_select_pdupdate_dtrsm_insn_cnt(void)
{
   //printf("select cnt pdupdate(dtrsm)\n");
   writel(MXU_SEL_PDUPDATE_DTRSM_CNT_ADDR, 1);
}

