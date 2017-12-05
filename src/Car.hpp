#ifndef CAR_H
#define CAR_H

#include "DynamicGameObject.hpp"
#include "SoundManager.hpp"
#include "Spotlight.hpp"
#include "ParticleManager.hpp"


extern double lastTime;
extern double deltaTime;
extern double currentTime;

class Car : public DynamicGameObject{
private:
    float health;
    float armour;
    
    bool isAlive;
    bool turned;
    
    btDiscreteDynamicsWorld* world;
protected:
    btRaycastVehicle* car;
    float fireRate;     // (shots/seconds)
    double lastShot;
    
public:
    Car(const char* path, const char* texture_path, const char* normal_path, GLuint shaderprog, btScalar masa, btVector3 startPos, btQuaternion startRot, glm::vec3 specular, GLuint specular_loc);
    Car(const char* path, const char* texture_path, const char* normal_path, GLuint shaderprog, btScalar masa, btVector3 startPos, btQuaternion startRot, glm::vec3 specular, GLuint specular_loc, btCollisionShape* coll);
    ~Car();

    glm::mat4 model;
    Spotlight* frontLight1;
    Spotlight* frontLight2;

    btVector3 dirr;
    
    SoundManager* sound;
    ParticleManager* particleManager;

    virtual void initialize(btDiscreteDynamicsWorld* world) = 0;
    virtual void updatePhysics() = 0;

    virtual void accelerate() = 0;
    virtual void brake() = 0;
    virtual void reverse() = 0;
    virtual void turnLeft() = 0;
    virtual void turnRight() = 0;

    virtual void fire() = 0;

    virtual void spawn() = 0;
    virtual void despawn(btDiscreteDynamicsWorld* world) = 0;

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
    
    void setSoundManager(SoundManager* sound);

};

#endif