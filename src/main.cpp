#include <SFML/Graphics.hpp>
#include <optional>
#include <string>
#include <iostream>
#include "Maze.h"
#include "PathFinder.h"

const int CELL_SIZE = 40;
const int WIDTH = 15;
const int HEIGHT = 15;

sf::Color getColor(int cellType) {
    switch(cellType) {
        case 0: return sf::Color::Black;
        case 1: return sf::Color::White;
        case 2: return sf::Color::Green;
        case 3: return sf::Color::Red;
        case 4: return sf::Color::Blue;
        default: return sf::Color::Magenta;
    }
}

void initMaze(Maze& maze) {
    maze.reset();
    for (int i = 0; i < WIDTH; ++i) {
        maze.setWall(i, 0, true);
        maze.setWall(i, HEIGHT-1, true);
        maze.setWall(0, i, true);
        maze.setWall(WIDTH-1, i, true);
    }
}

int main() {
    Maze maze(WIDTH, HEIGHT);
    initMaze(maze);
    PathFinder finder(maze);

    sf::RenderWindow window(sf::VideoMode({WIDTH*CELL_SIZE + 400, HEIGHT*CELL_SIZE}), "Maze Editor & Solver");
    window.setFramerateLimit(60);

    bool searching = false;
    int delayMs = 100;
    bool stepByStepMode = false;
    sf::Clock stepClock;
    bool needStep = false;

    enum EditMode { WALL_TOGGLE, PLACE_START, PLACE_FINISH };
    EditMode currentMode = WALL_TOGGLE;
    bool isMouseLeftPressed = false;
    bool isMouseRightPressed = false;

    sf::Font font;
    if (!font.openFromFile("LiberationSans-Regular.ttf")) {
    }

    sf::Text infoText(font);
    infoText.setCharacterSize(18);
    infoText.setFillColor(sf::Color::Yellow);
    infoText.setPosition({600.f, 10.f});

    sf::Text winText(font, "PATH FOUND!", 30);
    winText.setFillColor(sf::Color::Green);
    winText.setPosition({WIDTH*CELL_SIZE/2.f - 100.f, HEIGHT*CELL_SIZE/2.f - 50.f});

    sf::Text failText(font, "NO PATH!", 30);
    failText.setFillColor(sf::Color::Red);
    failText.setPosition({WIDTH*CELL_SIZE/2.f - 80.f, HEIGHT*CELL_SIZE/2.f - 50.f});

    auto editCell = [&](int cx, int cy, bool isLeft, bool isRight) {
        if (cx <= 0 || cx >= WIDTH-1 || cy <= 0 || cy >= HEIGHT-1) return;
        if (searching) return;

        bool changed = false;
        if (currentMode == WALL_TOGGLE) {
            if (isLeft && !maze.isStart(cx, cy) && !maze.isFinish(cx, cy)) {
                if (maze.getCell(cx, cy) != 0) changed = true;
                maze.setCell(cx, cy, 0);
            } else if (isRight && !maze.isStart(cx, cy) && !maze.isFinish(cx, cy)) {
                if (maze.getCell(cx, cy) != 1) changed = true;
                maze.setCell(cx, cy, 1);
            }
        } else if (currentMode == PLACE_START && isLeft) {
            if (!maze.isWall(cx, cy) && !maze.isFinish(cx, cy)) {
                auto oldStart = maze.getStart();
                if (oldStart.first != cx || oldStart.second != cy) changed = true;
                maze.setStart(cx, cy);
            }
        } else if (currentMode == PLACE_FINISH && isLeft) {
            if (!maze.isWall(cx, cy) && !maze.isStart(cx, cy)) {
                auto oldFinish = maze.getFinish();
                if (oldFinish.first != cx || oldFinish.second != cy) changed = true;
                maze.setFinish(cx, cy);
            }
        }

        if (changed && !searching) {
            finder.reset();
        }
    };

    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::Space) {
                    if (!searching && maze.hasFinish() && maze.hasStart()) {
                        finder.start();
                        searching = true;
                        stepClock.restart();
                    }
                }
                if (key->code == sf::Keyboard::Key::R) {
                    initMaze(maze);
                    finder.reset();
                    searching = false;
                }
                if (key->code == sf::Keyboard::Key::W) currentMode = WALL_TOGGLE;
                if (key->code == sf::Keyboard::Key::S) currentMode = PLACE_START;
                if (key->code == sf::Keyboard::Key::F) currentMode = PLACE_FINISH;
                if (key->code == sf::Keyboard::Key::T) {
                    stepByStepMode = !stepByStepMode;
                    if (!stepByStepMode && searching) stepClock.restart();
                }
                if (key->code == sf::Keyboard::Key::Down && !stepByStepMode) {
                    delayMs = std::max(10, delayMs - 10);
                    std::cout << "Delay: " << delayMs << " ms" << std::endl;
                }
                if (key->code == sf::Keyboard::Key::Up && !stepByStepMode) {
                    delayMs = std::min(500, delayMs + 10);
                    std::cout << "Delay: " << delayMs << " ms" << std::endl;
                }
                if (stepByStepMode && searching && (key->code == sf::Keyboard::Key::Enter || key->code == sf::Keyboard::Key::Right)) {
                    needStep = true;
                }
                if (key->code == sf::Keyboard::Key::G) {
                    maze.generateMaze();
                    finder.reset();
                    searching = false;
                }
            }

            if (const auto* mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouse->button == sf::Mouse::Button::Left) {
                    isMouseLeftPressed = true;
                    sf::Vector2i pos = sf::Mouse::getPosition(window);
                    editCell(pos.x / CELL_SIZE, pos.y / CELL_SIZE, true, false);
                }
                if (mouse->button == sf::Mouse::Button::Right) {
                    isMouseRightPressed = true;
                    sf::Vector2i pos = sf::Mouse::getPosition(window);
                    editCell(pos.x / CELL_SIZE, pos.y / CELL_SIZE, false, true);
                }
            }
            if (event->is<sf::Event::MouseButtonReleased>()) {
                if (event->getIf<sf::Event::MouseButtonReleased>()->button == sf::Mouse::Button::Left)
                    isMouseLeftPressed = false;
                if (event->getIf<sf::Event::MouseButtonReleased>()->button == sf::Mouse::Button::Right)
                    isMouseRightPressed = false;
            }
            if ((isMouseLeftPressed || isMouseRightPressed) && event->is<sf::Event::MouseMoved>()) {
                sf::Vector2i pos = sf::Mouse::getPosition(window);
                editCell(pos.x / CELL_SIZE, pos.y / CELL_SIZE, isMouseLeftPressed, isMouseRightPressed);
            }
        }

        if (searching) {
            if (stepByStepMode) {
                if (needStep) {
                    bool finished = finder.step();
                    needStep = false;
                    if (finished) searching = false;
                }
            } else {
                if (stepClock.getElapsedTime().asMilliseconds() >= delayMs) {
                    bool finished = finder.step();
                    stepClock.restart();
                    if (finished) searching = false;
                }
            }
        }

        std::string modeStr = stepByStepMode ? "STEP (T)" : "AUTO (T)";
        std::string delayStr = stepByStepMode ? "n/a" : std::to_string(delayMs) + " ms";
        std::string toolStr;
        switch(currentMode) {
            case WALL_TOGGLE:  toolStr = "Wall Editor (W) [LMB=wall, RMB=erase]"; break;
            case PLACE_START:  toolStr = "Place Start (S) [LMB=set start]"; break;
            case PLACE_FINISH: toolStr = "Place Finish (F) [LMB=set finish]"; break;
        }
        infoText.setString(
            toolStr + "\n" +
            "Delay: " + delayStr + "  Mode: " + modeStr + "\n" +
            "Space: start | R: reset | T: step mode\n" +
            "G: generate maze\n" +
            (stepByStepMode ? "Enter/Right: step | " : "")
        );

        window.clear(sf::Color::Black);
        for (int i = 0; i < WIDTH; ++i) {
            for (int j = 0; j < HEIGHT; ++j) {
                sf::RectangleShape cell({(float)CELL_SIZE-1.f, (float)CELL_SIZE-1.f});
                cell.setPosition(sf::Vector2f(i*CELL_SIZE, j*CELL_SIZE));
                cell.setFillColor(getColor(maze.getCell(i, j)));
                window.draw(cell);
            }
        }
        window.draw(infoText);
        if (finder.isFinished()) {
            if (finder.pathFound()) window.draw(winText);
            else window.draw(failText);
        }
        window.display();
    }
    return 0;
}