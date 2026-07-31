#pragma once

#include "libidk/basis.hpp"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace idk
{
    class Transform;
}


class idk::Transform
{
public:
    Transform() = default;
 
    Transform( const glm::vec3 &pos,
               const glm::quat &rot = glm::identity<glm::quat>(),
               const glm::vec3 &scale = glm::vec3(1.0f) );
 
    // -------------------------------------------------------------------------
    //  Getters
    // -------------------------------------------------------------------------
    const glm::vec3& GetPosition() const { return pos_;   }
    const glm::quat& GetRotation() const { return rot_;   }
    const glm::vec3& GetScale()    const { return scale_; }
 
    glm::vec3 GetForward() const { return rot_ * world_axis::FORWARD; }
    glm::vec3 GetRight()   const { return rot_ * world_axis::RIGHT;   }
    glm::vec3 GetUp()      const { return rot_ * world_axis::UP;      }
 
    // -------------------------------------------------------------------------
    //  Setters
    // -------------------------------------------------------------------------
    void SetPosition(const glm::vec3 &pos)   { pos_ = pos; dirty_ = true; }
    void SetRotation(const glm::quat &rot)   { rot_ = glm::normalize(rot); dirty_ = true; }
    void SetScale   (const glm::vec3 &scale) { scale_ = scale; dirty_ = true; }
 
    void SetPosition(float x, float y, float z) { SetPosition({x, y, z}); }
    void SetScale   (float s)                   { SetScale({s, s, s}); }
 
    // -------------------------------------------------------------------------
    //  Translation
    // -------------------------------------------------------------------------
    void Translate(const glm::vec3 &delta);
 
    void TranslateLocal(const glm::vec3 &delta);

    // -------------------------------------------------------------------------
    //  Rotation — world space (pre-multiply: delta * q)
    //  Rotates around a world axis, regardless of current orientation.
    //  e.g. always spin around world Y like a compass needle.
    // -------------------------------------------------------------------------
    void RotateWorld(float angleDeg, const glm::vec3 &worldAxis);
    void YawWorld  (float deg) { RotateWorld(deg, world_axis::UP);      }
    void PitchWorld(float deg) { RotateWorld(deg, world_axis::RIGHT);   }
    void RollWorld (float deg) { RotateWorld(deg, world_axis::FORWARD); }
 
    // -------------------------------------------------------------------------
    //  Rotation — local space (post-multiply: q * delta)
    //  Rotates around the object's own axes.
    //  e.g. an airplane rolling along its own fuselage axis.
    // -------------------------------------------------------------------------
    void RotateLocal(float angleDeg, const glm::vec3 &localAxis);
    void YawLocal  (float deg) { RotateLocal(deg, world_axis::UP);      }
    void PitchLocal(float deg) { RotateLocal(deg, world_axis::RIGHT);   }
    void RollLocal (float deg) { RotateLocal(deg, world_axis::FORWARD); }
 
    void SetEulerAngles(float pitchDeg, float yawDeg, float rollDeg); // Y->X->Z order
    void LookAt(const glm::vec3 &target, const glm::vec3 &up = world_axis::UP);
 
    const glm::mat4& GetModelMatrix() const;
 
    static Transform Mix(const Transform &x, const Transform &y, float a);
 
private:
    glm::vec3 pos_;
    glm::quat rot_;
    glm::vec3 scale_;
 
    mutable glm::mat4 model_ = glm::mat4(1.0f);
    mutable bool      dirty_ = true;
};
