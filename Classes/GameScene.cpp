//
//  GameScene.cpp
//  CocosDemo
//
//  Created by kazuhisa on 2014/02/20.
//
//

#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "BlockSprite.h"
#include "CCPlaySE.h"


using namespace cocos2d;
using namespace CocosDenshion;
using namespace std;

Scene* GameScene::scene()
{
    auto scene = Scene::create();
    auto layer = GameScene::create();
    scene->addChild(layer);
    return scene;
}


bool GameScene::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    m_listener = EventListenerTouchOneByOne::create();
    m_listener -> onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    m_listener -> onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
    
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_listener, this);
    
    initForVariables();
    
    showBackground();
    
    showBlock();
    
    SimpleAudioEngine::getInstance()->preloadEffect(MP3_REMOVE_BLOCK);
    
    return true;
}

void GameScene::showBackground()
{
    Size winSize = Director::getInstance()->getWinSize();
        
    m_background = Sprite::create(PNG_BACKGROUND);
    m_background->setPosition(Point(winSize.width *0.5, winSize.height *0.5));
    addChild(m_background, kZOrderBackground, kTagBackground);
}

void GameScene::initForVariables()
{
    srand((unsigned)time(NULL));
    
    BlockSprite *pBlock = BlockSprite::createWithBlockType(kBlockRed);
    m_blockSize = pBlock->getContentSize().height;
    
    
    blockTypes.push_back(kBlockRed);
    blockTypes.push_back(kBlockGreen);
    blockTypes.push_back(kBlockBlue);
    blockTypes.push_back(kBlockGray);
    blockTypes.push_back(kBlockYellow);
}

Point GameScene::getPosition(int posIndexX, int posIndexY)
{
    float offsetX = m_background->getContentSize().width * 0.029;
    float offsetY = m_background->getContentSize().height * 0.168;
    
    //CCLOG("x:%f,y:%f", (posIndexX + 0.5) * m_blockSize + offsetX,(posIndexY + 2.55) * m_blockSize + offsetY);
    
    return Point((posIndexX +0.5) * m_blockSize + offsetX, (posIndexY + 2.55) * m_blockSize + offsetY);
    
}

int GameScene::getTag(int posIndexX, int posIndexY)
{
    return kTagBaseBlock +posIndexX *100 +posIndexY;
}

void GameScene::showBlock()
{
    for (int i = 0; i < MAX_BLOCK_X; i++) {
        for (int j = 0; j <MAX_BLOCK_Y; j++) {
            kBlock blockType = (kBlock)(rand() % kBlockCount);
            
            int tag = getTag(i, j);
            m_blockTag[blockType].push_back(tag);
            
            BlockSprite * pBlock = BlockSprite::createWithBlockType(blockType);
            pBlock->setPosition(getPosition(i, j));
            m_background->addChild(pBlock,kZOrderBlock,tag);
        }
    }
}

//touch
bool GameScene::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}

void GameScene::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    //CCLOG("onTouchEnded");
    Point touchPoint = m_background->convertTouchToNodeSpace(pTouch);
    
    int tag = 0;
    kBlock blockType;
    getTouchBlockTag(touchPoint, tag, blockType);
    
    if (tag != 0)
    {
        list<int> sameColorBlockTags = getSameColorBlockTags(tag, blockType);
        
        if (sameColorBlockTags.size() > 1) {
            removeBlock(sameColorBlockTags, blockType);
            
            movingBlockAnimation1(sameColorBlockTags);
        }
    }
}

void GameScene::getTouchBlockTag(cocos2d::Point touchPoint, int &tag, kBlock &blockType)
{
    for (int i = 0;i < MAX_BLOCK_X; i++)
    {
        for(int j = 0;j < MAX_BLOCK_Y; j++)
        {
            int currentTag = getTag(i,j);
            Node *node = m_background->getChildByTag(currentTag);
            if (node && node ->getBoundingBox().containsPoint(touchPoint))
            {
                tag = currentTag;
                blockType = ((BlockSprite*)node)->getBlockType();
                
                return;
            }
        }
    }
}

bool GameScene::hasSameColorBlock(list<int> blockTagList, int searchBlockTag)
{
    list<int>::iterator it;
    for (it = blockTagList.begin(); it != blockTagList.end(); ++it)
    {
        if(*it == searchBlockTag)
        {
            return true;
        }
    }
    return false;
}

list<int> GameScene::getSameColorBlockTags(int baseTag, kBlock blockType)
{
    list<int> sameColorBlockTags;
    sameColorBlockTags.push_back(baseTag);
    
    list<int>::iterator it = sameColorBlockTags.begin();
    while (it != sameColorBlockTags.end())
    {
        int tags[] = {
            *it +100,
            *it -100,
            *it +1,
            *it -1,
        };
        
        for(int i =0; i < sizeof(tags) / sizeof(tags[0]); i++)
        {
            if (!hasSameColorBlock(sameColorBlockTags, tags[i]))
            {
                if (hasSameColorBlock(m_blockTag[blockType], tags[i]))
                {
                    sameColorBlockTags.push_back(tags[i]);
                }
            }
        }
        
        it++;
    }
    
    return sameColorBlockTags;
}

void GameScene::removeBlock(list<int> blockTags, kBlock blockType)
{
    bool first = true;
    
    list<int>::iterator it = blockTags.begin();
    while (it != blockTags.end()) {
        m_blockTag[blockType].remove(*it);
        
        Node *block = m_background->getChildByTag(*it);
        if(block)
        {
            ScaleTo* scale = ScaleTo::create(REMOVING_TIME, 0);
            CallFuncN* func = CallFuncN::create(this,callfuncN_selector(GameScene::removingBlock));
            
            FiniteTimeAction* sequence = Sequence::create(scale,func, NULL);
            
            FiniteTimeAction* action;
            
            if(first)
            {
                CCPlaySE* playSE = CCPlaySE::actionWithSoundFile(MP3_REMOVE_BLOCK, 0.1f, 0.1f, 0.1f);
                action = Spawn::create(sequence,playSE, NULL);
                
                first = false;
            }
            else
            {
                action = sequence;
            }
            
            block->runAction(action);
            
        }
        
        it++;
    }
    
    //SimpleAudioEngine::getInstance()->playEffect(MP3_REMOVE_BLOCK);
}

void GameScene::removingBlock(cocos2d::Node *block)
{
    block->removeFromParentAndCleanup(true);
}

GameScene::PositionIndex GameScene::getPositionIndex(int tag)
{
    int pos1_x = (tag - kTagBaseBlock) /100;
    int pos1_y = (tag - kTagBaseBlock) %100;
    
    return PositionIndex(pos1_x,pos1_y);
}

void GameScene::setNewPosition1(int tag, PositionIndex posIndex)
{
    BlockSprite* blockSprite = (BlockSprite*)m_background->getChildByTag(tag);
    int nextPosY = blockSprite->getNextPosY();
    if(nextPosY == 1)
    {
        nextPosY = posIndex.y;
    } else if (nextPosY < 0){
        nextPosY = posIndex.y+nextPosY;
    }
    CCLOG("posIndex.y:%d,nextPosY:%d",posIndex.y,nextPosY);
    
    blockSprite->setNextPos(posIndex.x, nextPosY);
}

void GameScene::searchNewPosition1(list<int> blocks)
{
    list<int>::iterator it1 = blocks.begin();
    while (it1 != blocks.end()) {
        PositionIndex posIndex1 = getPositionIndex(*it1);
        
        vector<kBlock>::iterator it2 = blockTypes.begin();
        while (it2 != blockTypes.end()) {
            
            list<int>::iterator it3 = m_blockTag[*it2].begin();
            while (it3 != m_blockTag[*it2].end()) {

                PositionIndex posIndex2 = getPositionIndex(*it3);
                
                if(posIndex1.x == posIndex2.x && posIndex1.y < posIndex2.y)
                {
                    setNewPosition1(*it3, posIndex2);
                }


                it3++;
            }
            
            it2++;
        }
        
        it1++;
    }
}

void GameScene::moveBlock()
{
    
    vector<kBlock>::iterator it1 = blockTypes.begin();
    while (it1 != blockTypes.end()) {
        
        list<int>::iterator it2 = m_blockTag[*it1].begin();
        while (it2 != m_blockTag[*it1].end())
        {
            BlockSprite* blockSprite = (BlockSprite*)m_background->getChildByTag(*it2);
            
            int nextPosX = blockSprite->getNextPosX();
            int nextPosY = blockSprite->getNextPosY();
            
            if (nextPosX != -1 || nextPosY != -1)
            {
                int newTag = getTag(nextPosX, nextPosY);
                blockSprite->initNextPos();
                blockSprite->setTag(newTag);
                
                *it2 = newTag;
                //CCLOG("x:%d,y:%d",nextPosX,nextPosY);
                MoveTo *move = MoveTo::create(MOVING_TIME, getPosition(nextPosX, nextPosY));
                blockSprite->runAction(move);
                
            }
            
            it2++;
        }
        
        it1++;
    }
}

void GameScene::movingBlockAnimation1(list<int> blocks)
{
    searchNewPosition1(blocks);
    
    moveBlock();
}















