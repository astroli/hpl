#ifndef HPL_MXU_H
#define HPL_MXU_H

/*
 * ---------------------------------------------------------------------
 * Include files
 * ---------------------------------------------------------------------
 */

/*
 * ---------------------------------------------------------------------
 * #define macro constants
 * ---------------------------------------------------------------------
 */

/*
 * ---------------------------------------------------------------------
 * Function prototypes
 * ---------------------------------------------------------------------
 */
#ifdef HPL_CALL_MXU
#endif

#ifdef HPL_MXU_PROFILING
void mxu_start_insn_cnt(void);
void mxu_reset_insn_cnt(void);
void mxu_stop_insn_cnt(void);
void mxu_show_insn_cnt(void);
void mxu_select_default_insn_cnt(void);
void mxu_select_pdfact_insn_cnt(void);
void mxu_select_pdupdate_insn_cnt(void);
void mxu_select_pdupdate_dgemm_insn_cnt(void);
void mxu_select_pdupdate_dtrsm_insn_cnt(void);
#endif

#ifdef HPL_MXU_SAVE_DATA
void mxu_dump_matrix(char *header, double *data, int row, int col, int ld);
int mxu_check_coeff_mat(double *data, int N, int NB, int row, int col, int ld);
void mxu_save_coeff_mat(double *data, int N, int NB, int row, int col, int ld);
void mxu_save_prev_pvt(double *data, int N, int NB, int row, int col, int ld, int jj);
void mxu_save_post_pvt(double *data, int N, int NB, int row, int col, int ld, int jj);
void mxu_save_post_pvt_l1(double *data, int N, int NB, int jb, int jj);
void mxu_save_prev_dger(double *data, int N, int NB, int row, int col, int ld, int jj);
void mxu_save_post_dger(double *data, int N, int NB, int row, int col, int ld, int jj);
void mxu_save_post_dscale(double *data, int N, int NB, int row, int col, int ld, int jj);
void mxu_save_post_daxpy(double *data, int N, int NB, int row, int col, int ld, int jj);
void mxu_save_ipiv(int *data, int N, int NB, int jb, int ii, int jj);
void mxu_save_prev_dlaswp00N(double *data, int N, int NB, int row, int col, int ld, int ii, int jj);
void mxu_save_post_dlaswp00N(double *data, int N, int NB, int row, int col, int ld, int ii, int jj);
void mxu_save_prev_dtrsm_l1(double *data, int N, int NB, int jb, int ii, int jj);
void mxu_save_post_dtrsm(double *data, int N, int NB, int row, int col, int ld, int ii, int jj);
void mxu_save_prev_dgemm(double *data, int N, int NB, int row, int col, int ld, int ii, int jj);
void mxu_save_prev_dgemm_l2(double *data, int N, int NB, int row, int col, int ld, int ii, int jj);
void mxu_save_post_dgemm(double *data, int N, int NB, int row, int col, int ld, int ii, int jj);
void mxu_save_prev_dtrsv(double *data, int N, int NB, int row, int col, int ld, int anp);
void mxu_save_prev_dtrsv_x(double *data, int N, int NB, int row, int anp);
void mxu_save_post_dtrsv(double *data, int N, int NB, int row, int col, int ld, int anp);
void mxu_save_post_dtrsv_x(double *data, int N, int NB, int row, int anp);
void mxu_save_post_dcopy_x(double *data, int N, int NB, int row);
void mxu_save_prev_dgemv(double *data, int N, int NB, int row, int col, int ld);
void mxu_save_prev_dgemv_x(double *data, int N, int NB, int row);
void mxu_save_post_dgemv(double *data, int N, int NB, int row, int col, int ld);
void mxu_save_post_dgemv_x(double *data, int N, int NB, int row);

#endif

#endif
/*
 * End of hpl_mxu.h
 */
