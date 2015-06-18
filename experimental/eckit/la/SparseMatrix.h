/*
 * (C) Copyright 1996-2015 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation
 * nor does it submit to any jurisdiction.
 */

/// @file   SparseMatrix.h
/// @author Florian Rathgeber
/// @date   June 2015

#ifndef eckit_la_SparseMatrix_h
#define eckit_la_SparseMatrix_h

#include <numeric>
#include <vector>

#include "eckit/exception/Exceptions.h"
#include "eckit/la/types.h"
#include "eckit/la/Triplet.h"

namespace eckit {
namespace la {

//-----------------------------------------------------------------------------

/// Sparse matrix in CRS (compressed row storage) format
class SparseMatrix {
public:  // types
    typedef la::Index Index;
    typedef la::Scalar Scalar;
    typedef std::vector<Scalar> ScalarStorage;
    typedef std::vector<Index> IndexStorage;
    typedef ScalarStorage::size_type Size;

public:  // methods

    // -- Constructors

    /// Default constructor, empty matrix
    SparseMatrix() {}

    /// Constructor setting sizes, empty matrix
    SparseMatrix(Size rows, Size cols) : rows_(rows), cols_(cols) {}

    /// Constructor setting sizes and number of non-zeros, allocates memory
    SparseMatrix(Size rows, Size cols, Size nnz)
        : v_(nnz), outer_(rows+1), inner_(nnz), rows_(rows), cols_(cols) {}

    /// Constructor initialising matrix with existing storage
    SparseMatrix(Size rows, Size cols,
                 const ScalarStorage& v, const IndexStorage& outer, const IndexStorage& inner)
        : v_(v), rows_(rows), cols_(cols) {
        ASSERT(v.size() == inner.size() && outer.size() == rows+1);
    }

    /// Initialise matrix from vector of triplets
    ///
    /// Resets data and index vectors, requires row and column number to be set.
    /// Assumes no order of triplets and allows duplicates, which are summed.
    void setFromTriplets(const std::vector<Triplet>& triplets) {
        ASSERT( rows_ > 0 && cols_ > 0 );

        // Build sparsity pattern i.e. inner indices for each row
        std::vector<std::set<Index> > nz(rows_);
        for (std::vector<Triplet>::const_iterator it = triplets.begin(); it != triplets.end(); ++it) {
            ASSERT(it->row() >= 0 && it->row() < rows_ && it->col() >= 0 && it->col() < cols_);
            nz[it->row()].insert(it->col());
        }

        // Build vector of outer indices
        outer_.resize(rows_+1);
        outer_[0] = 0;
        for (Index i = 0; i < rows_; ++i)
            outer_[i+1] = outer_[i] + nz[i].size();
        Size nnz = outer_[rows_];

        // Build vectors of inner indices and values
        inner_.resize(nnz);
        v_.assign(nnz, Scalar(0));
        for (std::vector<Triplet>::const_iterator it = triplets.begin(); it != triplets.end(); ++it) {
            const Size row = it->row();
            const Size col = it->col();
            // Find the position in the current row
            const Size pos = outer_[row] + std::distance(nz[row].begin(), nz[row].find(col));
            inner_[pos] = col;
            v_[pos] += it->value();
        }
    }

    /// @returns size (rows * cols)
    Size size() const { return rows_*cols_; }
    /// @returns number of rows
    Size rows() const { return rows_; }
    /// @returns number of columns
    Size cols() const { return cols_; }
    /// @returns inner size (number of columns)
    Size innerSize() const { return cols_; }
    /// @returns outer size (number of rows)
    Size outerSize() const { return rows_; }
    /// @returns number of non-zeros
    Size nnz() const { return v_.size(); }

    /// @returns modifiable view of the data vector
    Scalar* data() { return v_.data(); }
    /// @returns read-only view of the data vector
    const Scalar* data() const { return v_.data(); }
    /// @returns modifiable view of the outer index vector
    Index* outer() { return outer_.data(); }
    /// @returns read-only view of the outer index vector
    const Index* outer() const { return outer_.data(); }
    /// @returns modifiable view of the inner index vector
    Index* inner() { return inner_.data(); }
    /// @returns read-only view of the inner index vector
    const Index* inner() const { return inner_.data(); }

protected:  // members
    ScalarStorage v_;     /// Data
    IndexStorage outer_;  /// Starts of rows
    IndexStorage inner_;  /// Column indices
    Index rows_;          /// Number of rows
    Index cols_;          /// Number of columns
};

//-----------------------------------------------------------------------------

}  // namespace la
}  // namespace eckit

#endif
