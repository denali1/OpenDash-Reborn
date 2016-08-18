#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "MenuNode.h"
#include "HotKey.h"

HotKey::HotKey()
{
    mNode = 0;
    mButton = -1;
}

HotKey::HotKey(MenuNode *fNode, int fButton)
{
    mNode = fNode;
    mButton = fButton;
}

HotKey::~HotKey()
{
    // yeah
}

void HotKey::setNode(MenuNode *fNode)
{
    mNode = fNode;
}

void HotKey::setButton(int fButton)
{
    mButton = fButton;
}

MenuNode *HotKey::getNode()
{
    return mNode;
}

int HotKey::getButton()
{
    return mButton;
}
