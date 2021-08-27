#pragma once

struct BufferRefreshSystem : paperback::system::instance
{
    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "BufferRefreshSystem"
    };

    PPB_FORCEINLINE
    void Execute( paperback::coordinator::instance& Coordinator )
    {
        UNREFERENCED_PARAMETER( Coordinator );

        glutSwapBuffers();
        glClear(GL_COLOR_BUFFER_BIT);
    }
};