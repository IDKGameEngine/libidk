#include "libidk/transform.hpp"


idk::Transform::Transform(const glm::vec3 &pos, const glm::quat &rot, const glm::vec3 &scale)
:   pos_(pos), rot_(rot), scale_(scale)
{

}
 

void idk::Transform::Translate(const glm::vec3 &delta)
{
    pos_ += delta;
    dirty_ = true;
}

void idk::Transform::TranslateLocal(const glm::vec3 &delta)
{
    pos_ += rot_ * delta;
    dirty_ = true;
}

void idk::Transform::RotateWorld(float angleDeg, const glm::vec3 &worldAxis)
{
    glm::quat delta = glm::angleAxis(glm::radians(angleDeg), glm::normalize(worldAxis));
    rot_ = glm::normalize(delta * rot_);
    dirty_ = true;
}

void idk::Transform::RotateLocal(float angleDeg, const glm::vec3 &localAxis)
{
    glm::quat delta = glm::angleAxis(glm::radians(angleDeg), glm::normalize(localAxis));
    rot_ = glm::normalize(rot_ * delta);
    dirty_ = true;
}


void idk::Transform::SetEulerAngles(float pitchDeg, float yawDeg, float rollDeg)
{
    rot_ = glm::normalize(
        glm::angleAxis(glm::radians(yawDeg),   world_axis::UP)
        * glm::angleAxis(glm::radians(pitchDeg), world_axis::RIGHT)
        * glm::angleAxis(glm::radians(rollDeg),  world_axis::FORWARD)
    );
    dirty_ = true;
}


void idk::Transform::LookAt(const glm::vec3 &target, const glm::vec3 &up)
{
    glm::vec3 dir = target - pos_;
    if (glm::length(dir) < 1e-6f)
        return;
    dir  = glm::normalize(dir);
    rot_ = glm::normalize(glm::quatLookAt(dir, up));
    dirty_ = true;
}


const glm::mat4 &idk::Transform::GetModelMatrix() const
{
    if (dirty_)
    {
        model_ = glm::translate(glm::mat4(1.0f), pos_)
                * glm::mat4_cast(rot_)
                * glm::scale(glm::mat4(1.0f), scale_);
        dirty_ = false;
    }
    return model_;
}


idk::Transform idk::Transform::Mix(const Transform &x, const Transform &y, float a)
{
    return Transform(
        glm::mix(x.pos_, y.pos_, a),
        glm::slerp(x.rot_, y.rot_, a),
        glm::mix(x.scale_, y.scale_, a)
    );
}

