#pragma once

struct BufferRefreshSystem : paperback::system::instance
{
    PPB_FORCEINLINE
    void Execute( paperback::coordinator::instance& Coordinator )
    {
        UNREFERENCED_PARAMETER( Coordinator );

        glutSwapBuffers();
        glClear(GL_COLOR_BUFFER_BIT);
    }
};