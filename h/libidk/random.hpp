#pragma once

#include <cmath>


namespace idk
{
    inline float randf()
    {
        return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    }

    inline float randf( float min, float max )
    {
        return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX/(max-min)));
    }

    inline float randf_guassian(float scale, int n)
    {
        // 1.0   --> 1.0
        // 1.5   --> 1.0 + 0.5
        // 1.75  --> 1.0 + 0.5 + 0.25
        // 1.875 --> 1.0 + 0.5 + 0.25 + 0.125

        float result  = 0.0f;
        float width   = scale;
        float maximum = 0.0f;

        for (int i=1; i<=n; i++)
        {
            result  += width * idk::randf(-1.0f, +1.0f);
            maximum += width;
            width   /= 2.0f;
        }

        return scale * (result / maximum);
    }

    inline void randvec2(float min, float max, float *out)
    {
        out[0] = idk::randf(min, max);
        out[1] = idk::randf(min, max);
    }

    inline void randvec3(float min, float max, float *out)
    {
        idk::randvec2(min, max, out);
        out[2] = idk::randf(min, max);
    }

    inline void randvec4(float min, float max, float *out)
    {
        idk::randvec3(min, max, out);
        out[3] = idk::randf(min, max);
    }

};
