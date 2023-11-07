#pragma once

#include "Scene\SceneInfo.h"

class CSlimeSceneInfo :
    public CSceneInfo
{
public:
    CSlimeSceneInfo();
    ~CSlimeSceneInfo();

private:

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);

public:
    virtual void TitanStart();
    virtual void TitanEnd();
};

