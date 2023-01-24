#pragma once

#include <iostream>
#include <vector>
#include <sstream> //Used for updateText() function

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>



class Game {
private:
	//Variables

	//Window
	sf::RenderWindow* window;
	sf::Event ev;
	sf::VideoMode videoMode;


	//Resources
	sf::Font font;
	sf::Text hudText;
	sf::SoundBuffer projectileBuffer;
	sf::Sound projectileSound;
	sf::SoundBuffer collisionBuffer;
	sf::Sound collisionSound;

	//Game logic
	bool endGame;
	int health;
	unsigned points;
	float asteroidSpawnTimer;
	float asteroidSpawnTimerMax;
	int maxAsteroids;
	int shootTimer;
	int shootTimerMax;

	//Game objects
	sf::Texture asteroidTexture;
	sf::Texture playerTexture;
	sf::Texture projectileTexture;
	std::vector<sf::Sprite> asteroids;
	std::vector<sf::Sprite> projectiles;
	sf::Sprite asteroid;
	sf::Sprite player;
	sf::Sprite projectile;

	//Private functions
	void initVariables();
	void initWindow();
	void initAsteroids();
	void initPlayer();
	void initProjectile();
	void initFonts();
	void initText();

public:
	//Constructors and Destructor
	Game();
	virtual ~Game();

	//Accessors
	const bool running() const;
	const bool getEndGame() const;

	//Functions
	void pollEvents();

	void updateText();
	void updateAsteroids();
	void updtatePlayer();
	void update();

	void spawnAsteroid();

	void renderText();
	void renderAsteroids();
	void renderPlayer();
	void renderProjectiles();
	void render();

	void runGame();

};
