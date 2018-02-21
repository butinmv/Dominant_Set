#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "ResourcePath.hpp"
#include "programmObject.hpp"
#include "button.hpp"

using namespace std;
using namespace sf;

vector<vector<int> > g;
vector<vector<int> > vec;
int n;

// Интерфейс
vector<int> x, y;                       // координаты вершин
int r = 650;                            // радиус круга

bool isPressRight = 0;
bool isPressLeft = 0;
bool isPressU = 0;
bool isPressO = 0;

Color white = Color(255, 255, 255);
Color white_100 = Color(255, 255, 255, 100);
Color white_255 = Color(255, 255, 255, 255);
Color blue_background = Color(35, 95, 165);
Color line_color = Color(255, 255, 255, 50);
Color noCycle = Color(24, 67, 117);
vector<int> memory;
int dom_count = 0;
string str = "";

void inpFile();                         // считывание файла
void showGraph();                       // показ графа
bool nextSet(int* a, int n, int m);     // сочетания без повторений из n по m
void allCombination();                  // занесения всех комбинаций в вектор
void showCombination();                 // показ всех возможных комбинаций

bool findDominantSet(int q)
{
    // vec      -- все возможные комбинации
    // vec[q]  -- текущая возможная комбинация
    
    // создаем вектор дополнений для вершин графа
    vector<int> dop(n);
    for (int i = 0; i < n; i++)
        dop[i] = i;
    
    // удаляем ненужные нам элементы в дополнении
    for (int i = 0; i < vec[q].size(); i++)
        for (int j = 0; j < dop.size();j++)
            if (vec[q][i] == dop[j])           // если текущая вершина в графе есть, то удаляем его из вектора дополнений
                dop.erase(dop.begin() + j);
    
    // вывод дополнения
    cout << endl;
    for (int i = 0; i < dop.size(); i++)
        cout << dop[i] + 1 << " ";
    
    // вывод предполагаемого доминирующего множества
    cout << ": ";
    for (int i = 0; i < vec[q].size();i++)
        cout << vec[q][i] + 1 << " ";
    
    
    vector<bool> c(dop.size(), 0); // есть ли связь у элемента из дополнения с возможным доминирующем множеством
    for (int i = 0; i < vec[q].size(); i++) // обход по вершинам доминирующего множества
        for (int j = 0; j < dop.size(); j++) // обход вершин вектора дополнения
            for (int k = 0; k < g[vec[q][i]].size(); k++) // обход по текущему элементу из вектора связей
            {
                //g[vec[q][i]][k]; текущая элемент из вектора связей
                //dop[j];           текущая вершина дополнения
                if (g[vec[q][i]][k] == dop[j])
                {
                    c[j] = true; // j-элемент дополнения имеет связь с доминирующем множеством
                    break;
                }
            }
    
    bool connect = true; // флажок отвещающий за связь
    for (int i = 0; i < c.size(); i++)
        if (c[i] == false) // если хотя бы один элемент не имеет связи
            connect = false;

    return connect;
}

void work()
{
    inpFile();
    showGraph();
    allCombination();
    for (int i = 0; i < vec.size(); i++)
    {
        if (findDominantSet (i))
        {
            cout << " - true";
            memory.push_back(i);
        }
        else
            cout << " - false";
    }
    //showCombination();
}

int main()
{
    // настройки окна
    ContextSettings settings;
    settings.antialiasingLevel = 100;
    VideoMode videoMode;
    videoMode.width = 2440;
    videoMode.height = 2160;
    string name = "Dominant Set";
    RenderWindow window(videoMode, name, Style::Close, settings);
    
    // настройки шрифта
    Font font;
    if (!font.loadFromFile("cour.ttf"))
    {
        return EXIT_FAILURE;
    }
    Text text("", font, 35);
    text.setFillColor(white);
    text.setStyle(Text::Bold);
    
    // настройки иконки
    Image icon;
    if (!icon.loadFromFile("icon.png"))
    {
        return EXIT_FAILURE;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    
    // найстройки кнопок
    vector<Button> button;
    string str = "";
    char numberCircle[1000] = "";
    
    Button next(1000, 1920, 520, 80, white_100, Color(0,0,0), ">");
    button.push_back(next);
    
    Button prev(120, 1920, 520, 80, white_100, Color(0,0,0), "<");
    button.push_back(prev);
    
    Button info(videoMode.width - 840, 160, 800, videoMode.height - 320, white_100, white_100, "INFORMATION\n", 1);
    button.push_back(info);
    
    // сделать бесцветным + добавить текст
    Button infoDom(videoMode.width - 840, 240, 1000, videoMode.height - 320, Color(255, 255, 255, 0), Color(255, 255, 255), str, 0);
    button.push_back(infoDom);
    infoDom.setFont(30);
    
    work();
    
    for (int i = 0; i < memory.size(); i++)
    {
        string s = to_string(i + 1);
        str = str + "\n   Dominant Set NO." + s + ": ";
        if (i < 9)
            str += " ";
        for (int j = 0; j < vec[memory[i]].size(); j++)
            str = str + " " + to_string(vec[memory[i]][j] + 1);
    }
    infoDom.setText(str);
    
    // задаем круг для вершины
    int r = 30;
    
    // белый круг, используется в цикле
    CircleShape shape(r);
    shape.setOutlineColor(noCycle);
    shape.setOutlineThickness(5);
    shape.setFillColor(Color(0, 0, 0, 0));
    
    CircleShape shape2(r);
    shape2.setOutlineColor(white_255);
    shape2.setOutlineThickness(5);
    shape2.setFillColor(Color(0, 0, 0, 0));

    while (window.isOpen())
    {
        // кнопки ">" и "<"
        if (next.update(&window) || (Keyboard::isKeyPressed(Keyboard::Right) && !isPressRight))
        {
            dom_count++;
            if(dom_count >= memory.size())
                dom_count--;
            isPressRight = 1;
        }
        
        if (prev.update(&window) || (Keyboard::isKeyPressed(Keyboard::Left) && !isPressLeft))
        {
            dom_count--;
            if(dom_count < 0)
                dom_count++;
            isPressLeft = 1;
        }
        
        
        if(!Keyboard::isKeyPressed(Keyboard::Right))
            isPressRight = 0;
        if(!Keyboard::isKeyPressed(Keyboard::Left))
            isPressLeft = 0;
        if(!Keyboard::isKeyPressed(Keyboard::U))
            isPressU = 0;
        if(!Keyboard::isKeyPressed(Keyboard::O))
            isPressO = 0;
        
        
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
                window.close();
            
            // двигаем узлы мышкой
            if (Mouse::isButtonPressed(Mouse::Left))
            {
                for (int i = 0; i < n; i++)
                    if (x[i] <= (Mouse::getPosition(window)).x && x[i] >= (Mouse::getPosition(window)).x - 2 * r &&
                        y[i] <= (Mouse::getPosition(window)).y && y[i] >= (Mouse::getPosition(window)).y - 2 * r)
                    {
                        x[i] = (Mouse::getPosition(window)).x - r;
                        y[i] = (Mouse::getPosition(window)).y - r;
                        break;
                    }
            }
            
            window.clear(blue_background);
            
            
            // вывод ребер
            int edge = g.size();
            for (int i = 0; i < edge; i++)
            {
                int size = g[i].size();
                for(int j = 0; j < size; j++)
                {
                    Vertex line[] =
                    {
                        Vertex(Vector2f(x[i] + r, y[i] + r), white_255),
                        Vertex(Vector2f(x[g[i][j]] + r, y[g[i][j]] + r), white_255)
                    };
                    window.draw(line, 2, Lines);
                }
            }
            
            
            // вывод вершин
            for (int i = 0; i < n; i++)
            {
                shape.setPosition(x[i], y[i]);
                string s = to_string(i + 1);
                text.setString(s);
                text.setPosition(x[i] - 30, y[i] - 30);
                window.draw(shape);
                window.draw(text);
            }
            
            
            for (int i = 0; i < vec[memory[dom_count]].size(); i++)
            {
                shape2.setPosition(x[vec[memory[dom_count]][i]], y[vec[memory[dom_count]][i]]);
                string s = to_string(i + 1);
                text.setString(s);
                text.setPosition(x[i] - 30, y[i] - 30);
                window.draw(shape2);
                window.draw(text);
            }
            
            int v = 40;
            int size = videoMode.width / v + 1;
            for(int j = 0; j < size; j++)
            {
                Vertex line[] =
                {
                    Vertex(Vector2f(j * v, 0), line_color),
                    Vertex(Vector2f(j * v, videoMode.height), line_color)
                };
                window.draw(line, 2, Lines);
            }
            
            size = videoMode.height / v + 1;
            for(int j = 0; j < size; j++)
            {
                Vertex line[] =
                {
                    Vertex(Vector2f(0, j * v), line_color),
                    Vertex(Vector2f(videoMode.width, j * v), line_color)
                };
                window.draw(line, 2, Lines);
            }
            
            
            // вывод кнопок
            size = button.size();
            for(int i = 0; i < size; i++)
                button[i].draw(window);
            
            window.display();
        }
    }
    return 0;
}

void inpFile()
{
    int size = g.size();
    for (int i = 0; i < size; i++)
        g[i].clear();
    g.clear();
    
    ifstream file("input.txt");
    
    if (!file.is_open())
        cout << "Error! File is not found!" << endl;
    else
        file >> n;
    
    for (int i = 0; i < n; i++)
        g.push_back(vector<int>());
    
    // вывод по кругу
    for (int i = 0; i < n; i++)
    {
        int xx = 800 - 30 + r * cos(i * 360 / n * M_PI / 180);
        int yy = 1000 - 30 + r * sin(i * 360 / n * M_PI / 180);
        x.push_back(xx);
        y.push_back(yy);
    }
    
    int v_start, v_finish;
    bool flag = true;
    while(!file.eof())
    {
        file >> v_start;
        file >> v_finish;
        if (v_start > 0 && v_start <= n && v_finish > 0 && v_finish <= n)
        {
            g[v_start - 1].push_back(v_finish - 1);
            g[v_finish - 1].push_back(v_start - 1);
        }
    }
}

void showGraph()
{
    cout << "Количество вершин: " << n << endl;
    for (int i = 0; i < g.size(); i++)
    {
        cout << i + 1 << " - ";
        for (int j = 0; j < g[i].size(); j++)
            cout << g[i][j] + 1 << " ";
        cout << endl;
    }
}

bool nextSet(int* a, int n, int m)
{
    int k = m;
    for (int i = k - 1; i >= 0; --i)
        if (a[i] < n - k + i + 1)
        {
            ++a[i];
            for (int j = i + 1; j < k; ++j)
                a[j] = a[j - 1] + 1;
            return true;
        }
    return false;
}
void allCombination()
{
    int m;
    vector<int> lk;
    for (m = 1; m < n + 1; m++)
    {
        int* a = new int[m];
        for (int i = 0; i < m; i++)
        {
            a[i] = i + 1;
            lk.push_back(a[i] - 1);
        }
        vec.push_back(lk);
        lk.clear();
        if (n >= m)
        {
            while (nextSet(a, n, m))
            {
                for (int i = 0; i < m; i++)
                    lk.push_back(a[i] - 1);
                vec.push_back(lk);
                lk.clear();
            }
        }
    }
}
void showCombination()
{
    for (int i = 0; i < vec.size();i++)
    {
        for (int j = 0; j < vec[i].size();j++)
            cout << vec[i][j] + 1<< " ";
        cout << endl;
    }
}
