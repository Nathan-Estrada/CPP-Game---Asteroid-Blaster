#include <iostream>

#include "Game.h"

//Initializes all variables
void Game::initVariables() {
	window = nullptr;

	//Game logic
	endGame = false;
	health = 10;
	points = 0;
	asteroidSpawnTimerMax = 50.f;
	asteroidSpawnTimer = asteroidSpawnTimerMax;
	maxAsteroids = 7;
	shootTimer = 0.f;
	shootTimerMax = 23.f;
}

void Game::initWindow() {
	videoMode.height = 600;
	videoMode.width = 800;
	window = new sf::RenderWindow(videoMode, "Asteroid Blaster");
	window->setFramerateLimit(60);
}

//Constructors
Game::Game() {
	initVariables();
	initFonts();
	initText();
	initWindow();
	initAsteroids();
	initPlayer();
	initProjectile();
}

//Destructors
Game:: ~Game() {
	delete window;
}

//Objects
void Game::initAsteroids() { //Asteroid Sprite
	asteroidTexture.loadFromFile("images/Asteroid_05.GIF");
	asteroid.setTexture(asteroidTexture);
}

void Game::initPlayer() { //Player Sprite
	playerTexture.loadFromFile("images/ship.png");
	player.setTexture(playerTexture);
	player.setPosition(35, 200);
	player.rotate(90.f);

	//Projectile properties
	projectileBuffer.loadFromFile("sounds/Attack Sound.wav");
	projectileSound.setBuffer(projectileBuffer);
	collisionBuffer.loadFromFile("sounds/womp.wav");
	collisionSound.setBuffer(collisionBuffer);
}

void Game::initProjectile() { //Projectile Sprite
	projectileTexture.loadFromFile("images/Laser Sprites/14.png");
	projectile.setTexture(projectileTexture);
	projectile.setScale(.3, .3);
}


//Specialized text for hud
void Game::initFonts() {
	font.loadFromFile("Fonts/Square.ttf");
}

void Game::initText() {
	hudText.setFont(font);
	hudText.setCharacterSize(24);
	hudText.setFillColor(sf::Color::White);
	hudText.setString("NONE");
}

//Accessors
const bool Game::running() const {
	return window->isOpen();
}

const bool Game::getEndGame() const {
	return endGame;
}

//Functions
void Game::spawnAsteroid() {
	/*
	@return void
	Spawns asteroid and sets randomly sets its size and vertical position
	Adds asteroid to the vector
	*/

	asteroid.setPosition(
		800,
		static_cast<float>(rand() % static_cast<int>(window->getSize().y - asteroid.getGlobalBounds().height)));

	//Randomize asteroid size

	int type = rand() % 3;

	switch (type) {
	case 0:
		asteroid.setScale(0.5, 0.5);
		break;

	case 1:
		asteroid.setScale(0.7, 0.7);
		break;

	case 2:
		asteroid.setScale(1, 1);
		break;
	default:
		asteroid.setScale(1, 1);
		break;
	}
	//Spawn asteroid
	asteroids.push_back(asteroid);
}

void Game::pollEvents() {
	while (window->pollEvent(ev)) {
		switch (ev.type) {
		case sf::Event::Closed:
			window->close();
			break;
		case sf::Event::KeyPressed: //Pressing escape terminates program and displays end-game stats
			if (ev.key.code == sf::Keyboard::Escape) {
				window->close();
				std::cout << "Game over!" << std::endl;
				std::cout << "Points scored: " << points << std::endl;
				break;
			}
		}
	}
}

//Displays user's health and points in corner of screen
void Game::updateText() {
	std::stringstream ss;
	ss << "Points: " << points << std::endl;
	ss << "Health: " << this->health << std::endl;
	hudText.setString(ss.str());
}

/*
Updates asteroid spawn timer and spawns Asteroids when the current amount of Asteroids
is less than the max amount of Asteroids allowed
Moves the Asteroids from right to left
*/
void Game::updateAsteroids() {
	//Updates timer for asteroid spawning
	if (asteroids.size() < maxAsteroids) {

		//Spawns asteroid and resets timer
		if (asteroidSpawnTimer >= asteroidSpawnTimerMax) {
			spawnAsteroid();
			asteroidSpawnTimer = 0.f;
		}
		else {
			asteroidSpawnTimer += 2.f;
		}
	}

	//Move and updating Asteroids
	for (int i = 0; i < asteroids.size(); i++) {
		bool deleted = false;
		asteroids[i].move(-4.f, 0.f);

		//Remove Asteroids outside of window view
		if (asteroids[i].getPosition().x < 0 - asteroid.getScale().x) {
			health -= 1;
			asteroids.erase(asteroids.begin() + i);
		}
	}
};

void Game::updtatePlayer() {
	//Keyboard input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		if (player.getPosition().y > 0) {
			player.move(0.0f, -7.f);
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		if (player.getPosition().y < window->getSize().y - player.getGlobalBounds().height) {
			player.move(0.0f, 7.f);
		}
	}

	//Determines rate of fire for player's attack
	if (shootTimer < shootTimerMax) {
		shootTimer++;
	}
	//Binds attack to space
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && shootTimer >= shootTimerMax) {
		projectile.setPosition(player.getPosition());
		projectiles.push_back(sf::Sprite(projectile)); //Adds projectile to projectiles vector upon firing weapon
		projectileSound.play();
		shootTimer = 0;
	}
	//Erases off screen projectiles from vector
	for (int i = 0; i < projectiles.size(); i++) {

	}
	//Collision Detection
	for (int i = 0; i < projectiles.size(); i++) {
		for (int j = 0; j < asteroids.size(); j++) {
			if (projectiles[i].getGlobalBounds().intersects(asteroids[j].getGlobalBounds())) {
				projectiles.erase(projectiles.begin() + i);
				asteroids.erase(asteroids.begin() + j);
				collisionSound.play();
				points++; //Points are awarded upon projectile eliminating asteroid
			}
		}
	}
}

void Game::update() {
	//Event polling
	pollEvents();
	if (!endGame) {
		updateText();
		updateAsteroids();
		updtatePlayer();
	}
	//Endgame condition
	if (health <= 0) {
		endGame = true;
		std::cout << "Game over!" << std::endl;
		std::cout << "You scored " << points << " points.";
	}
}

void Game::renderText() {
	window->draw(hudText);
}

void Game::renderAsteroids() {
	//Rendering all the Asteroids
	for (auto& e : asteroids) {
		window->draw(e);
	}
}

void Game::renderProjectiles() {
	for (int i = 0; i < projectiles.size(); i++) {
		projectiles[i].move(10.f, 0.f);
		window->draw(projectiles[i]);
	}
}

void Game::renderPlayer() {
	window->draw(player);
}

void Game::render() {
	//Clear old frames
	window->clear();

	//Draw game objects
	renderAsteroids();
	renderText();
	renderPlayer();
	renderProjectiles();
	window->display();
}

void Game::runGame() {
	while (running() && !getEndGame()) {
		update();
		render();
	}
}