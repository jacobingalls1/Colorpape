#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
using namespace std;

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
using namespace sf;

const int SCREEN_X = 1920, SCREEN_Y = 1080, COLOR_FUDGE = 10, MAX_VALUE = 100, NAME_UPDATE = 40, COLOR_STEP = 4,
        COLOR_UPDATE = 10, TEXT_SIZE = 72, SQUARE_SIDE = 600;
const string OUTPUT_FILE = "C:\\Users\\Ophiuchus\\Pictures\\wallpaper\\COLORS\\", FILE_EXT = ".png";
int colorR(0), colorG(0), colorB(0);

vector<string> prefixes;
// = {"paper", "foily", "scratchy", "japanese", "strange", "daytime", "watercolor",
//                                 "oily", "fat", "incredibly", "otherwise", "languishing", "finished", "decent",
//                                 "soulful", "grounded", "ethical", "immoral", "foolish", "stupid", "immature",
//                                 "organic", "not", "plastic", "city", "Owen's", "rope", "not very", "strong", "weak"}

string fromColor(Color in) {
    string out = prefixes[rand() % prefixes.size()];
    out.push_back(' ');
    if (in.r > .75 * (in.b + in.g)) {
        out.append("red");
        return out;
    }
    if (in.b > .75 * (in.r + in.g)) {
        out.append("blue");
        return out;
    }
    if (in.g > .75 * (in.b + in.r)) {
        out.append("green");
        return out;
    }
    if (3 * in.r < in.b + in.g) {
        out.append("cyan");
        return out;
    }
    if (3 * in.b < in.r + in.g) {
        out.append("yellow");
        return out;
    }
    if (3 * in.g < in.b + in.r) {
        out.append("magenta");
        return out;
    }
    out.append("grey");
    return out;
}

Color nextColor() {
    colorR += COLOR_STEP;
    if (colorR > MAX_VALUE) {
        colorR %= MAX_VALUE;
        colorG += COLOR_STEP;
    }
    if (colorG > MAX_VALUE) {
        colorG %= MAX_VALUE;
        colorB += COLOR_STEP;
    }
    if (colorB > MAX_VALUE) {
        colorB %= MAX_VALUE;
    }
    return Color(colorR, colorG, colorB);
//    return Color(rand() % MAX_VALUE, rand() % MAX_VALUE, rand() % MAX_VALUE);
}

int sign(int in) {
    if (in < 0) {
        return -1;
    } else if (in > 0) {
        return 1;
    } else {
        return 0;
    }
}

void step(Color &now, Color target) {
    int diffR(target.r - now.r), diffG(target.g - now.g), diffB(target.b - now.b);
    if (abs(diffR) < COLOR_FUDGE and abs(diffG) < COLOR_FUDGE and abs(diffB) < COLOR_FUDGE) {
        now = target;
        return;
    }
    now.r += COLOR_STEP * sign(diffR);
    now.g += COLOR_STEP * sign(diffG);
    now.b += COLOR_STEP * sign(diffB);
}

int main() {


    ifstream file("adjectives1.txt");
    string word;
    while (!file.eof()) {
        file >> word;
        prefixes.push_back(word);
    }
    file.close();

    srand(time(NULL));

    Font font;
    font.loadFromFile("LCD_Solid.ttf");


    RenderWindow window(VideoMode(SCREEN_X, SCREEN_Y), "Colors", Style::None);
    window.requestFocus();
    window.setFramerateLimit(10);
    window.setMouseCursorVisible(false);

//    Color target(nextColor());

    Text name;
    name.setFont(font);
    name.setCharacterSize(TEXT_SIZE);
    Text color;
    color.setFont(font);
    color.setCharacterSize(TEXT_SIZE);
    Vector2f center(SCREEN_X / 2, SCREEN_Y / 2);

    int count = 0;

    while( window.isOpen() ) {
        Color now(nextColor());
        window.clear(now);



//        RectangleShape rect;
//        rect.setPosition(center - Vector2f(SQUARE_SIDE / 2, SQUARE_SIDE / 2));
//        rect.setFillColor(now);
//        rect.setSize(Vector2f(SQUARE_SIDE, SQUARE_SIDE));
//        rect.setOutlineColor(Color::Black);
//        rect.setOutlineThickness(10);
//        window.draw(rect);

        stringstream colorS;
//        stringstream targetS;

        name.setString(fromColor(now));
        FloatRect textRect = name.getLocalBounds();
        name.setOrigin(textRect.left + textRect.width/2.0f,
                       textRect.top  + textRect.height/2.0f);
        name.setPosition(sf::Vector2f(SCREEN_X/2.0f,SCREEN_Y/2.0f));
        name.setFillColor(Color(255 - now.r, 255 - now.g, 255 - now.b));



//        colorS << (int)now.r << " " << (int)now.g << " " << (int)now.b;
//        color.setString(colorS.str());
//        color.setFillColor(Color::White);
//        color.setPosition(center - Vector2f(color.getCharacterSize() * (color.getString().getSize()), TEXT_SIZE));



//        if (now == target) {
//            target = nextColor();
//        } else {
//            step(now, target);
//        }

        window.draw(name);
        window.draw(color);


//        if (count % NAME_UPDATE == 0) {
//            target = nextColor();
//        }

        if (count % COLOR_UPDATE == 0) {
        }

        window.display();



        Texture thisWindow;
        thisWindow.update(window);

        string colorName(name.getString());
        colorName[colorName.find(' ')] = '_';

        Image screen = thisWindow.copyToImage();
        string filepath(OUTPUT_FILE);
        filepath.append(colorName);
        filepath.push_back('_');
        filepath.append(to_string(count));
        filepath.append(FILE_EXT);

//        screen.saveToFile(filepath);

        count++;


        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            } else if (event.type == Event::KeyPressed) {
                switch (event.key.code) {
                    case Keyboard::Escape:
                    case Keyboard::Q:
                        window.close();
                        break;
                }
            }
        }
    }

    return EXIT_SUCCESS;
}
