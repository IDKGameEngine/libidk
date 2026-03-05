#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

// #define GLM_ENABLE_EXPERIMENTAL
// #include <glm/gtx/matrix_decopose.hpp>

struct Transform;
class Transformable;


struct Transform
{
private:
    friend class Transformable;
    glm::mat4 m4;
    glm::vec3 pos;
    glm::quat rot;
    float scale;
    bool dirty;

public:
    Transform(const glm::vec3 &p = glm::vec3(0.0f))
    :   m4(1.0f),
        pos(p),
        rot(glm::vec3(0.0f)),
        scale(1.0f),
        dirty(false)
    {

    }

    const glm::mat4 &to_mat4()
    {
        if (dirty)
        {
            m4 = glm::translate(glm::mat4(1.0f), pos);
            m4 = glm::rotate(m4, glm::radians(rot.x), glm::vec3(1,0,0));
            m4 = glm::rotate(m4, glm::radians(rot.y), glm::vec3(0,1,0));
            m4 = glm::rotate(m4, glm::radians(rot.z), glm::vec3(0,0,1));
            m4 = glm::scale(m4, glm::vec3(scale));
            dirty = false;
        }
        return m4;
    }

    glm::vec3 getPos() { return pos; }
    glm::quat getRot() { return rot; }
    float getScale() { return scale; }

    void setPos(const glm::vec3 &p)
    {
        dirty = true;
        pos = p;
    }

    void setRot(const glm::quat &q)
    {
        dirty = true;
        rot = q;
    }

    void setScale(float s)
    {
        dirty = true;
        scale = s;
    }

    void translate(float x, float y, float z)
    {
        dirty = true;
        pos += glm::vec3(x, y, z);
    }
    void translate(const glm::vec3 &delta)
    {
        dirty = true;
        pos += delta;
    }

    void rotate(float radTheta, const glm::vec3 &axis)
    {
        dirty = true;
        rot = glm::normalize(rot * glm::angleAxis(radTheta, axis));
    }
};



// class Transformable
// {
// private:
//     Transform mT;

// public:
//     Transform &getTransform() { return mT; }

//     const glm::vec3 &getPos() { return mT.pos; }
//     const glm::quat &getRot() { return mT.rot; }
//     float getScale() { return mT.scale; }

//     void setPos(const glm::vec3 &p) { mT.pos = p; }
//     void setRot(const glm::quat &q) { mT.rot = q; }
//     void setScale(float s) { mT.scale = s; }

//     void translate(float x, float y, float z) { mT.pos += glm::vec3(x, y, z); }
//     void translate(const glm::vec3 &delta) { mT.pos += delta; }

//     void rotate(float radTheta, const glm::vec3 &axis)
//     {
//         mT.rot = glm::normalize(mT.rot * glm::angleAxis(radTheta, axis));
//     }

// };

