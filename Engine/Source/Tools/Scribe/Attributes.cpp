#include "Attributes.hpp"
#include "Attributes/AttrProperty.hpp"

void CAttributeManager::Initialize()
{
    mAttributes = {{WT("Property"), std::move(ProcessProperty)}};
}
