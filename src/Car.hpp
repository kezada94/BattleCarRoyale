#ifndef CAR_H
#define CAR_H

#include "DynamicGameObject.hpp"

class Car : public DynamicGameObject{

private:
    float health;
    float armour;
    bool isAlive;
    bool turned;
    
    btRaycastVehicle* car;
    btDiscreteDynamicsWorld* world;
public:
    Car(const char* path, const char* texture_path, GLuint shaderprog, btScalar masa, btVector3 startPos, btQuaternion startRot);
    Car(const char* path, const char* texture_path, GLuint shaderprog, btScalar masa, btVector3 startPos, btQuaternion startRot, btCollisionShape* coll);
    ~Car();

    virtual void initialize(btDiscreteDynamicsWorld* world) = 0;
    virtual void updatePhysics() = 0;

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
    bool getTurned();
    bool getBrake();
    btRaycastVehicle* getCar();
    btDiscreteDynamicsWorld* getWorld();
        

    void setHealth(float vida);
    void setArmour(float armor);
    void setIsAlive(bool isAliv);
    void setTurned(bool turn);
    void setBrake(bool brake);
    void setCar(btRaycastVehicle* car);
    void setWorld(btDiscreteDynamicsWorld* world);
    

};

#endif