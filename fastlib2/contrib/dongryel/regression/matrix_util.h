#ifndef MATRIX_UTIL_H
#define MATRIX_UTIL_H

#include "fastlib/fastlib.h"

class MatrixUtil {

  public:

    static double L1Norm(const Matrix &m_mat) {

      double l1_norm = 0;
      for(index_t j = 0; j < m_mat.n_cols(); j++) {
	const double *m_mat_column = m_mat.GetColumnPtr(j);
	double tmp_l1_norm = 0;

	for(index_t i = 0; i < m_mat.n_rows(); i++) {
	  tmp_l1_norm += fabs(m_mat_column[i]);
	}
	l1_norm = std::max(l1_norm, tmp_l1_norm);
      }
      return l1_norm;
    }

    static double L1Norm(const Vector &v_vec) {
      
      double l1_norm = 0;
      for(index_t d = 0; d < v_vec.length(); d++) {
	l1_norm += fabs(v_vec[d]);
      }
      
      return l1_norm;
    }

    static double EntrywiseLpNorm(const Matrix &m_mat, int p) {

      double lp_norm = 0;
      for(index_t j = 0; j < m_mat.n_cols(); j++) {
	const double *m_mat_column = m_mat.GetColumnPtr(j);

	for(index_t i = 0; i < m_mat.n_rows(); i++) {
	  lp_norm += pow(fabs(m_mat_column[i]), p);
	}
      }
      return lp_norm;
    }

    static double EntrywiseLpNorm(int length, const double *v_arr, int p) {
      double lp_norm = 0;
      for(index_t d = 0; d < length; d++) {
	lp_norm += pow(fabs(v_arr[d]), p);
      }
      
      return lp_norm;
    }

    static double EntrywiseLpNorm(const Vector &v_vec, int p) {
      
      double lp_norm = 0;
      for(index_t d = 0; d < v_vec.length(); d++) {
	lp_norm += pow(fabs(v_vec[d]), p);
      }
      
      return lp_norm;
    }

    /** @brief Compute the pseudoinverse of the matrix.
     *
     *  @param A The matrix to compute the pseudoinverse of.
     *  @param A_inv The computed pseudoinverse by singular value
     *               decomposition.
     */
    static void PseudoInverse(const Matrix &A, Matrix *A_inv) {
      Vector ro_s;
      Matrix ro_U, ro_VT;
      
      // compute the SVD of A
      la::SVDInit(A, &ro_s, &ro_U, &ro_VT);
      
      // take the transpose of V^T and U
      Matrix ro_VT_trans;
      Matrix ro_U_trans;
      la::TransposeInit(ro_VT, &ro_VT_trans);
      la::TransposeInit(ro_U, &ro_U_trans);
      Matrix ro_s_inv;
      ro_s_inv.Init(ro_VT_trans.n_cols(), ro_U_trans.n_rows());
      ro_s_inv.SetZero();
      
      // initialize the diagonal by the inverse of ro_s
      for(index_t i = 0; i < ro_s.length(); i++) {
	if(ro_s[i] > 0.001 * ro_s[0]) {
	  ro_s_inv.set(i, i, 1.0 / ro_s[i]);
	}
	else {
	  ro_s_inv.set(i, i, 0);
	}
      }
      Matrix intermediate;
      la::MulInit(ro_s_inv, ro_U_trans, &intermediate);
      la::MulInit(ro_VT_trans, intermediate, A_inv);
    }
};

#endif
