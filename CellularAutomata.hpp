#pragma once

#include "Matrix.hpp"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include <cstdint>
#include <iterator>

inline sf::Rect<unsigned int> patch(sf::Rect<unsigned int> bounds, 
                                    sf::Vector2u center, 
                                    sf::Vector2u distanceFromCenter)
{
    constexpr auto min = std::numeric_limits<std::uint32_t>::lowest();

    const auto diffX = center.x - distanceFromCenter.x;
    const auto diffY = center.y - distanceFromCenter.y;

    const auto minX = std::max(center.x < diffX ? min : diffX,  bounds.left);
    const auto minY = std::max(center.y < diffY ? min : diffY,  bounds.top);
    const auto maxX = std::min(center.x + distanceFromCenter.x, bounds.left + bounds.width - 1u);
    const auto maxY = std::min(center.y + distanceFromCenter.y, bounds.top  + bounds.height - 1u);
    return {minX, minY, maxX - minX + 1u, maxY - minY + 1u};
}

inline unsigned int sum(const Matrix<unsigned int>& matrix, sf::Rect<unsigned int> patch)
{
    auto sum = 0u;
    for (auto row = 0u; row < patch.height; ++row)
    {
        for (auto col = 0u; col < patch.width; ++col)
        {
            sum += matrix(row + patch.top, col + patch.left) > 0u;
        }
    }
    return sum;
}

struct CellularAutomata : Matrix<unsigned int>
{
    CellularAutomata(sf::Vector2u automataSize, 
                     sf::Vector2u patchSize,
                     const std::vector<unsigned int>& deadCellsRules, 
                     const std::vector<unsigned int>& aliveCellsRules)
    : Matrix(automataSize.y, automataSize.x),
      patchSize_(patchSize),
      cellsRules_({deadCellsRules, aliveCellsRules}),
      cellsCopy_(rows(), cols())
    {
        const auto numberOfCellsInPatch = (2 * patchSize.x + 1) * (2 * patchSize.y + 1) - 1;
        if (size(aliveCellsRules) != numberOfCellsInPatch || size(deadCellsRules) != numberOfCellsInPatch)
        {
            throw std::invalid_argument("Size of cells rules does not match patch size.");
        }
    }

    void updateState()
    {
        const auto bounds = sf::Rect<unsigned int>(0u, 0u, cols(), rows());
        const auto distanceFromCenter = sf::Vector2u(1u, 1u);
        for (auto row = 0u; row < rows(); ++row)
        {
            for (auto col = 0u; col < cols(); ++col)
            {
                const auto center = sf::Vector2u(col, row);
                const auto patch = ::patch(bounds, center, distanceFromCenter);
                cellsCopy_(row, col) = ::sum(*this, patch) - (*this)(row, col);
            }
        }

        std::transform(begin(cellsCopy_), 
                       end(cellsCopy_), 
                       begin(*this), 
                       begin(*this), 
                       [this](auto neighbors, auto oldState){
            return cellsRules_(oldState, neighbors);
        });
    }

    unsigned int neighbors(std::size_t row, std::size_t col) const
    {
        return cellsCopy_(row, col);
    }

    sf::Vector2u patchSize() const 
    {
        return patchSize_;
    }

private:    
    sf::Vector2u patchSize_;
            Matrix<unsigned int> cellsRules_;
    mutable Matrix<unsigned int> cellsCopy_;
};