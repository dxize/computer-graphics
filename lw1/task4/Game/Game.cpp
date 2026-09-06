#include "Game.h"
#include "../Palette.h"

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <stdexcept>
#include <string>

Game::Game()
{
    if (!m_font.openFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        throw std::runtime_error("Cannot load font");
    }

    unsigned int seed = static_cast<unsigned int>(std::time(nullptr));
    std::srand(seed);

    LoadWords();
    StartNewGame();
}

sf::String Game::ToSfString(const std::string& text) const
{
    return sf::String::fromUtf8(
        text.begin(),
        text.end()
    );
}

void Game::AddWordFromLine(const std::string& line)
{
    const std::size_t separator = line.find('|');

    if (separator == std::string::npos)
    {
        return;
    }

    const std::string wordText = line.substr(0, separator);

    const std::string hintText = line.substr(separator + 1);

    const sf::String word = ToSfString(wordText);
    const sf::String hint = ToSfString(hintText);

    m_words.push_back({ word, hint });
}

int Game::GetRandomWordIndex() const
{
    int index = std::rand() % static_cast<int>(m_words.size());

    if (m_words.size() > 1)
    {
        while (m_words[index].word == m_word)
        {
            index = std::rand() % static_cast<int>(m_words.size());
        }
    }

    return index;
}

void Game::ResetGameState()
{
    m_usedLetters.clear();

    m_mistakes = 0;
    m_finished = false;
    m_won = false;

    m_hangman.SetMistakes(0);
}

void Game::LoadWords()
{
    std::ifstream file("words.txt");

    if (!file.is_open())
    {
        throw std::runtime_error("Cannot open words.txt");
    }

    std::string line;

    while (std::getline(file, line))
    {
        AddWordFromLine(line);
    }

    if (m_words.empty())
    {
        throw std::runtime_error("words.txt is empty");
    }
}

void Game::StartNewGame()
{
    const int index = GetRandomWordIndex();

    m_word = m_words[index].word;
    m_hint = m_words[index].hint;

    ResetGameState();
}

bool Game::HandleEndGameClick(sf::Vector2f mousePosition)
{
    if (IsInside(mousePosition, m_yesButtonPosition, m_buttonSize))
    {
        StartNewGame();
        return false;
    }

    if (IsInside(mousePosition, m_noButtonPosition, m_buttonSize))
    {
        return true;
    }

    return false;
}

void Game::HandleLetterClick(sf::Vector2f mousePosition)
{
    const char32_t letter = GetClickedLetter(mousePosition);

    if (letter != U'\0')
    {
        GuessLetter(letter);
    }
}

bool Game::HandleClick(sf::Vector2f mousePosition)
{
    if (m_finished)
    {
        return HandleEndGameClick(mousePosition);
    }

    HandleLetterClick(mousePosition);

    return false;
}

void Game::GuessLetter(char32_t letter)
{
    if (IsLetterUsed(letter))
    {
        return;
    }

    m_usedLetters += letter;

    if (!WordContains(letter))
    {
        AddMistake();
    }

    CheckGameEnd();
}

void Game::AddMistake()
{
    m_mistakes++;
    m_hangman.SetMistakes(m_mistakes);
}

void Game::CheckGameEnd()
{
    if (IsWordGuessed())
    {
        m_finished = true;
        m_won = true;
        return;
    }

    if (m_mistakes >= m_maxMistakes)
    {
        m_finished = true;
        m_won = false;
    }
}

bool Game::IsLetterUsed(char32_t letter) const
{
    for (std::size_t i = 0; i < m_usedLetters.getSize(); ++i)
    {
        if (m_usedLetters[i] == letter)
        {
            return true;
        }
    }

    return false;
}

bool Game::WordContains(char32_t letter) const
{
    for (std::size_t i = 0; i < m_word.getSize(); ++i)
    {
        if (m_word[i] == letter)
        {
            return true;
        }
    }

    return false;
}

bool Game::IsWordGuessed() const
{
    for (std::size_t i = 0; i < m_word.getSize(); ++i)
    {
        if (!IsLetterUsed(m_word[i]))
        {
            return false;
        }
    }

    return true;
}

bool Game::IsInside(
    sf::Vector2f point,
    sf::Vector2f position,
    sf::Vector2f size
) const
{
    return point.x >= position.x &&
           point.x <= position.x + size.x &&
           point.y >= position.y &&
           point.y <= position.y + size.y;
}

sf::String Game::GetHiddenWord() const
{
    std::u32string result;

    for (std::size_t i = 0; i < m_word.getSize(); ++i)
    {
        if (IsLetterUsed(m_word[i]))
        {
            result += m_word[i];
        }
        else
        {
            result += U'_';
        }

        result += U' ';
    }

    return sf::String(result);
}

sf::Vector2f Game::GetLetterPosition(std::size_t index) const
{
    const int lettersPerRow = 17;
    const int row = static_cast<int>(index) / lettersPerRow;
    const int column = static_cast<int>(index) % lettersPerRow;

    return {
        65.0f + column * 52.0f,
        570.0f + row * 48.0f
    };
}

char32_t Game::GetClickedLetter(sf::Vector2f mousePosition) const
{
    for (std::size_t i = 0; i < m_alphabet.getSize(); ++i)
    {
        const sf::Vector2f position = GetLetterPosition(i);

        if (IsInside(mousePosition, position, {38.0f, 38.0f}))
        {
            return m_alphabet[i];
        }
    }

    return U'\0';
}

sf::Color Game::GetLetterColor(char32_t letter) const
{
    if (!IsLetterUsed(letter))
    {
        return Palette::Unused;
    }

    if (WordContains(letter))
    {
        return Palette::Correct;
    }

    return Palette::Wrong;
}

void Game::DrawHint(sf::RenderTarget& target) const
{
    sf::Text title(m_font, U"Подсказка:", 24);
    title.setPosition({410.0f, 80.0f});
    title.setFillColor(Palette::Text);

    sf::Text hint(m_font, m_hint, 28);
    hint.setPosition({410.0f, 120.0f});
    hint.setFillColor(Palette::Text);

    target.draw(title);
    target.draw(hint);
}

void Game::DrawWord(sf::RenderTarget& target) const
{
    sf::Text word(m_font, GetHiddenWord(), 42);
    word.setPosition({410.0f, 300.0f});
    word.setFillColor(Palette::Text);

    target.draw(word);
}

void Game::DrawMistakes(sf::RenderTarget& target) const
{
    sf::String text = U"Ошибки: ";
    text += sf::String(std::to_string(m_mistakes));
    text += U" / ";
    text += sf::String(std::to_string(m_maxMistakes));

    sf::Text mistakes(m_font, text, 22);
    mistakes.setPosition({410.0f, 390.0f});
    mistakes.setFillColor(Palette::Text);

    target.draw(mistakes);
}

void Game::DrawAlphabet(sf::RenderTarget& target) const
{
    for (std::size_t i = 0; i < m_alphabet.getSize(); ++i)
    {
        const char32_t letter = m_alphabet[i];

        sf::String letterText;
        letterText += letter;

        sf::Text text(m_font, letterText, 28);
        text.setPosition(GetLetterPosition(i));
        text.setFillColor(GetLetterColor(letter));

        target.draw(text);
    }
}

void Game::DrawButton(
    sf::RenderTarget& target,
    sf::Vector2f position,
    const sf::String& text
) const
{
    sf::RectangleShape button(m_buttonSize);
    button.setPosition(position);
    button.setFillColor(Palette::Button);
    button.setOutlineThickness(2.0f);
    button.setOutlineColor(Palette::ButtonOutline);

    target.draw(button);

    sf::Text label(m_font, text, 22);
    label.setPosition({ position.x + 22.0f, position.y + 7.0f });
    label.setFillColor(Palette::Text);

    target.draw(label);
}

void Game::DrawEndBackground(sf::RenderTarget& target) const
{
    sf::RectangleShape panel({ 1000.0f, 700.0f });
    panel.setFillColor(Palette::Panel);

    target.draw(panel);
}

void Game::DrawResultMessage(sf::RenderTarget& target) const
{
    sf::String message;

    if (m_won)
    {
        message = U"\n\nВы выиграли!";
    }
    else
    {
        message = U"Вы проиграли.\n\nСлово: ";
        message += m_word;
    }

    sf::Text result(m_font, message, 32);
    result.setPosition({ 300.0f, 200.0f });
    result.setFillColor(Palette::Text);

    target.draw(result);
}

void Game::DrawRestartControls(sf::RenderTarget& target) const
{
    sf::Text question(m_font, U"Начать новую игру?", 26);
    question.setPosition({ 390.0f, 380.0f });
    question.setFillColor(Palette::Text);

    target.draw(question);

    DrawButton(target, m_yesButtonPosition, U"ДА");
    DrawButton(target, m_noButtonPosition, U"НЕТ");
}

void Game::DrawEndPanel(sf::RenderTarget& target) const
{
    DrawEndBackground(target);
    DrawResultMessage(target);
    DrawRestartControls(target);
}

void Game::draw(
    sf::RenderTarget& target,
    sf::RenderStates states
) const
{
    if (m_finished)
    {
        DrawEndPanel(target);
        return;
    }

    target.draw(m_hangman, states);

    DrawHint(target);
    DrawWord(target);
    DrawMistakes(target);
    DrawAlphabet(target);
}
