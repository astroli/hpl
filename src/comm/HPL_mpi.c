#include "hpl.h"

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
