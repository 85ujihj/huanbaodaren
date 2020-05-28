

#include "PropIconShow.h"

PropIconShow::PropIconShow(PropInfo* propInfo)
:m_propInfo(propInfo)
{
    char str[32];
    sprintf(str, "prop_icon/propIcon_%u.png", m_propInfo->getIconNumber());
    m_path = str;
    
}

PropIconShow::~PropIconShow()
{

}

PropIconShow* PropIconShow::create(PropInfo* propInfo)
{
    if (propInfo == NULL)
        return NULL;
    
    PropIconShow* show = new PropIconShow(propInfo);
    if (show && show->initWithFile(show->m_path.c_str()))
    {
        show->autorelease();
        return show;
    }
    CC_SAFE_DELETE(show);
    return NULL;
}