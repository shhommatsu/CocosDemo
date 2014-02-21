#include "CCPlaySE.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCPlaySE* CCPlaySE::actionWithSoundFile(const std::string &filename, float pitch, float pan, float gain) {
    CCPlaySE* pRet = new CCPlaySE();
    if (pRet != nullptr && pRet->initWithSoundFile(filename, pitch, pan, gain))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

CCPlaySE* CCPlaySE::create(std::string sound)
{
    CCPlaySE* pRet = new CCPlaySE();
    if (pRet)
    {
        pRet->autorelease();
    }
    
    return pRet;
}


CCPlaySE::~CCPlaySE()
{
}

bool CCPlaySE::initWithSoundFile(const std::string &filename, float pitch, float pan, float gain) {
    _soundFile = filename;
    _pitch = pitch;
    _pan = pan;
    _gain = gain;
    return true;
}

CCPlaySE* CCPlaySE::clone() const
{
	// no copy constructor
	auto a = new CCPlaySE();
    a->initWithSoundFile(_soundFile, _pitch, _pan, _gain);
	a->autorelease();
	return a;
}

CCPlaySE* CCPlaySE::reverse() const
{
	// returns a copy of itself
	return this->clone();
}

void CCPlaySE::update(float time)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_soundFile.c_str());
}