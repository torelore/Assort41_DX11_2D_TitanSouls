#pragma once

#include "Scene\SceneInfo.h"

class CStartingRuinSceneInfo :
    public CSceneInfo
{
public:
    CStartingRuinSceneInfo();
    ~CStartingRuinSceneInfo();

private:
    bool m_BossStart;


public:
    virtual bool Init();
    virtual void Update(float DeltaTime);

public:
    virtual void TitanStart();
    virtual void TitanEnd();
};

