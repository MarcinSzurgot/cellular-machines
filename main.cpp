#include "CellularAutomata.hpp"
#include "Matrix.hpp"
#include "RectanglesMatrix.hpp"
#include "WindowRunner.hpp"

#include <SFML/Window/Event.hpp>

#include <fstream>
#include <limits>
#include <chrono>
#include <random>

namespace
{

void randomize(CellularAutomata& automata)
{
    const auto now = std::chrono::high_resolution_clock::now();
    const auto seed = now.time_since_epoch().count();
    auto generator = std::mt19937(seed);

    for (auto row = 0u; row < automata.rows(); ++row)
    {
        for (auto col = 0u; col < automata.cols(); ++col)
        {
            automata(row, col) = std::uniform_int_distribution{0u, 1u}(generator);
        }
    }
}

CellularAutomata readAutomataFromFile(const char* filename)
{
    auto automataSize = sf::Vector2u();
    auto patchSize = sf::Vector2u();
    auto deadCellsRules = std::vector<unsigned int>();
    auto aliveCellsRules = std::vector<unsigned int>();

    auto file = std::ifstream(filename);
    file >> automataSize.x >> automataSize.y >> patchSize.x >> patchSize.y;

    const auto numberOfCellsInPatch = (2 * patchSize.x + 1) * (2 * patchSize.y + 1) - 1;
    for (auto i = 0u; i < numberOfCellsInPatch; ++i)
    {
        auto value = 0u;
        file >> value;
        deadCellsRules.push_back(value);

    }

    for (auto i = 0u; i < numberOfCellsInPatch; ++i)
    {
        auto value = 0u;
        file >> value;
        aliveCellsRules.push_back(value);
    }

    return CellularAutomata(automataSize, patchSize, deadCellsRules, aliveCellsRules);
}

void applyColors(const CellularAutomata& automata, RectanglesMatrix& rectangles)
{
    const auto low = sf::Color::White;
    const auto high = sf::Color::Black;
    const auto patchSize = automata.patchSize();
    const auto numberOfCellsInPatch = (2 * patchSize.x + 1) * (2 * patchSize.y + 1) - 1;
    for (auto row = 0u; row < automata.rows(); ++row)
    {
        for (auto col = 0u; col < automata.cols(); ++col)
        {
            const auto cellState = automata.neighbors(row, col);
            if (cellState > 0u)
            {
                const auto saturation = cellState / static_cast<float>(numberOfCellsInPatch);
                const auto color = sf::Color(high.r * saturation + low.r * (1 - saturation),
                                            high.g * saturation + low.g * (1 - saturation),
                                            high.b * saturation + low.b * (1 - saturation));
                rectangles.setColor({row, col}, color);
            }
            else
            {
                rectangles.setColor({row, col}, sf::Color::Black);
            }
        }
    }
}

}

int main()
{
    auto automata = readAutomataFromFile("automata.txt");
    auto cells = RectanglesMatrix(sf::Vector2u(automata.cols(), automata.rows()), sf::Vector2f(800.f, 600.f));

    randomize(automata);
    // automata(automata.rows() / 2, automata.cols() / 2 - 1) = 1;
    // automata(automata.rows() / 2, automata.cols() / 2 + 0) = 1;
    // automata(automata.rows() / 2, automata.cols() / 2 + 1) = 1;
    applyColors(automata, cells);

    auto runner = WindowRunner();
    runner.run([&](sf::RenderWindow& window){
        for (auto event = sf::Event(); window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
        automata.updateState();
        applyColors(automata, cells);
        window.draw(cells);
    });
    return 0;
}