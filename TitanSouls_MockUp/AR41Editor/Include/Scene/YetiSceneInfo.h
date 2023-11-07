#pragma once

#include "Scene\SceneInfo.h"

class CYetiSceneInfo :
    public CSceneInfo
{
public:
    CYetiSceneInfo();
    ~CYetiSceneInfo();

private:

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);

public :
    virtual void TitanStart();
    virtual void TitanEnd();
};

