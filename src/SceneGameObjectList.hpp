#ifndef SCENE_GAME_OBJECT_LIST_H
#define SCENE_GAME_OBJECT_LIST_H

#include "GameObject.hpp"

typedef struct SceneGameObjectLists_node{
    GameObject* obj;
    SceneGameObjectLists_node* next;
}SceneGameObjectLists_node;

class SceneGameObjectLists{
public:
    SceneGameObjectLists();
    ~SceneGameObjectLists();

    void addGameObject(GameObject* obj);
    void clear();
    GameObject* removeGameObject(const int i);

    GameObject* get(const int i);
    int size();

private:
    SceneGameObjectLists_node* list;
    int length;
};
#endif
