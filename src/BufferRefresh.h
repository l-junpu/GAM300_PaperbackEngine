#pragma once

struct BufferRefreshSystem : paperback::system::instance
{
    __inline
    void Execute( void )
    {
        glutSwapBuffers();
        glClear(GL_COLOR_BUFFER_BIT);
    }
};