#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <sys/ioctl.h>
#include <linux/mxu_uapi.h>

#include "hpl.h"

#ifdef MPI_PCIE
//#define TRACE_TX_RX

#define CHECK_MPI_OPEN_FILE() \
   do { \
      if (hpl_mpi_open_file()) \
         return -1; \
   } while (0)


static FILE *g_mxu_pcie;

#ifdef TRACE_TX_RX
static uint32_t g_mpi_send_count;
static uint32_t g_mpi_recv_count;
#endif

static void fill_mpi_status(MPI_Status *mpi_status,
                            const struct mxu_mpi_status *mxu_status)
{
   mpi_status->MPI_SOURCE = mxu_status->mpi_source;
   mpi_status->MPI_TAG = mxu_status->mpi_tag;
   mpi_status->MPI_ERROR = mxu_status->mpi_error;
   mpi_status->_ucount = mxu_status->count;
}

int hpl_mpi_open_file(void)
{
   if (!g_mxu_pcie)
      g_mxu_pcie = fopen("/dev/mxu-pcie", "r");

   if (g_mxu_pcie == NULL) {
      printf("Failed to open /dev/mxu-pcie\n");
      return -1;
   }

   return 0;
}

int HPL_MPI_Init(int *argc, char ***argv)
{
   CHECK_MPI_OPEN_FILE();

   return MPI_Init(argc, argv);
}

int HPL_MPI_Finalize(void)
{
   CHECK_MPI_OPEN_FILE();

   return MPI_Finalize();
}

int HPL_MPI_Comm_free(MPI_Comm *comm)
{
   CHECK_MPI_OPEN_FILE();

   return MPI_Comm_free(comm);
}

int HPL_MPI_Abort(MPI_Comm comm, int errorcode)
{
   CHECK_MPI_OPEN_FILE();

   printf("%s\n", __func__);
   return MPI_Abort(comm, errorcode);
}

int HPL_MPI_Iprobe(int source, int tag, MPI_Comm comm, int *flag,
                   MPI_Status *status)
{
   struct mxu_mpi_iprobe mpi_iprobe = {0};
   int ret;

   CHECK_MPI_OPEN_FILE();

   mpi_iprobe.source = source;
   mpi_iprobe.tag = tag;
   mpi_iprobe.comm = (uintptr_t)comm;
   mpi_iprobe.flag = 0;

   ret = ioctl(fileno(g_mxu_pcie), MXU_MPI_IPROBE, &mpi_iprobe);
   if (!ret) {
      *flag = mpi_iprobe.flag;
      fill_mpi_status(status, &mpi_iprobe.status);
      ret = MPI_SUCCESS;
   } else {
      printf("MXU_MPI_IPROBE failed\n");
      ret = MPI_ERR_COMM;
   }

   return ret;
   //return MPI_Iprobe(source, tag, comm, flag, status);
}

int HPL_MPI_Comm_rank(MPI_Comm comm, int *rank)
{
   struct mxu_mpi_comm_rank mpi_comm_rank = {0};
   int ret;

   CHECK_MPI_OPEN_FILE();

   mpi_comm_rank.comm = (uintptr_t)comm;
   ret = ioctl(fileno(g_mxu_pcie), MXU_MPI_COMM_RANK, &mpi_comm_rank);

   if (!ret) {
      *rank = mpi_comm_rank.rank;
      ret = MPI_SUCCESS;
   } else {
      printf("MXU_MPI_COMM_RANK failed\n");
      ret = MPI_ERR_COMM;
   }

   return ret;

   //return MPI_Comm_rank(comm, rank);
}

int HPL_MPI_Comm_size(MPI_Comm comm, int *size)
{
   struct mxu_mpi_comm_size mpi_comm_size = {0};
   int ret;

   CHECK_MPI_OPEN_FILE();

   mpi_comm_size.comm = (uintptr_t)comm;
   ret = ioctl(fileno(g_mxu_pcie), MXU_MPI_COMM_SIZE, &mpi_comm_size);
   if (!ret) {
      *size = mpi_comm_size.size;
      ret = MPI_SUCCESS;
   } else {
      printf("MXU_MPI_COMM_SIZE failed\n");
      ret = MPI_ERR_COMM;
   }

   return ret;

   //return MPI_Comm_size(comm, size);
}

int HPL_MPI_Comm_split(MPI_Comm comm, int color, int key, MPI_Comm *newcomm)
{
   CHECK_MPI_OPEN_FILE();

   return MPI_Comm_split(comm, color, key, newcomm);
}

static void hex_dump(char *header, void *buf, int len)
{
   int i;
   unsigned char *p = buf;

   printf("%s: ", header ? header : "");
   for (i = 0; i < len; i++) {
      printf("%02x ", p[i]);

      if (i % 16 == 15 && i != len - 1)
         printf("\n%s: ", header ? header : "");
   }

   printf("\n");
}

int HPL_MPI_Send(const void *buf, int count, MPI_Datatype datatype, int dest,
                 int tag, MPI_Comm comm)
{
   struct mxu_mpi_send mpi_send = {0};
   int ret = MPI_SUCCESS;
   int type_size = 1;
   uint32_t buf_size;

   CHECK_MPI_OPEN_FILE();

   MPI_Type_size(datatype, &type_size);
   buf_size = count * type_size;

   mpi_send.buf = (uintptr_t)buf;
   mpi_send.count = buf_size;
   mpi_send.data_type = (uintptr_t)datatype;
   mpi_send.dest = dest;
   mpi_send.tag = tag;
   mpi_send.comm = (uintptr_t)comm;
   mpi_send.timeout_ms = 1000000;

#ifdef TRACE_TX_RX
   printf("%s(%d): %d(%d * %d) bytes, dest=%d, tag=0x%x\n",
          __func__, g_mpi_send_count, buf_size, count, type_size, dest, tag);
#endif

   if (ioctl(fileno(g_mxu_pcie), MXU_MPI_SEND, &mpi_send)) {
      printf("MXU_MPI_SEND failed\n");
      ret = MPI_ERR_COMM;
   }

#ifdef TRACE_TX_RX
   g_mpi_send_count++;
#endif

   return ret;
}

int HPL_MPI_Ssend(const void *buf, int count, MPI_Datatype datatype, int dest,
                  int tag, MPI_Comm comm)
{
   CHECK_MPI_OPEN_FILE();

   printf("%s\n", __func__);
   return MPI_Ssend(buf, count, datatype, dest, tag, comm);
}

int HPL_MPI_Issend(const void *buf, int count, MPI_Datatype datatype, int dest,
                   int tag, MPI_Comm comm, MPI_Request *request)
{
   CHECK_MPI_OPEN_FILE();

   printf("%s\n", __func__);
   return MPI_Issend(buf, count, datatype, dest, tag, comm, request);
}

int HPL_MPI_Irecv(void *buf, int count, MPI_Datatype datatype, int source,
                  int tag, MPI_Comm comm, MPI_Request *request)
{
   CHECK_MPI_OPEN_FILE();

   printf("%s\n", __func__);
   return MPI_Irecv(buf, count, datatype, source, tag, comm, request);
}

int HPL_MPI_Recv(void *buf, int count, MPI_Datatype datatype, int source,
                 int tag, MPI_Comm comm, MPI_Status *status)
{
   struct mxu_mpi_recv mpi_recv = {0};
   int ret = MPI_SUCCESS;
   int type_size = 1;
   uint32_t buf_size;

   MPI_Type_size(datatype, &type_size);
   buf_size = count * type_size;

   CHECK_MPI_OPEN_FILE();

   mpi_recv.buf = (uintptr_t)buf;
   mpi_recv.count = buf_size;
   mpi_recv.data_type = (uintptr_t)datatype;
   mpi_recv.source = source;
   mpi_recv.tag = tag;
   mpi_recv.comm = (uintptr_t)comm;
   mpi_recv.timeout_ms = 1000000;

#ifdef TRACE_TX_RX
   printf("%s(%d): %d(%d * %d) bytes, source=%d, tag=0x%x\n",
          __func__, g_mpi_recv_count, buf_size, count, type_size, source, tag);
#endif

   if (ioctl(fileno(g_mxu_pcie), MXU_MPI_RECV, &mpi_recv)) {
#ifdef TRACE_TX_RX
      printf("MXU_MPI_RECV(%d) failed\n", g_mpi_recv_count);
#endif
      ret = MPI_ERR_COMM;
   } else {
      if (mpi_recv.count != buf_size) {
#ifdef TRACE_TX_RX
         printf("MXU_MPI_RECV(%d): received %d bytes, expected %d bytes\n",
                g_mpi_recv_count, mpi_recv.count, buf_size);
#else
         printf("MXU_MPI_RECV: received %d bytes, expected %d bytes\n",
                mpi_recv.count, buf_size);
#endif
         ret = MPI_ERR_TRUNCATE;
      }

      fill_mpi_status(status, &mpi_recv.status);
      ret = MPI_SUCCESS;
   }

#ifdef TRACE_TX_RX
   g_mpi_recv_count++;
#endif

   return ret;
}

int HPL_MPI_Type_create_struct(int count, const int array_of_block_lengths[],
                               const MPI_Aint array_of_displacements[],
                               const MPI_Datatype array_of_types[],
                               MPI_Datatype *newtype)
{
   CHECK_MPI_OPEN_FILE();

   return MPI_Type_create_struct(count, array_of_block_lengths,
                                 array_of_displacements, array_of_types,
                                 newtype);
}

int HPL_MPI_Type_vector(int count, int blocklength, int stride,
                        MPI_Datatype oldtype, MPI_Datatype *newtype)
{
   CHECK_MPI_OPEN_FILE();

   printf("%s\n", __func__);
   return MPI_Type_vector(count, blocklength, stride, oldtype, newtype);
}

int HPL_MPI_Type_commit(MPI_Datatype *type)
{
   CHECK_MPI_OPEN_FILE();

   return MPI_Type_commit(type);
}

int HPL_MPI_Type_contiguous(int count, MPI_Datatype oldtype,
                                       MPI_Datatype *newtype)
{
   CHECK_MPI_OPEN_FILE();

   return MPI_Type_contiguous(count, oldtype, newtype);
}

int HPL_MPI_Type_free(MPI_Datatype *type)
{
   CHECK_MPI_OPEN_FILE();

   return MPI_Type_free(type);
}

int HPL_MPI_Wait(MPI_Request *request, MPI_Status *status)
{
   CHECK_MPI_OPEN_FILE();

   printf("%s\n", __func__);
   return MPI_Wait(request, status);
}

int HPL_MPI_Get_address(const void *location, MPI_Aint *address)
{
   CHECK_MPI_OPEN_FILE();

   return MPI_Get_address(location, address);
}

double HPL_MPI_Wtime(void)
{
   CHECK_MPI_OPEN_FILE();

   return MPI_Wtime();
}

#else

int HPL_MPI_Init(int *argc, char ***argv)
{
   return MPI_Init(argc, argv);
}

int HPL_MPI_Finalize(void)
{
   return MPI_Finalize();
}

int HPL_MPI_Comm_free(MPI_Comm *comm)
{
   return MPI_Comm_free(comm);
}

int HPL_MPI_Abort(MPI_Comm comm, int errorcode)
{
   return MPI_Abort(comm, errorcode);
}

int HPL_MPI_Iprobe(int source, int tag, MPI_Comm comm, int *flag,
                    MPI_Status *status)
{
   return MPI_Iprobe(source, tag, comm, flag, status);
}

int HPL_MPI_Comm_rank(MPI_Comm comm, int *rank)
{
   return MPI_Comm_rank(comm, rank);
}

int HPL_MPI_Comm_size(MPI_Comm comm, int *size)
{
   return MPI_Comm_size(comm, size);
}

int HPL_MPI_Comm_split(MPI_Comm comm, int color, int key, MPI_Comm *newcomm)
{
   return MPI_Comm_split(comm, color, key, newcomm);
}

int HPL_MPI_Send(const void *buf, int count, MPI_Datatype datatype, int dest,
                 int tag, MPI_Comm comm)
{
   return MPI_Send(buf, count, datatype, dest, tag, comm);
}

int HPL_MPI_Ssend(const void *buf, int count, MPI_Datatype datatype, int dest,
                  int tag, MPI_Comm comm)
{
   return MPI_Ssend(buf, count, datatype, dest, tag, comm);
}

int HPL_MPI_Issend(const void *buf, int count, MPI_Datatype datatype, int dest,
                   int tag, MPI_Comm comm, MPI_Request *request)
{
   return MPI_Issend(buf, count, datatype, dest, tag, comm, request);
}

int HPL_MPI_Irecv(void *buf, int count, MPI_Datatype datatype, int source,
                  int tag, MPI_Comm comm, MPI_Request *request)
{
   return MPI_Irecv(buf, count, datatype, source, tag, comm, request);
}

int HPL_MPI_Recv(void *buf, int count, MPI_Datatype datatype, int source,
                 int tag, MPI_Comm comm, MPI_Status *status)
{
   return MPI_Recv(buf, count, datatype, source, tag, comm, status);
}


int HPL_MPI_Type_create_struct(int count, const int array_of_block_lengths[],
                               const MPI_Aint array_of_displacements[],
                               const MPI_Datatype array_of_types[],
                               MPI_Datatype *newtype)
{
   return MPI_Type_create_struct(count, array_of_block_lengths,
                                 array_of_displacements, array_of_types,
                                 newtype);
}

int HPL_MPI_Type_vector(int count, int blocklength, int stride,
                        MPI_Datatype oldtype, MPI_Datatype *newtype)
{
   return MPI_Type_vector(count, blocklength, stride, oldtype, newtype);
}

int HPL_MPI_Type_commit(MPI_Datatype *type)
{
   return MPI_Type_commit(type);
}

int HPL_MPI_Type_contiguous(int count, MPI_Datatype oldtype,
                                       MPI_Datatype *newtype)
{
   return MPI_Type_contiguous(count, oldtype, newtype);
}

int HPL_MPI_Type_free(MPI_Datatype *type)
{
   return MPI_Type_free(type);
}

int HPL_MPI_Wait(MPI_Request *request, MPI_Status *status)
{
   return MPI_Wait(request, status);
}

int HPL_MPI_Get_address(const void *location, MPI_Aint *address)
{
   return MPI_Get_address(location, address);
}

double HPL_MPI_Wtime(void)
{
   return MPI_Wtime();
}
#endif

