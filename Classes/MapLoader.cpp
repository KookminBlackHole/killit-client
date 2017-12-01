#include "MapLoader.h"

#include "Utils.h"
#include "ZOrder.h"
#include "Definitions.h"

USING_NS_CC;
using namespace std;

MapLoader MapLoader::instance;

MapLoader *MapLoader::getInstance() {
    return &instance;
}

void MapLoader::loadData(const string &path) {
    /// 맵 파일 읽음
    auto mapFileString = FileUtils::getInstance()->getStringFromFile(path);
    int **tempData, idx = -1;
    for (auto i : split(mapFileString, '\n')) {
        auto data = split(i, ' ');
        if (idx < 0) {
            width = toInt(data[0]);
            height = toInt(data[1]);
            
            mapData = new int*[height];
            tempData = new int*[height];
            idx++;
        } else {
            mapData[(idx / height)] = new int[width];
            tempData[(idx / height)] = new int[width];
            for (auto j : data) {
                tempData[(idx / height)][(idx % width)] = toInt(j);
                idx++;
            }	
        }
    }
    
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            mapData[height - i - 1][j] = tempData[i][j];
        }
    }
    
    for (int i = 0; i < height; i++) delete[] tempData[i];
    delete[] tempData;
}

void MapLoader::createMap(Node *scene, GameObject ****outTile, Sprite ****outFog) {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = visibleSize / 2;

    int zorder = ZORDER::WALL;
    /// 맵 타일 초기화 및 맵 데이터 매핑
    (*outTile) = new GameObject**[height];
    (*outFog) = new Sprite**[height];
    for (int i = 0; i < height; i++) {
        (*outTile)[i] = new GameObject*[width];
        (*outFog)[i] = new Sprite*[width];
        for (int j = 0; j < width; j++) {
            Vec2 pos = Vec2(j * TILE_SIZE + origin.x - TILE_SIZE * width * 0.5,
                            i * TILE_SIZE + origin.y - TILE_SIZE * height * 0.5);
            if (mapData[i][j] <= 10) {/// 게임 오브젝트 생성
                (*outTile)[i][j] = GameObject::create("res/tile0.png", mapData[i][j]);
                (*outTile)[i][j]->setSolidObject(false);
                (*outTile)[i][j]->setZOrder(zorder - 1000);
                if (mapData[i][j] == 1) {
                    auto door = Sprite::create("res/tile2.png");
                    door->getTexture()->setAliasTexParameters();
                    door->setAnchorPoint(Vec2(0.5f, 0.25f));
                    door->setGlobalZOrder(zorder);
                    (*outTile)[i][j]->addChild(door);
                    
                    (*outTile)[i][j]->setSolidObject(true);
                    (*outTile)[i][j]->setSolidArea(Rect(-Vec2(TILE_SIZE_HALF, TILE_SIZE_HALF), { TILE_SIZE, TILE_SIZE }));
                    (*outTile)[i][j]->type = mapData[i][j];
                }
            } else { /// 맵 타일
                int idx = mapData[i][j] - 11;
                
                auto sf = SpriteFrame::create("res/tileset_wall2.png", Rect(REAL_TILE_WIDTH * (idx % 7), REAL_TILE_HEIGHT * (idx / 7), REAL_TILE_WIDTH, REAL_TILE_HEIGHT));
                
                (*outTile)[i][j] = GameObject::create(sf, mapData[i][j]);
                (*outTile)[i][j]->setSolidObject(true);
                (*outTile)[i][j]->setSolidArea(Rect(-Vec2(TILE_SIZE_HALF, TILE_SIZE_HALF), { TILE_SIZE, TILE_SIZE }));
                (*outTile)[i][j]->image->setAnchorPoint(Vec2(0.5f, 0.25f));
                (*outTile)[i][j]->setZOrder(zorder);
            }
            
            (*outTile)[i][j]->setPosition(pos);
            (*outTile)[i][j]->setVisible(false);
            scene->addChild((*outTile)[i][j]);
            
            /// 맵 안개 생성
            (*outFog)[i][j] = Sprite::create("res/tile6.png");
            (*outFog)[i][j]->setGlobalZOrder(ZORDER::FOG);
            (*outFog)[i][j]->getTexture()->setAliasTexParameters();
            (*outFog)[i][j]->setScale(2);
            (*outFog)[i][j]->setPosition(pos.x, pos.y + TILE_SIZE);
            (*outFog)[i][j]->setVisible(false);
            		
            scene->addChild((*outFog)[i][j]);
        }
        zorder -= 1;
    }
}
