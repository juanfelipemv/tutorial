#include "PersonajeJugador.h"

PersonajeJugador::PersonajeJugador(int x, int y,
                                    SDL_Renderer* renderer,
                                    list<Personaje*>*personajes)
{
    this->rectangulo.x = x;
    this->rectangulo.y = y;
    this->personajes=personajes;

    SDL_Texture *textura = IMG_LoadTexture(renderer, "assets/personaje/down1.png");
    SDL_QueryTexture(textura, NULL, NULL, &this->rectangulo.w, &this->rectangulo.h);
    texturas_down.push_back(textura);

    texturas_down.push_back(IMG_LoadTexture(renderer, "assets/personaje/down2.png"));

    texturas_up.push_back(IMG_LoadTexture(renderer, "assets/personaje/up1.png"));
    texturas_up.push_back(IMG_LoadTexture(renderer, "assets/personaje/up2.png"));

    texturas_left.push_back(IMG_LoadTexture(renderer, "assets/personaje/left1.png"));
    texturas_left.push_back(IMG_LoadTexture(renderer, "assets/personaje/left2.png"));

    texturas_right.push_back(IMG_LoadTexture(renderer, "assets/personaje/right1.png"));
    texturas_right.push_back(IMG_LoadTexture(renderer, "assets/personaje/right2.png"));

    orientacion="down";

    textura_actual=texturas_down.begin();

    rayo_texture = IMG_LoadTexture(renderer,"assets/rayo.png");
    SDL_QueryTexture(rayo_texture, NULL, NULL,
                        &this->rayo_rect.w,
                        &this->rayo_rect.h);
    rayo_activado=false;

    rayo_cooldown = 100;
    rayo_frame_actual = 0;
}

PersonajeJugador::~PersonajeJugador()
{
    //dtor
}

void PersonajeJugador::logic(Uint8* teclas_presionadas)
{
    SDL_Rect temp = this->rectangulo;

    if(rayo_activado)
        rayo_frame_actual++;
    if(rayo_frame_actual>=rayo_cooldown)
        rayo_activado=false;

    if(teclas_presionadas[SDL_SCANCODE_SPACE] && !rayo_activado)
    {
        rayo_activado=true;
        rayo_frame_actual=0;
        rayo_rect.x = rectangulo.x;
        rayo_rect.y = rectangulo.y;
        if(orientacion=="right")
        {
            rayo_rect.x+=rectangulo.w;
        }
        if(orientacion=="left")
        {
            rayo_rect.x-=rectangulo.w;
        }
        if(orientacion=="up")
        {
            rayo_rect.y-=rectangulo.h;
        }
        if(orientacion=="down")
        {
            rayo_rect.y+=rectangulo.h;
        }
        rayo_orientacion=orientacion;
    }

    if( teclas_presionadas[ SDL_SCANCODE_UP ] )
    {
        rectangulo.y-=1;
        if(orientacion!="up")
            textura_actual=texturas_up.begin();
        orientacion="up";
    }
    if( teclas_presionadas[ SDL_SCANCODE_DOWN ] )
    {
        rectangulo.y+=1;
        if(orientacion!="down")
            textura_actual=texturas_down.begin();
        orientacion="down";
    }
    if( teclas_presionadas[ SDL_SCANCODE_RIGHT ] )
    {
        rectangulo.x+=1;
        if(orientacion!="right")
            textura_actual=texturas_right.begin();
        orientacion="right";
    }
    if( teclas_presionadas[ SDL_SCANCODE_LEFT ] )
    {
        rectangulo.x-=1;
        if(orientacion!="left")
            textura_actual=texturas_left.begin();
        orientacion="left";
    }

    for(list<Personaje*>::iterator i = personajes->begin();
            i!=personajes->end();
            i++)
    {
        if(this==(*i))
            continue;
        if(colision(this->rayo_rect, (*i)->rectangulo))
        {
            //rectangulo=temp;
            personajes->erase(i);
            rayo_activado=false;
            break;
        }
    }

    if(rayo_activado)
    {
        if(orientacion=="up")
        {
            rayo_rect.y-=2;
        }
        if(orientacion=="down")
        {
            rayo_rect.y+=2;
        }
        if(orientacion=="left")
        {
            rayo_rect.x-=2;
        }
        if(orientacion=="right")
        {
            rayo_rect.x+=2;
        }
    }
}

void PersonajeJugador::render(SDL_Renderer* renderer)
{
    Personaje::render(renderer);
    if(rayo_activado)
    {
        SDL_RenderCopy(renderer,rayo_texture, NULL,&rayo_rect);
    }
}




