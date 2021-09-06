#include "Paperback.h"

//-----------------------------------
//       Component & Systems
//-----------------------------------
#include "Components/component_includes.h"
#include "Systems/system_includes.h"

//-----------------------------------
//      Forward Declarations
//-----------------------------------
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

    // Initialization
    xcore::Init( "Starting xCore");
    InitializeGame();

    // Game Loop - To be replaced with purely PPB.Update()
    // when rendering systems is up
    glutInitWindowSize( m_Engine.m_Width, m_Engine.m_Height );
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE );
    glutCreateWindow( "Paperback Engine" );
    glutDisplayFunc([]
    {
        PPB.Update();
    });
    glutReshapeFunc([](int w, int h)
    {
        m_Engine.m_Width = w;
        m_Engine.m_Height = h;
        glViewport( 0, 0, w, h );
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        glOrtho( 0, w, 0, h, -1, 1 );
        glScalef( 1, -1, 1 );
        glTranslatef( 0, -h, 0 );
    });
    glutTimerFunc( 0, timer, 0 );
    glutMainLoop();

    // Termination
    xcore::Kill();
}












void timer( int value )
{
    UNREFERENCED_PARAMETER( value );
    glutPostRedisplay();
    glutTimerFunc( 15, timer, 0 );
}

void InitializeGame()
{
    // Register Components
    {
       PPB.RegisterComponents<
            Bullet,
            Rigidbody,
            Transform,
            Timer
        >();
    }

    // Register Systems
    {
        PPB.RegisterSystems<
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
        for (int i = 0; i < 4000; ++i)
        {
            PPB.CreateEntity( [&]( component::entity& Entity, Transform& transform, Rigidbody& rigidbody, Timer& timer )
                              {
                                  Entity.m_GlobalIndex = 0;
                              
                                  transform.m_Position.m_X = std::rand() % m_Engine.m_Width;
                                  transform.m_Position.m_Y = std::rand() % m_Engine.m_Height;
                              
                                  rigidbody.m_Velocity.m_X = ( std::rand() % 40 );
                                  rigidbody.m_Velocity.m_Y = ( std::rand() % 40 );
                              
                                  timer.m_Timer = (std::rand() / (float)RAND_MAX) * 8;
                              });
        }
    }
}