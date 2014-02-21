#ifndef __CCPLAYSE_H__
#define __CCPLAYSE_H__

#include "cocos2d.h"

class CCPlaySE : public cocos2d::ActionInstant
{
public:
    static CCPlaySE* actionWithSoundFile(const std::string &file, float pitch, float pan, float gain);
    /**
     * @js NA
     * @lua NA
     */
    ~CCPlaySE();
    bool initWithSoundFile(const std::string &file, float pitch, float pan, float gain);
    
    // Overrides
    virtual void update(float time) override;
	virtual CCPlaySE* clone() const override;
	virtual CCPlaySE* reverse() const override;

public:
    static CCPlaySE* create(std::string sound);
    
private:
    std::string _soundFile;
    float _pitch, _pan, _gain;
};


#endif // __CCPLAYSE_H__