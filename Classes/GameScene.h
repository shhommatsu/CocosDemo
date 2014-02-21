//
//  GameScene.h
//  CocosDemo
//
//  Created by kazuhisa on 2014/02/20.
//
//

#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include "cocos2d.h"
#include "Config.h"

#define MAX_BLOCK_X 8
#define MAX_BLOCK_Y 8
#define REMOVING_TIME 0.1f
#define MOVING_TIME 0.2f

#define PNG_BACKGROUND "background.png"
#define MP3_REMOVE_BLOCK "removeBlock.mp3"

class GameScene :   public cocos2d::Layer
{
protected:
    enum kTag
    {
        kTagBackground = 1,
        kTagBaseBlock = 10000,
        
    };
    
    enum kZOrder
    {
        kZOrderBackground,
        kZOrderBlock,
    };
    
    struct PositionIndex
    {
        PositionIndex(int x1,int y1)
        {
            x = x1;
            y = y1;
        }
        
        int x;
        int y;
    };
    
    //2-2-3
    cocos2d::Sprite* m_background;
    void showBackground();
    
    //2-2-4
    float m_blockSize;
    std::map<kBlock,std::list<int>> m_blockTag;
    void initForVariables();
    void showBlock();
    cocos2d::Point getPosition(int posIndexX,int posIndexY);
    int getTag(int posIndexX,int posIndexY);
    
    //2-2-5
    cocos2d::EventListenerTouchOneByOne *m_listener;
    
    void getTouchBlockTag(cocos2d::Point touchPoint, int &tag, kBlock &blockType);
    std::list<int> getSameColorBlockTags(int baseTag,kBlock blockType);
    void removeBlock(std::list<int> blockTags,kBlock blockType);
    bool hasSameColorBlock(std::list<int> blockTagList, int searchBlockTag);
    
    //2-3-1
    void removingBlock(cocos2d::Node* block);
    
    //2-3-2
    std::vector<kBlock> blockTypes;
    PositionIndex getPositionIndex(int tag);
    void setNewPosition1(int tag, PositionIndex posIndex);
    void searchNewPosition1(std::list<int> blocks);
    void moveBlock();
    void movingBlockAnimation1(std::list<int> blocks);
    
    
public:
    virtual bool init();
    static cocos2d::Scene* scene();
    CREATE_FUNC(GameScene);
    
    virtual bool onTouchBegan(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
    virtual void onTouchEnded(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
    
};

#endif //   __GAMESCENE_H__#endif // __GAMESCENE_H__
