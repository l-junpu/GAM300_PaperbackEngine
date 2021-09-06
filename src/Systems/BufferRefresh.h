#pragma once

struct BufferRefreshSystem : paperback::system::instance
{
    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "BufferRefreshSystem"
    };

    PPB_FORCEINLINE
    void OnSystemCreated( void ) noexcept
    {
        
    }

    PPB_FORCEINLINE
    void Update( void )
    {
        glutSwapBuffers();
        glClear(GL_COLOR_BUFFER_BIT);
    }
};