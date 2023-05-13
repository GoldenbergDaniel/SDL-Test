#define OBJECT

typedef struct Object
{
    u16 width;
    u16 height;
    SDL_Color color;
    v2 pos;
    v2 vel;
    v2 acc;
} Object;

Object new_object(u16 width, u16 height, SDL_Color color);
void init_object(Object *object);
void update_object(Object *object, f64 t, f64 dt);
void draw_object(Object *object, SDL_Renderer *renderer);

