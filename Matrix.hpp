#pragma once

#include <vector>

template<typename T>
struct Matrix
{
    Matrix(const std::vector<std::vector<T>>& values)
    : rows_(values.size()),
      cols_(values.empty() ? 0u : values[0].size())
    {
        values_.reserve(rows_ * cols_);
        for (auto row = 0u; row < rows_; ++row)
        {
            for (auto col = 0u; col < cols_; ++col)
            {
                values_.emplace_back(values[row][col]);
            }
        }
    }

    Matrix(std::size_t rows, std::size_t cols, const T& sample = {})
    : rows_(rows),
      cols_(cols),
      values_(rows_ * cols_, sample)
    {}

    T& operator()(std::size_t row, std::size_t col)
    {
        return values_[row * cols_ + col];
    }

    const T& operator()(std::size_t row, std::size_t col) const
    {
        return values_[row * cols_ + col];
    }

    std::size_t rows() const
    {
        return rows_;
    }

    std::size_t cols() const
    {
        return cols_;
    }

    friend auto begin(const Matrix<auto>& matrix)
    { 
        return begin(matrix.values_); 
    }

    friend auto begin(Matrix<auto>& matrix)
    { 
        return begin(matrix.values_); 
    }

    friend auto end(const Matrix<auto>& matrix)
    { 
        return end(matrix.values_); 
    }

    friend auto end(Matrix<auto>& matrix)
    { 
        return end(matrix.values_); 
    }

private:
    std::size_t rows_;
    std::size_t cols_;
    std::vector<T> values_;
};