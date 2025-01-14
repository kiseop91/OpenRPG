﻿#include "stdafx.h"

#include "Game/Game.h"
#include "GUI/Gui.h"
#include "Components/SoundComponent.h"
#include "States/State.h"
#include "States/MainMenu/MainMenuState.h"
#include "States/Game/GameState.h"
#include "States/Editor/EditorState.h"
#include "States/Settings/SettingsState.h"

#include "Managers/SoundManager.h"
#include "Managers/StateManager.h"

// Initaillizer functions
void MainMenuState::initVariables() {}

void MainMenuState::initBackground() {
	this->background.setSize(sf::Vector2f(
		static_cast<float>(this->window->getSize().x),
		static_cast<float>(this->window->getSize().y)));
	if (!this->backgroundTexture.loadFromFile("Resources/image/Backgrounds/bg3.png")) {
		throw "ERROR::MAIN_MENU_STATE::FAILED_TO_LOAD_BACKGROUND_TEXTURE";
	}

	this->background.setTexture(&this->backgroundTexture);
}

void MainMenuState::initFonts() {
	if (!this->font.loadFromFile("Fonts/R2.ttc")) {
		throw("메인메뉴 폰트로딩 실패");
	}
}

void MainMenuState::initKeybinds() {
	std::ifstream ifs("Config/mainmenuestate_keybinds.ini");

	if (ifs.is_open()) {
		std::string key = "";
		std::string key2 = "";
		int key_value = 0;
		while (ifs >> key >> key2) {
			this->keybinds[key] = this->supportedKeys->at(key2);
		}
	}
	ifs.close();

	this->keybinds["CLOSE"] = this->supportedKeys->at("Escape");
	this->keybinds["MOVE_LEFT"] = this->supportedKeys->at("A");
	this->keybinds["MOVE_RIGHT"] = this->supportedKeys->at("D");
	this->keybinds["MOVE_UP"] = this->supportedKeys->at("W");
	this->keybinds["MOVE_DOWN"] = this->supportedKeys->at("S");
}

void MainMenuState::initButtons() {

	if (!btnTexure.loadFromFile("Resources/image/Buttons/btn1.png")) {
		throw "ERROR::MAIN_MENU_STATE::FAILED_TO_LOAD_BACKGROUND_TEXTURE";
	}

	this->buttons["GAME_STATE"] = new gui::Button(
		1500, 500, 250, 160, &btnTexure, &this->font, L"새 게임", 40, sf::Color(0, 0, 0, 255),
		sf::Color(150, 150, 150, 250), sf::Color(20, 20, 20, 50), sf::Color(255, 255, 255, 255),
		sf::Color(255, 255, 255, 255), sf::Color(255, 255, 255, 255));

	this->buttons["SETTING_STATE"] = new gui::Button(
		1500, 600, 250, 160, &btnTexure, &this->font, L"게임 설정", 40, sf::Color(0, 0, 0, 255),
		sf::Color(150, 150, 150, 250), sf::Color(20, 20, 20, 50), sf::Color(255, 255, 255, 255),
		sf::Color(255, 255, 255, 255), sf::Color(255, 255, 255, 255));

	this->buttons["EDITOR_STATE"] = new gui::Button(
		1500, 700, 250, 160, &btnTexure, &this->font, L"에디터", 40, sf::Color(0, 0, 0, 255),
		sf::Color(150, 150, 150, 250), sf::Color(20, 20, 20, 50), sf::Color(255, 255, 255, 255),
		sf::Color(255, 255, 255, 255), sf::Color(255, 255, 255, 255));

	this->buttons["EXIT_STATE"] = new gui::Button(
		1500, 900, 250, 160, &btnTexure, &this->font, L"종 료", 40, sf::Color(0, 0, 0, 255),
		sf::Color(150, 150, 150, 250), sf::Color(20, 20, 20, 50), sf::Color(255, 255, 255, 255),
		sf::Color(255, 255, 255, 255), sf::Color(255, 255, 255, 255));
}

MainMenuState::MainMenuState(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys)
	: State(window, supportedKeys) {
	this->initVariables();
	this->initBackground();
	this->initFonts();
	this->initKeybinds();
	this->initButtons();
	this->initSounds();
}

MainMenuState::~MainMenuState() {
	auto it = this->buttons.begin();
	for (it = this->buttons.begin(); it != this->buttons.end(); ++it) {
		delete it->second;
	}
}

void MainMenuState::updateInput(const float& dt) {

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
		this->background.setFillColor(sf::Color::Cyan);
}

void MainMenuState::updateButtons() {
	//모든 버튼들의 상태를 기능에맞게 업데이트해줌
	for (auto& it : this->buttons) {
		it.second->update(this->mousePosView);
	}

	//게임시작
	if (this->buttons["GAME_STATE"]->isPressed()) {
		StateManager::getInstance()->Push(new GameState(this->window, this->supportedKeys));
	}

	//설정
	if (this->buttons["SETTING_STATE"]->isPressed()) {
		StateManager::getInstance()->Push(new SettingsState(this->window, this->supportedKeys));
	}

	//에디터
	if (this->buttons["EDITOR_STATE"]->isPressed()) {
		StateManager::getInstance()->Push(new EditorState(this->window, this->supportedKeys));
	}

	//종료
	if (this->buttons["EXIT_STATE"]->isPressed()) {
		this->endState();
	}
}

void MainMenuState::update() {
	auto dt = Game::getInstance()->frameTime();

	this->updateMousePositions();
	this->updateInput(dt);

	this->updateButtons();
}

void MainMenuState::renderButtons(sf::RenderTarget& target) {
	for (auto& it : this->buttons) {
		it.second->render(target);
	}
}

void MainMenuState::render(sf::RenderTarget* target) {
	if (!target)
		target = this->window;

	target->draw(this->background);
	this->renderButtons(*target);

	//삭제예정. 디버깅용.
	sf::Text mouseText;
	mouseText.setPosition(sf::Vector2f(this->mousePosView.x, this->mousePosView.y - 15));
	mouseText.setFont(this->font);
	mouseText.setCharacterSize(15);
	std::stringstream ss;
	ss << this->mousePosView.x << " " << this->mousePosView.y;
	mouseText.setString(ss.str());

	target->draw(mouseText);
}

// 메인 화면이 표시될 때 BGM 재생
void MainMenuState::onActivated() {
	auto sm = SoundManager::getInstance();
	sm->LoadBGM(sounds["BACKGROUND_MUSIC"]);
	sm->getBGM()->setLoop(true)->play();
}
// 메인 화면이 숨겨질 때 BGM 중지
void MainMenuState::onDeactivated() {
	SoundManager::getInstance()->getBGM()->stop();
}