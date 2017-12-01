#ifndef _MAP_LOADER_H_
#define _MAP_LOADER_H_

#include "cocos2d.h"

#include <string>
#include "GameObject.h"

class MapLoader {
public:
    static MapLoader *getInstance();
    
    void loadData(const std::string &path);
    
    void createMap(cocos2d::Node *scene, GameObject ****outTile, cocos2d::Sprite ****outFog);
    
    int getWidth() const { return width; }
    int getHeight() const { return height; }
	int getMapData(int x, int y) const { return mapData[y][x]; }

	void setMapData(int x, int y, int value) { mapData[y][x] = value; }
    
private:
    MapLoader() {}
    virtual ~MapLoader() {}
    
    static MapLoader instance;
    
    int **mapData;
    
    int width, height;

};

#endif
