#pragma once

#include "Scene\SceneInfo.h"

class CKnightSceneInfo :
    public CSceneInfo
{
public:
    CKnightSceneInfo();
    ~CKnightSceneInfo();

private:

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);

public:
    virtual void TitanStart();
    virtual void TitanEnd();
};

