#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#undef main
#include<math.h>
#include<vector>
using namespace std;

struct box{
    struct point{
        vector<double> points;
        vector<point*> connections;
    };
    vector<point*> points;
    box(int dim){
        for( int i = 0; i < pow(2,dim); ++i ){
            point p;
            for( int j = 0; j < dim; ++j ){
                if( j % 2 == 0 ){

                }
            }
        }
    }
};

int main(){
    const SDL_VideoInfo* info = 0;
    if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) return 1;
    if( !(info = SDL_GetVideoInfo( ))) return 2;
    int bpp = info->vfmt->BitsPerPixel;

    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 6 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 6 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    SDL_Surface* screen;
    int width = 640, height = 480;
    if( !(screen=SDL_SetVideoMode( width, height, 16, SDL_OPENGL ))) return 3;

    glOrtho( 0, width, height, 0, -pow( 2, 15 ) , pow( 2, 15 ) );


    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );

    while( true ){
        usleep(10000);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
        glClearColor( 1,1,1,1 );
        glColor3ub(255,255,255);

    }
}
