#pragma once

#include "libidk/platform/IPlatformVideo.hpp"


namespace idk
{
    class SDL3Video: public IPlatformVideo
    {
    private:
        const char      *mTitle;
        void            *mWin;
        void            *mGl;
        int32_t          mWidth;
        int32_t          mHeight;

    public:
        SDL3Video(const char *title, int w, int h);
        ~SDL3Video();

        virtual void  update(PlatformContext&) final;

        virtual void *getImpl()   final { return mWin; }
        virtual int   getWidth()  final { return mWidth; };
        virtual int   getHeight() final { return mHeight; };

        virtual void  setWindowVisibility(bool visible) final;
        virtual void  setWindowResolution(int w, int h) final;
        virtual void  setRenderResolution(int w, int h) final;

    };
}

