#pragma once

#include "libidk/transform.hpp"

namespace idk
{
    class Camera;
}


class idk::Camera
{
public:
    Camera() = default;
    Camera(float aspect, float vfov=90.0f, float znear=0.01f, float zfar=1000.0f);

    void setFov        (float deg)   { vfov_   = deg;   projDirty_=true; }
    void setAspectRatio(float ratio) { aspect_ = ratio; projDirty_=true; }
    void setNearPlane  (float near)  { znear_  = near;  projDirty_=true; }
    void setFarPlane   (float far)   { zfar_   = far;   projDirty_=true; }

    Transform &getTransform() { viewDirty_=true; return transform_; }
    const Transform &getTransform() const { return transform_; }
    const glm::mat4 &getView() const;
    const glm::mat4 &getProj() const;
    glm::mat4 getProjView() const { return getProj() * getView(); }

    static idk::Camera getMixed(idk::Camera &start, idk::Camera &target, float alpha);

private:
    Transform transform_;

    float aspect_;
    float vfov_;
    float znear_;
    float zfar_;

    mutable glm::mat4 view_;
    mutable glm::mat4 proj_;
    mutable bool viewDirty_ = true;
    mutable bool projDirty_ = true;
};