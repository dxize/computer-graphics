#include "AlchemyModel.h"

#include <algorithm>
#include <stdexcept>

AlchemyModel::AlchemyModel()
{
    AddElements();
    Reset();
}

void AlchemyModel::AddElements()
{
    m_elements = {
        { "fire", U"Огонь" },
        { "water", U"Вода" },
        { "earth", U"Земля" },
        { "air", U"Воздух" },
        { "steam", U"Пар" },
        { "alcohol", U"Спирт" },
        { "lava", U"Лава" },
        { "dust", U"Пыль" },
        { "powder", U"Порох" },
        { "explosion", U"Взрыв" },
        { "smoke", U"Дым" },
        { "energy", U"Энергия" },
        { "stone", U"Камень" },
        { "storm", U"Буря" },
        { "metal", U"Металл" },
        { "electricity", U"Электричество" },
        { "hydrogen", U"Водород" },
        { "oxygen", U"Кислород" },
        { "ozone", U"Озон" },
        { "mud", U"Грязь" },
        { "geyser", U"Гейзер" },
        { "boiler", U"Паровой котел" },
        { "pressure", U"Давление" },
        { "volcano", U"Вулкан" },
        { "detonating_gas", U"Гремучий газ" },
        { "swamp", U"Болото" },
        { "molotov", U"Коктейль Молотова" },
        { "life", U"Жизнь" },
        { "bacteria", U"Бактерии" },
        { "vodka", U"Водка" }
    };
}

const ElementInfo& AlchemyModel::GetElement(const std::string& id) const
{
    for (const ElementInfo& element : m_elements)
    {
        if (element.id == id)
        {
            return element;
        }
    }

    throw std::runtime_error("Unknown element: " + id);
}

const std::vector<ElementInfo>& AlchemyModel::GetElements() const
{
    return m_elements;
}

const std::vector<std::string>& AlchemyModel::GetDiscovered() const
{
    return m_discovered;
}

std::vector<std::string> AlchemyModel::Combine(const std::string& first, const std::string& second)
{
    const std::vector<std::string> results = m_recipeBook.Find(first, second);

    for (const std::string& id : results)
    {
        Discover(id);
    }

    return results;
}

void AlchemyModel::Discover(const std::string& id)
{
    if (IsDiscovered(id))
    {
        return;
    }

    m_discovered.push_back(id);
}

bool AlchemyModel::IsDiscovered(const std::string& id) const
{
    return std::find(m_discovered.begin(), m_discovered.end(), id) != m_discovered.end();
}

bool AlchemyModel::IsFinished() const
{
    return m_discovered.size() == m_elements.size();
}

void AlchemyModel::Reset()
{
    m_discovered = {
        "fire",
        "water",
        "earth",
        "air"
    };
}

void AlchemyModel::SortDiscovered()
{
    std::sort(
        m_discovered.begin(),
        m_discovered.end(),
        [this](const std::string& first, const std::string& second)
        {
            return GetElement(first).name.toUtf32() < GetElement(second).name.toUtf32();
        }
    );
}
