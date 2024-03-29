#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <list>
#include <iostream>
#include <vector>
#include "entity.h"
#include "level.h"
#include "view.h"

using namespace sf;


void menu () {
    Texture men1, men2, men3, menBackground, menHelp;
    men1.loadFromFile("images/Menu1.png");
    men2.loadFromFile("images/Menu2.png");
    men3.loadFromFile("images/Menu3.png");
    menHelp.loadFromFile("images/Menuhelp.png");
    menBackground.loadFromFile("images/MarioMenu.png");
    Sprite menu1(men1), menu2(men2), menu3(men3), about(menHelp), menuBg(menBackground);
    bool isMenu = 1;
    int menuNum = 0;
    menu1.setPosition(100, 30);
    menu2.setPosition(100, 130);
    menu3.setPosition(100, 230);
    menuBg.setPosition(345, 0);
     RenderWindow window(sf::VideoMode(1024, 600), "Mario 0.5");
    while (isMenu) {
        menu1.setColor(Color::White);
        menu2.setColor(Color::White);
        menu3.setColor(Color::White);
        menuNum = 0;
        window.clear(Color(167, 207, 218));

        if (IntRect(100, 30, 300, 100).contains(Mouse::getPosition(window))) {
            menu1.setColor(Color::Blue);
            menuNum = 1;
        }
        if (IntRect(100, 131, 300, 100).contains(Mouse::getPosition(window))) {
            menu2.setColor(Color::Blue);
            menuNum = 2;
        }
        if (IntRect(100, 232, 300, 100).contains(Mouse::getPosition(window))) {
            menu3.setColor(Color::Blue);
            menuNum = 3;
        }
        if (Mouse::isButtonPressed(Mouse::Left)) {
            if (menuNum == 1) isMenu = false;
            if (menuNum == 2) {
                window.draw(about);
                window.display();
                while (!Keyboard::isKeyPressed(Keyboard::Escape));
            }
            if (menuNum == 3) {
                window.close();
                exit (0);
                isMenu = false;
            }
        }
        window.draw(menuBg);
        window.draw(menu1);
        window.draw(menu2);
        window.draw(menu3);
        window.display();
    }

}

bool startGame (int & startLevel, RenderWindow & window, int & lives) {

    view.reset(FloatRect(0, 0, 700, 600));


    Level lvl;

    switch (startLevel) {
        case 1:
            lvl.LoadFromFile("level1.tmx");
            break;
        case 2:
            lvl.LoadFromFile("level2.tmx");
            break;
        case 3:
            lvl.LoadFromFile("level3.tmx");
            break;
    }
    //lvl.LoadFromFile("level1.tmx");

    SoundBuffer coinBuffer;
    coinBuffer.loadFromFile("sounds/coin.wav");
    Sound coinSound;
    coinSound.setBuffer(coinBuffer);
    SoundBuffer deathBuffer;
    deathBuffer.loadFromFile("sounds/mariodie.wav");
    Sound deathSound;
    deathSound.setBuffer(deathBuffer);
    SoundBuffer stageBuffer;
    stageBuffer.loadFromFile("sounds/stage.wav");
    Sound stageSound;
    stageSound.setBuffer(stageBuffer);

    Image heroImage;
    heroImage.loadFromFile("images/mario.png");
    heroImage.createMaskFromColor(Color(255, 255, 255));
    Image easyEnemyImage;
    easyEnemyImage.loadFromFile("images/EasyEnemy.png");
    easyEnemyImage.createMaskFromColor(Color(255, 255, 255));
    Image coinImage;
    coinImage.loadFromFile("images/coin.png");
    coinImage.createMaskFromColor(Color(255, 255, 255));
    Image mediumEnemyImage;
    mediumEnemyImage.loadFromFile("images/MediumEnemy.png");
    mediumEnemyImage.createMaskFromColor(Color(255, 255, 255));
    Image movePlatformImage;
    movePlatformImage.loadFromFile("images/MovingPlatform.png");

    std::list<Entity*> entities;
    std::list<Entity*>::iterator it;
    std::list<Entity*>::iterator it2;

    std::vector<Object> e = lvl.GetObjects("EasyEnemy");
    for (int i = 0; i < e.size(); i++) {
        entities.push_back(new Enemy(easyEnemyImage, "EasyEnemy", lvl, e[i].rect.left, e[i].rect.top, 15, 25));
    }
    e = lvl.GetObjects("coin");
    for (int i = 0; i < e.size(); i++) {
        entities.push_back(new Coin(coinImage, "coin", lvl, e[i].rect.left, e[i].rect.top, 15, 15));
    }
    e = lvl.GetObjects("MediumEnemy");
    for (int i = 0; i < e.size(); i++) {
        entities.push_back(new Enemy(mediumEnemyImage, "MediumEnemy", lvl, e[i].rect.left, e[i].rect.top, 16, 24));
    }
    e = lvl.GetObjects("exit");
    for (int i = 0; i < e.size(); i++) {
        entities.push_back(new Exit(mediumEnemyImage, "exit", lvl, e[i].rect.left, e[i].rect.top, 16, 24));
    }
    e = lvl.GetObjects("fire");
    for (int i = 0; i < e.size(); i++) {
        entities.push_back(new Fire(mediumEnemyImage, "fire", lvl, e[i].rect.left, e[i].rect.top, 16, 24));
    }
    e = lvl.GetObjects("MovingPlatform");
    for (int i = 0; i < e.size(); i++)
        entities.push_back(new MovingPlatform(movePlatformImage, "MovingPlatform", lvl, e[i].rect.left, e[i].rect.top, 95, 22));

    Object player = lvl.GetObject("player");
    Player p(heroImage, "Player", lvl, player.rect.left, player.rect.top, 22, 38);

    Clock clock;

    while (window.isOpen())
	{

		float time = clock.getElapsedTime().asMicroseconds();

		clock.restart();
		time = time / 800;

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		for (it = entities.begin(); it != entities.end();) {
                Entity *b = *it;
                b->update(time);
                if (b->life == false) {
                    it = entities.erase(it);
                    delete b;
                }
                else it++;
        }

        for (it = entities.begin(); it != entities.end(); it++) {
            if (((*it)->name == "MovingPlatform") && ((*it)->getRect().intersects(p.getRect())))
			{
				Entity *movPlat = *it;
				if (p.dy>0)
				if (p.y + p.height < movPlat->y + movPlat->height)
				{
					p.y = movPlat->y - p.height + 3;
                    p.x += movPlat->dx*time;
                    p.dy = 0;
                    p.onGround = true;
				}
			}
            if ((*it)->getRect().intersects(p.getRect())) {
                if ((*it)->name == "EasyEnemy" || (*it)->name == "MediumEnemy") {
                    if ((p.dy>0) && (p.onGround == false) && (p.y + p.height < (*it)->y + (*it)->height)) {
                        p.dy = -0.3;
                        (*it)->lives--;
                        if ((*it)->name == "EasyEnemy")
                            (*it)->sprite.setTextureRect(IntRect(60, 8, 16, 8));
                        p.Score++;
                    }
                    else {
                        if (((*it)->lives != 0)) {
                            deathSound.play();
                            p.lives--;
                            lives--;
                            return false;
                            }
                        }
                }
                if ((*it)->name == "fire") {
                    deathSound.play();
                    p.lives--;
                    lives--;
                    return false;
                }
                if ((*it)->name == "coin") {
                    p.Score++;
                    coinSound.play();
                    (*it)->life = false;
                }
                if ((*it)->name == "exit") {
                    stageSound.play();
                    startLevel++;
                    return false;
                }
            }
            for (it2 = entities.begin(); it2 != entities.end(); it2++) {
                if ((*it)->getRect() != (*it2)->getRect())
                if (((*it)->getRect().intersects((*it2)->getRect())) && ((*it)->name == "EasyEnemy")){
                    (*it)->dx *= -1;
                    (*it)->sprite.scale(-1.5, 1.5);
                }
            }
        }
        p.update(time);



        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            lives = 0;
            return false;
        }

		window.setView(view);
		window.clear(Color(167, 207, 218));
		lvl.Draw(window);


        for (it = entities.begin(); it != entities.end(); it++) {
            window.draw((*it)->sprite);
		}

		window.draw(p.sprite);
		window.display();
	}

    return 0;

}

void gameRunning (int & startLevel, RenderWindow & window, int & lives) {
    if (startGame (startLevel, window, lives)) gameRunning(startLevel, window, lives);
    else {
        if (lives == 0) {
            lives = 3;
            startLevel = 1;
            menu();
            return;
        }
        if (lives != 0) {
            gameRunning (startLevel, window, lives);
        }

    }
}

int main()
{
 RenderWindow window(sf::VideoMode(1024, 600), "Mario 0.5");
    int StartLevel = 1;
    int lives = 3;

    while (1) {
        StartLevel = 1;
        lives = 3;
        gameRunning(StartLevel, window, lives);
    }

    return 0;
}
