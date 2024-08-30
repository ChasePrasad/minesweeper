#include <SFML/System.hpp>
#include <iostream>
#include <chrono>
#include <map>
#include "board.h"
using namespace chrono;

void readConfig(int &c, int &r, int &m);
void setText(Text &text, float x, float y);
void welcome(RenderWindow &win, int wid, int h);
bool enterName(int wid, int h, string &n);
void UI(RenderWindow &win, int c, int r, bool p, bool w, bool l);
void counter(RenderWindow &win, int col, int r, int c);
void timer(RenderWindow &win, int c, int r, int e);
void leaderboard(int c, int r, RenderWindow &win, string n = "NaN", int t = 0);
void game(int c, int r, int m, string n);

int main()
{
    int columns, rows, mines;
    readConfig(columns, rows, mines);
    int width = columns * 32;
    int height = rows * 32 + 100;
    string name;

    if(enterName(width, height, name))
    {
        game(columns, rows, mines, name);
    }
}

void readConfig(int &c, int &r, int &m)
{
    ifstream file("files/config.cfg");
    string columns, rows, mines;

    getline(file, columns);
    getline(file, rows);
    getline(file, mines);

    c = stoi(columns);
    r = stoi(rows);
    m = stoi(mines);

    file.close();
}

void setText(Text &text, float x, float y)
{
    FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    text.setPosition(Vector2f(x, y));
}

void welcome(RenderWindow &win, int wid, int h)
{
    win.clear(Color::Blue);
    
    Font font;
    font.loadFromFile("files/font.ttf");

    Text text;
    text.setFont(font);
    text.setString("WELCOME TO MINESWEEPER!");
    text.setCharacterSize(24);
    text.setFillColor(Color::White);
    text.setStyle(Text::Bold | Text::Underlined);
    setText(text, wid / 2.0f, h / 2.0f - 150);
    win.draw(text);

    text.setString("Enter your name:");
    text.setCharacterSize(20);
    text.setStyle(Text::Bold);
    setText(text, wid / 2.0f, h / 2.0f - 75);
    win.draw(text);
}

bool enterName(int wid, int h, string &n)
{
    Font font;
    font.loadFromFile("files/font.ttf");

    Text text;
    text.setFont(font);
    text.setCharacterSize(18);
    text.setFillColor(Color::Yellow);
    text.setStyle(Text::Bold);
    setText(text, wid / 2.0f, h / 2.0f - 45);

    RenderWindow welcomeWindow(VideoMode(wid, h), "Minesweeper");
    Event event;
    string name;
    bool redraw = true;

    while (true)
    {
        while (welcomeWindow.pollEvent(event))
        {
            if (event.type == Event::TextEntered)
            {
                if (event.text.unicode >= 65 && event.text.unicode <= 90 || event.text.unicode >= 97 && event.text.unicode <= 122)
                {
                    if (name.size() < 10)
                    {
                        if (name.size() == 0)
                        {
                            name += toupper(static_cast<char>(event.text.unicode));
                            redraw = true;
                        }
                        else
                        {
                            name += tolower(static_cast<char>(event.text.unicode));
                            redraw = true;
                        }
                    }
                }
                else if (event.text.unicode == 8)
                {
                    if (name.size() > 0)
                    {
                        name.pop_back();
                        redraw = true;
                    }
                
                }
                else if (event.text.unicode == 13)
                {
                    if (name.size() > 0)
                    {
                        welcomeWindow.close();
                        n = name;
                        return true;
                    }
                }
            }
            if (event.type == Event::Closed)
            {
                welcomeWindow.close();
                return false;
            }
        }

        if(redraw)
        {
            welcome(welcomeWindow, wid, h);
            text.setString(name + "|");
            setText(text, wid / 2.0f, h / 2.0f - 45);
            welcomeWindow.draw(text);
            welcomeWindow.display();
            redraw = false;
        }
    }
}

void UI(RenderWindow &win, int c, int r, bool p, bool w, bool l)
{
    Texture smileTexture;
    if (w)
    {
        smileTexture.loadFromFile("files/images/face_win.png");
    }
    else if (l)
    {
        smileTexture.loadFromFile("files/images/face_lose.png");
    }
    else
    {
        smileTexture.loadFromFile("files/images/face_happy.png");
    }

    Sprite smileSprite;
    smileSprite.setTexture(smileTexture);
    smileSprite.setPosition(c / 2.0f * 32 - 32, 32 * (r + 0.5f));

    win.draw(smileSprite);

    Texture debugTexture;
    debugTexture.loadFromFile("files/images/debug.png");

    Sprite debugSprite;
    debugSprite.setTexture(debugTexture);
    debugSprite.setPosition(c * 32 - 304, 32 * (r + 0.5f));

    win.draw(debugSprite);

    Texture playPauseTexture;
    if (p)
    {
        playPauseTexture.loadFromFile("files/images/play.png");
    }
    else
    {
        playPauseTexture.loadFromFile("files/images/pause.png");
    }

    Sprite playPauseSprite;
    playPauseSprite.setTexture(playPauseTexture);
    playPauseSprite.setPosition(c * 32 - 240, 32 * (r + 0.5f));

    win.draw(playPauseSprite);

    Texture leaderboardTexture;
    leaderboardTexture.loadFromFile("files/images/leaderboard.png");

    Sprite leaderboardSprite;
    leaderboardSprite.setTexture(leaderboardTexture);
    leaderboardSprite.setPosition(c * 32 - 176, 32 * (r + 0.5f));

    win.draw(leaderboardSprite);
}

void counter(RenderWindow &win, int col, int r, int c)
{
    int count;
    if (c < 0)
    {
        count = abs(c);
    }
    else
    {
        count = c;
    }
    int digits[3];

    digits[0] = count / 100;
    digits[1] = (count % 100) / 10;
    digits[2] = count % 10;

    Texture counterTexture;
    counterTexture.loadFromFile("files/images/digits.png");

    Sprite counterSprite;
    counterSprite.setTexture(counterTexture);

    if (c < 0)
    {
        counterSprite.setTextureRect(IntRect(21 * 10, 0, 21, 32));
        counterSprite.setPosition(12, 32 * (r + 0.5f) + 16);
        win.draw(counterSprite);
    }

    counterSprite.setTextureRect(IntRect(21 * digits[0], 0, 21, 32));
    counterSprite.setPosition(33, 32 * (r + 0.5f) + 16);
    win.draw(counterSprite);

    counterSprite.setTextureRect(IntRect(21 * digits[1], 0, 21, 32));
    counterSprite.setPosition(54, 32 * (r + 0.5f) + 16);
    win.draw(counterSprite);

    counterSprite.setTextureRect(IntRect(21 * digits[2], 0, 21, 32));
    counterSprite.setPosition(75, 32 * (r + 0.5f) + 16);
    win.draw(counterSprite);
}

void timer(RenderWindow &win, int c, int r, int e)
{
    int minutes = e / 60;
    int seconds = e % 60;

    int digits[4];
    digits[0] = minutes / 10;
    digits[1] = minutes % 10;
    digits[2] = seconds / 10;
    digits[3] = seconds % 10;

    Texture timerTexture;
    timerTexture.loadFromFile("files/images/digits.png");

    Sprite timerSprite;
    timerSprite.setTexture(timerTexture);

    timerSprite.setTextureRect(IntRect(21 * digits[0], 0, 21, 32));
    timerSprite.setPosition(c * 32 - 97, 32 * (r + 0.5f) + 16);
    win.draw(timerSprite);

    timerSprite.setTextureRect(IntRect(21 * digits[1], 0, 21, 32));
    timerSprite.setPosition(c * 32 - 76, 32 * (r + 0.5f) + 16);
    win.draw(timerSprite);

    timerSprite.setTextureRect(IntRect(21 * digits[2], 0, 21, 32));
    timerSprite.setPosition(c * 32 - 54, 32 * (r + 0.5f) + 16);
    win.draw(timerSprite);

    timerSprite.setTextureRect(IntRect(21 * digits[3], 0, 21, 32));
    timerSprite.setPosition(c * 32 - 33, 32 * (r + 0.5f) + 16);
    win.draw(timerSprite);
}

void leaderboard(int c, int r, RenderWindow &win, string n, int t)
{
    int width = c * 16;
    int height = r * 16 + 50;

    Font font;
    font.loadFromFile("files/font.ttf");

    Text titleText;
    titleText.setFont(font);
    titleText.setString("LEADERBOARD");
    titleText.setStyle(Text::Bold | Text::Underlined);
    titleText.setFillColor(Color::White);
    titleText.setCharacterSize(20);
    setText(titleText, (float)width / 2, (float)height / 2 - 120);

    ifstream ifile("files/leaderboard.txt");
    int count = 1;
    string entryTime, entryName, leaderboard;
    map<string, string> entries;

    while (!ifile.eof())
    {
        getline(ifile, entryTime, ',');
        getline(ifile, entryName);
        entries[entryTime] = entryName.substr(1);
        count++;
    }
    ifile.close();

    while (entries.size() > 5)
    {
        entries.erase(prev(entries.end()));
    }

    if (n != "NaN" && t != 0)
    {
        int min = t / 60;
        int sec = t % 60;
        string time;

        if (min < 10)
        {
            time += "0";
        }
        time += to_string(min) + ":";

        if (sec < 10)
        {
            time += "0";
        }
        time += to_string(sec);

        entries[time] = n + "*";

        while (entries.size() > 5)
        {
            entries.erase(prev(entries.end()));
        }

        ofstream ofile("files/leaderboard.txt");
        for (auto i = entries.begin(); i != prev(entries.end()); i++)
        {
            if (i->second.back() == '*')
            {
                ofile << i->first << ", " << i->second.substr(0, i->second.size() - 1) << endl;
            }
            else
            {
                ofile << i->first << ", " << i->second << endl;
            }
        }
        if (prev(entries.end())->second.back() == '*')
        {
            ofile << prev(entries.end())->first << ", " << prev(entries.end())->second.substr(0, prev(entries.end())->second.size() - 1);
        }
        else
        {
            ofile << prev(entries.end())->first << ", " << prev(entries.end())->second;
        }
        ofile.close();
    }

    int curr = count - 1;
    for (auto i = entries.begin(); i != entries.end(); i++)
    {
        leaderboard += to_string(count - curr) + ".\t" + i->first + "\t" + i->second + "\n\n";
        curr--;
    }
    leaderboard.pop_back();
    leaderboard.pop_back();

    Text entriesText;
    entriesText.setFont(font);
    entriesText.setString(leaderboard);
    entriesText.setStyle(Text::Bold);
    entriesText.setFillColor(Color::White);
    entriesText.setCharacterSize(18);
    setText(entriesText, (float)width / 2, (float)height / 2 + 20);

    RenderWindow leaderboardWindow(VideoMode(width, height), "Minesweeper");
    Event event;

    while (leaderboardWindow.isOpen())
    {
        while (leaderboardWindow.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                leaderboardWindow.close();
            }
        }

        while (win.pollEvent(event)) {}

        leaderboardWindow.clear(Color::Blue);
        leaderboardWindow.draw(titleText);
        leaderboardWindow.draw(entriesText);

        leaderboardWindow.display();
    }
}

void game(int c, int r, int m, string n)
{
    RenderWindow gameWindow(VideoMode(c * 32, r * 32 + 100), "Minesweeper");
    Event event;
    high_resolution_clock::time_point start = high_resolution_clock::now();
    high_resolution_clock::time_point pauseTime;
    int pauseDuration = 0;
    int elapsed = 0;
    int finalTime = 0;
    Board board(c, r, 0, 0, 0);
    Texture tile;
    int count = m;
    bool paused = false;
    bool cont = true;
    bool win = false;
    bool lose = false;
    bool over = false;
    bool leaderboardDisplayed = false;
    bool displayedOnce = false;
    bool clickedOnce = false;

    while (gameWindow.isOpen())
    {
        while (gameWindow.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                gameWindow.close();
            }
            if (event.type == Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == Mouse::Left)
                {
                    if (event.mouseButton.x >= c / 2.0f * 32 - 32 && event.mouseButton.x <= c / 2.0f * 32 + 32 && event.mouseButton.y >= 32 * (r + 0.5f) && event.mouseButton.y <= 32 * (r + 0.5f) + 64)
                    {
                        if (paused)
                        {
                            board.pause();
                            paused = !paused;
                        }

                        int dumpC;
                        int dumpR;
                        readConfig(dumpC, dumpR, m);
                        board = Board(c, r, 0, 0, 0);
                        start = high_resolution_clock::now();
                        pauseDuration = 0;
                        elapsed = 0;
                        finalTime = 0;
                        count = m;
                        win = false;
                        lose = false;
                        cont = true;
                        over = false;
                        leaderboardDisplayed = false;
                        displayedOnce = false;
                        clickedOnce = false;
                    }
                    if (cont && event.mouseButton.x >= c * 32 - 304 && event.mouseButton.x <= c * 32 - 240 && event.mouseButton.y >= 32 * (r + 0.5f) && event.mouseButton.y <= 32 * (r + 0.5f) + 64)
                    {
                        if (clickedOnce)
                        {
                            board.debug();
                        }
                    }
                    if (cont && event.mouseButton.x >= c * 32 - 240 && event.mouseButton.x <= c * 32 - 176 && event.mouseButton.y >= 32 * (r + 0.5f) && event.mouseButton.y <= 32 * (r + 0.5f) + 64)
                    {
                        if(paused)
                        {
                            high_resolution_clock::time_point now = high_resolution_clock::now();
                            pauseDuration += duration_cast<chrono::seconds>(now - pauseTime).count();
                        }
                        else
                        {
                            pauseTime = high_resolution_clock::now();
                        }

                        board.pause();
                        paused = !paused;
                    }
                    if (event.mouseButton.x >= c * 32 - 176 && event.mouseButton.x <= c * 32 - 112 && event.mouseButton.y >= 32 * (r + 0.5f) && event.mouseButton.y <= 32 * (r + 0.5f) + 64)
                    {
                        if (!paused)
                        {
                            pauseTime = high_resolution_clock::now();
                            board.pause();
                            paused = !paused;
                            leaderboardDisplayed = true;

                            gameWindow.clear(Color::White);

                            board.print(gameWindow, tile);
                            UI(gameWindow, c, r, paused, win, lose);
                            counter(gameWindow, c, r, count);
                            elapsed = duration_cast<chrono::seconds>(pauseTime - start).count() - pauseDuration;
                            if (over)
                            {
                                elapsed = finalTime;
                            }
                            timer(gameWindow, c, r, elapsed);

                            gameWindow.display();

                            leaderboard(c, r, gameWindow);

                            leaderboardDisplayed = false;
                            
                            high_resolution_clock::time_point now = high_resolution_clock::now();
                            pauseDuration += duration_cast<chrono::seconds>(now - pauseTime).count();
                            board.pause();
                            paused = !paused;
                        }
                        else
                        {
                            leaderboardDisplayed = true;
                            leaderboard(c, r, gameWindow);
                            leaderboardDisplayed = false;
                        }
                    }

                    int column = event.mouseButton.x / 32;
                    int row = event.mouseButton.y / 32;

                    if (cont && column < c && row < r)
                    {
                        if (!clickedOnce)
                        {
                            int dumpC;
                            int dumpR;
                            readConfig(dumpC, dumpR, m);
                            board = Board(c, r, m, column, row);
                            clickedOnce = true;
                        }

                        cont = board.reveal(column, row);

                        if (!cont)
                        {
                            lose = true;
                            over = true;
                            finalTime = duration_cast<chrono::seconds>(high_resolution_clock::now() - start).count() - pauseDuration;
                        }
                    }
                }
                else if (event.mouseButton.button == Mouse::Right)
                {
                    int column = event.mouseButton.x / 32;
                    int row = event.mouseButton.y / 32;

                    if (cont && column < c && row < r)
                    {
                        if (board.flag(column, row))
                        {
                            count--;
                        }
                        else
                        {
                            count++;
                        }
                    }
                }
                if (board.checkWin())
                {
                    win = true;
                    cont = false;
                    count = 0;
                    over = true;

                    gameWindow.clear(Color::White);

                    board.print(gameWindow, tile);
                    UI(gameWindow, c, r, paused, win, lose);
                    counter(gameWindow, c, r, count);
                    if(!displayedOnce)
                    {
                        finalTime = duration_cast<chrono::seconds>(high_resolution_clock::now() - start).count() - pauseDuration;
                    }
                    timer(gameWindow, c, r, finalTime);

                    gameWindow.display();

                    if (!leaderboardDisplayed && !displayedOnce)
                    {
                        leaderboardDisplayed = true;
                        leaderboard(c, r, gameWindow, n, finalTime);
                        leaderboardDisplayed = false;
                        displayedOnce = true;
                    }
                }
            }
        }
        gameWindow.clear(Color::White);

        board.print(gameWindow, tile);
        UI(gameWindow, c, r, paused, win, lose);
        counter(gameWindow, c, r, count);
        if (!paused && !over && !leaderboardDisplayed)
        {
            elapsed = duration_cast<chrono::seconds>(high_resolution_clock::now() - start).count() - pauseDuration;
        }
        else if (over)
        {
            elapsed = finalTime;
        }
        timer(gameWindow, c, r, elapsed);

        gameWindow.display();
    }
}