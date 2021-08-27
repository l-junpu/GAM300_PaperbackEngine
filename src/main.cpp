#include "Paperback.h"

// Game Components
#include "Timer.h"
#include "Bullet.h"
#include "Rigidbody.h"
#include "Transform.h"

// Game Systems
#include "PhysicsSystem.h"
#include "ShipLogicSystem.h"
#include "BulletLogicSystem.h"
#include "ShipRenderingSystem.h"
#include "BulletRenderingSystem.h"
#include "BufferRefresh.h"


using namespace paperback;
void timer(int value);
void InitializeGame();


int main(int argc, char* argv[])
{
#if defined( PAPERBACK_DEBUG ) | defined( PAPERBACK_DEBUG )
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
    ShowWindow( GetConsoleWindow(), SW_SHOW );
#else
    ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif

    xcore::Init( "Starting xCore");


    InitializeGame();


    glutInitWindowSize(m_Engine.m_Width, m_Engine.m_Height);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutCreateWindow("Paperback Engine");
    glutDisplayFunc([]
        {
            m_Engine.m_Coordinator.Update();
        });
    glutReshapeFunc([](int w, int h)
        {
            m_Engine.m_Width = w;
            m_Engine.m_Height = h;
            glViewport(0, 0, w, h);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0, w, 0, h, -1, 1);
            glScalef(1, -1, 1);
            glTranslatef(0, -h, 0);
        });
    glutTimerFunc(0, timer, 0);
    glutMainLoop();


    xcore::Kill();
}












void timer(int value)
{
    UNREFERENCED_PARAMETER(value);
    glutPostRedisplay();
    glutTimerFunc(15, timer, 0);
}

void InitializeGame()
{
    // Register Components
    {
        m_Engine.m_Coordinator.RegisterComponents<
            Bullet,
            Rigidbody,
            Transform,
            Timer
        >();
    }

    // Register Systems
    {
        m_Engine.m_Coordinator.RegisterSystems<
            PhysicsSystem,
            ShipLogicSystem,
            BulletLogicSystem,
            ShipRenderingSystem,
            BulletRenderingSystem,
            BufferRefreshSystem
        >();
    }

    // Entity Creation
    {
        for (int i = 0; i < 1000; ++i)
        {
            m_Engine.m_Coordinator.CreateEntity( [&]( component::entity& Entity, Transform& transform, Rigidbody& rigidbody, Timer& timer )
                                                 {
                                                     Entity.m_GlobalIndex = 0;
                                                 
                                                     transform.m_Position.m_X = std::rand() % m_Engine.m_Width;
                                                     transform.m_Position.m_Y = std::rand() % m_Engine.m_Height;
                                                 
                                                     rigidbody.m_Velocity.m_X = (std::rand() / (float)RAND_MAX) - 0.5f;
                                                     rigidbody.m_Velocity.m_Y = (std::rand() / (float)RAND_MAX) - 0.5f;
                                                     rigidbody.m_Velocity.Normalize();
                                                 
                                                     timer.m_Timer = (std::rand() / (float)RAND_MAX) * 8;
                                                 });
        }
    }
}