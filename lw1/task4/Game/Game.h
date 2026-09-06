#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

#include "../Graphics/Hangman.h"

class Game : public sf::Drawable
{
public:
    Game();

    bool HandleClick(sf::Vector2f mousePosition);

private:
    struct WordData
    {
        sf::String word;
        sf::String hint;
    };

    const int m_maxMistakes = 7;

    sf::Font m_font;
    Hangman m_hangman;

    std::vector<WordData> m_words;

    sf::String m_word;
    sf::String m_hint;
    sf::String m_usedLetters;

    const sf::String m_alphabet = U"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";

    const sf::Vector2f m_yesButtonPosition = { 410.0f, 435.0f };
    const sf::Vector2f m_noButtonPosition = { 530.0f, 435.0f };
    const sf::Vector2f m_buttonSize = { 90.0f, 45.0f };

    int m_mistakes = 0;
    bool m_finished = false;
    bool m_won = false;

    bool HandleEndGameClick(sf::Vector2f mousePosition);
    void HandleLetterClick(sf::Vector2f mousePosition);

    void AddWordFromLine(const std::string& line);
    sf::String ToSfString(const std::string& text) const;

    void LoadWords();

    int GetRandomWordIndex() const;
    void ResetGameState();

    void StartNewGame();

    void GuessLetter(char32_t letter);
    void AddMistake();
    void CheckGameEnd();

    bool IsLetterUsed(char32_t letter) const;
    bool WordContains(char32_t letter) const;
    bool IsWordGuessed() const;
    bool IsInside(
        sf::Vector2f point,
        sf::Vector2f position,
        sf::Vector2f size
    ) const;

    sf::String GetHiddenWord() const;
    char32_t GetClickedLetter(sf::Vector2f mousePosition) const;
    sf::Vector2f GetLetterPosition(std::size_t index) const;
    sf::Color GetLetterColor(char32_t letter) const;

    void DrawEndBackground(sf::RenderTarget& target) const;
    void DrawResultMessage(sf::RenderTarget& target) const;
    void DrawRestartControls(sf::RenderTarget& target) const;

    void DrawHint(sf::RenderTarget& target) const;
    void DrawWord(sf::RenderTarget& target) const;
    void DrawMistakes(sf::RenderTarget& target) const;
    void DrawAlphabet(sf::RenderTarget& target) const;
    void DrawEndPanel(sf::RenderTarget& target) const;
    void DrawButton(
        sf::RenderTarget& target,
        sf::Vector2f position,
        const sf::String& text
    ) const;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
