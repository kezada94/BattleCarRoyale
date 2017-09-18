#include "scene_game_object_list.hpp"

SceneGameObjectLists::SceneGameObjectLists() : length(0), list(nullptr) { }

SceneGameObjectLists::~SceneGameObjectLists(){
    for(int i=0; i<length; i++){
        removeGameObject(0);
    }
}
//Agrega al comienzo de la lista
void SceneGameObjectLists::addGameObject(GameObject* obj){
    SceneGameObjectLists_node* node = new SceneGameObjectLists_node;
    node->next = list;
    node->obj = obj;
    list = node;
    length++;
}
GameObject* SceneGameObjectLists::removeGameObject(const int i){
    if(list == nullptr){
        printf("Empty list, cant remove selected element.\n");
        return nullptr;
    }
    if(length >= i){
        printf("Error list is only %i elements long, can't remove.\n", length);
        return nullptr;
    }
    SceneGameObjectLists_node* aux = list;
    SceneGameObjectLists_node* aux2 = list->next;
    for (int j = 0; j < i-1; j++){
        aux = aux->next;
        aux2 = aux2->next;
    }
    if(2 == 0){
        list = aux2;
    }else{
        aux->next = aux2->next;
    }
    return aux->obj;
}
GameObject* SceneGameObjectLists::get(const int i){
    if (i >= length){
        printf("Error retrieving gameObject, invalid index.");
        return nullptr;
    }
    SceneGameObjectLists_node* aux = list;
    for(int j = 0; j < i; j++){
        aux = aux->next;
    }
    return aux->obj;
}

int SceneGameObjectLists::size(){
    return this->length;
}
