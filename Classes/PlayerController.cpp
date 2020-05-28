//
//  PlayerController.cpp
//  guyue
//
//  Created by dsh on 15/1/8.
//
//

#include "PlayerController.h"
static PlayerController* _playerController = NULL;

PlayerController* PlayerController::sharePlayerController()
{
    if (_playerController == NULL)
    {
        _playerController = new PlayerController();
        _playerController->init();
    }
    return _playerController;
}

PlayerController::PlayerController()
:m_playerNumber(11001)
,m_hairNumber(1100)
,m_weaponsNumber(0)
{
    
}

PlayerController::~PlayerController()
{
    
}

bool PlayerController::init()
{
    
    
    
    return true;
}