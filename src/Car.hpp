#ifndef CAR_H
#define CAR_H

#include "DynamicGameObject.hpp"

class Car : public DynamicGameObject{

private:
    float health;
    float armour;
    bool isAlive;
    
    btRaycastVehicle* car;
public:
    Car(const char* path, const char* texture_path, GLuint shaderprog, btScalar masa, btVector3 startPos, btQuaternion startRot);
    Car(const char* path, const char* texture_path, GLuint shaderprog, btScalar masa, btVector3 startPos, btQuaternion startRot, btCollisionShape* coll);
    ~Car();

    virtual void initialize(btScalar hp, btScalar brake, btScalar sterring) = 0;

    virtual void accelerate() = 0;
    virtual void brake() = 0;
    virtual void reverse() = 0;
    virtual void turnLeft() = 0;
    virtual void turnRight() = 0;

    virtual void fire() = 0;

    virtual void spawn() = 0;
    virtual void despawn() = 0;

    float getHealth();
    float getArmour();
    bool getIsAlive();
    btRaycastVehicle* getCar();

    void setHealth(float vida);
    void setArmour(float armor);
    void setIsAlive(float isAliv);
    void setCar(btRaycastVehicle* car);

};

#endif