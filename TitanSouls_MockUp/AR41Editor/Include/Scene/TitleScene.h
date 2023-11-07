#pragma once
#include "Scene/SceneInfo.h"

class CTitleScene :
    public CSceneInfo
{
public:
    CTitleScene();
    ~CTitleScene();

public:
    virtual bool Init();
};

