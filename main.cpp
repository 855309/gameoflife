/*
    author: @fikret0
    date: November 13, 2021

    GitHub: @fikret0
*/

#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>

#define WIDTH 700
#define HEIGHT 700

#define XCELLS 35
#define YCELLS 35

int cellw = WIDTH / XCELLS, cellh = HEIGHT / YCELLS;

bool stopped = false;

std::vector<std::vector<int>> grid;

sf::RenderWindow mwindow(sf::VideoMode(WIDTH, HEIGHT), "Conway's Game of Life");

bool darkmode = false;

int getneighbours(int ln, int col, std::vector<std::vector<int>> dgrid = grid){
    int c = 0;
    for(int y = ln - 1; y <= ln + 1; y++){
        for(int x = col - 1; x <= col + 1; x++){
            if(y >= 0 && x >=0 && y < YCELLS && x < XCELLS){
                c += dgrid[y][x];
            }
        }
    }

    return c - dgrid[ln][col];
}

float defaultdelay = 0.5f;
float fastdelay = 0.05f;
float delay = defaultdelay;

sf::Clock gameclock;
void timemanager(){  
    float elapsed = gameclock.getElapsedTime().asSeconds();

    if (elapsed >= delay){
        std::vector<std::vector<int>> tmp = grid;

        for(int ln = 0; ln < YCELLS; ln++){
            std::vector<int> l = grid[ln];

            for(int cn = 0; cn < XCELLS; cn++){
                int col = l[cn];
                int nb = getneighbours(ln, cn);

                if(col == 1){
                    if(nb < 2 || nb > 3){
                        tmp[ln][cn] = 0;
                        // std::cout << "Dead: " << ln << ", " << cn << " - " << nb << std::endl;
                    }
                }
                else{
                    if(nb == 3){
                        tmp[ln][cn] = 1;
                        // std::cout << "Born: " << ln << ", " << cn << " - " << nb << std::endl;
                    }
                }
            }
        }

        grid = tmp;

        gameclock.restart();
    }
}

void initgrid(){
    for(int yc = 0; yc < YCELLS; yc++){
        std::vector<int> ln;
        for(int xc = 0; xc < XCELLS; xc++){
            ln.push_back(0);
        }

        grid.push_back(ln);
    }
}

void cleargrid(){
    grid.clear();
    initgrid();
}

void correcttitle(){
    if(stopped){
        mwindow.setTitle("Conway's Game of Life - Stopped");
    }
    else{
        mwindow.setTitle("Conway's Game of Life");
    }
}

void togglestop(){
    stopped = !stopped;
    if(stopped){
        mwindow.setTitle("Conway's Game of Life - Stopped");
    }
    else{
        mwindow.setTitle("Conway's Game of Life");
    }
}

void togglecell(int ln, int col){
    int v = grid[ln][col];
    if(v == 1){
        grid[ln][col] = 0;
    }
    else{
        grid[ln][col] = 1;
    }
}

int main(int argc, char **argv){
    std::cout << "GitHub: @fikret0 (c) 2021" << std::endl << "Conway's Game of Life" << std::endl << "--dark for dark mode" << std::endl << std::endl 
        << "Space: Toggle start/stop" << std::endl
        << "C: Clear the grid" << std::endl
        << "Tab: Fast forward" << std::endl;

    for(int ai = 0; ai < argc; ai++){
        std::string arg = argv[ai];
        if(arg == "--dark"){
            darkmode = true;
        }
    }

    sf::Color gridcolor(200, 200, 200);
    sf::Color cellcolor(0, 0, 0);
    sf::Color bgcolor(255, 255, 255);

    if(darkmode){
        gridcolor = sf::Color(55, 55, 55);
        cellcolor = sf::Color(255, 255, 255);
        bgcolor = sf::Color(0, 0, 0);
    }

    togglestop();
    cleargrid();

    while (mwindow.isOpen()){
        sf::Event event;
        while (mwindow.pollEvent(event)){
            if (event.type == sf::Event::Closed){
                mwindow.close();
            }
            else if(event.type == sf::Event::KeyPressed){
                if(event.key.code == sf::Keyboard::Space){ // toggle stop / start
                    togglestop();
                }
                else if(event.key.code == sf::Keyboard::C){ // clear
                    cleargrid();
                }
                else if(event.key.code == sf::Keyboard::Tab){ // fast forward
                    if(delay != fastdelay){
                        delay = fastdelay;
                        mwindow.setTitle("Conway's Game of Life - Fast Forward");
                    }
                }
            }
            else if(event.type == sf::Event::KeyReleased){
                if(event.key.code == sf::Keyboard::Tab){ // restore default speed
                    delay = defaultdelay;
                    correcttitle();
                }
            }
            else if (event.type == sf::Event::MouseButtonReleased){
                if (event.mouseButton.button == sf::Mouse::Left){
                    int col = floor(event.mouseButton.x / cellw);
                    int ln = floor(event.mouseButton.y / cellh);

                    gameclock.restart();
                    togglecell(ln, col);

                    // std::cout << "Line: " << ln << ", Column: " << col << std::endl;
                }
            }
        }

        mwindow.clear(bgcolor);

        if(!stopped){
            timemanager();
        }
        else{
            gameclock.restart();
        }

        float th = 1.f;

        /* Draw the grid */
        for(int xc = 0; xc <= XCELLS; xc++){
            int c = cellw * xc;

            sf::RectangleShape line;
            line.setPosition(c, 0);
            line.setSize(sf::Vector2f(th, HEIGHT));

            line.setFillColor(gridcolor);
            line.setOutlineThickness(0);

            mwindow.draw(line);
        }

        for(int yc = 0; yc <= YCELLS; yc++){
            int c = cellh * yc;

            sf::RectangleShape line;
            line.setPosition(0, c);
            line.setSize(sf::Vector2f(WIDTH, th));

            line.setFillColor(gridcolor);
            line.setOutlineThickness(0);

            mwindow.draw(line); 
        }

        /* Draw the cells */
        for(int ln = 0; ln < YCELLS; ln++){
            std::vector<int> l = grid[ln];
            for(int cn = 0; cn < XCELLS; cn++){
                int col = l[cn];

                if(col == 1){
                    sf::RectangleShape cell;
                    cell.setPosition(cn * cellw + th, ln * cellh + th);
                    cell.setSize(sf::Vector2f(cellw - th, cellh - th));

                    cell.setFillColor(cellcolor);
                    cell.setOutlineThickness(0);
                
                    mwindow.draw(cell);
                }
            }
        }

        mwindow.display();
    }
}