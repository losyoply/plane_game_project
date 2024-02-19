// [main.c]
// this template is provided for the 2D shooter game.

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <math.h>

// If defined, logs will be shown on console and written to file.
// If commented out, logs will not be shown nor be saved.
#define LOG_ENABLED
bool rank_stored = false;
int dirapple;
int bossappeared;
int usedskill;
int BOSSPOINT;
/* Constants. */
int volok = 1;
int point;
char pointstr[];
char hpstr[];
char bosshpstr[];
char angerstr[];
// Frame rate (frame per second)
const int FPS = 60;
// Display (screen) width.
const int SCREEN_W = 800;
// Display (screen) height.
const int SCREEN_H = 600;
// At most 10 audios can be played at a time.
const int RESERVE_SAMPLES = 10;
// Same as:
// const int SCENE_MENU = 1;
// const int SCENE_START = 2;
enum {
    SCENE_MENU = 1,
    SCENE_START = 2,
    // [HACKATHON 3-7]
    // TODO: Declare a new scene id.
    SCENE_SETTINGS = 3,
    SCENE_GAMEOVER = 4,
    SCENE_CHAR = 5,
    SCENE_HOWTO = 6,
    SCENE_WIN = 7
};

/* Input states */

// The active scene id.
int active_scene;
// Keyboard state, whether the key is down or not.
bool key_state[ALLEGRO_KEY_MAX];
// Mouse state, whether the key is down or not.
// 1 is for left, 2 is for right, 3 is for middle.
bool *mouse_state;
// Mouse position.
int mouse_x, mouse_y;
// TODO: More variables to store input states such as joysticks, ...

/* Variables for allegro basic routines. */

ALLEGRO_DISPLAY* game_display;
ALLEGRO_EVENT_QUEUE* game_event_queue;
ALLEGRO_TIMER* game_update_timer;

/* Shared resources*/

ALLEGRO_FONT* font_pirulen_32;
ALLEGRO_FONT* font_pirulen_24;
// TODO: More shared resources or data that needed to be accessed
// across different scenes.

/* Menu Scene resources*/
ALLEGRO_BITMAP* main_img_background;
// [HACKATHON 3-1]
// TODO: Declare 2 variables for storing settings images.
// Uncomment and fill in the code below.
ALLEGRO_BITMAP* img_settings;
ALLEGRO_BITMAP* img_settings2;
ALLEGRO_BITMAP* img_vol;
ALLEGRO_BITMAP* img_vol2;
ALLEGRO_BITMAP* img_howtoplay;
ALLEGRO_BITMAP* img_howtoplay_bg;

ALLEGRO_SAMPLE* main_bgm;
ALLEGRO_SAMPLE_ID main_bgm_id;
ALLEGRO_BITMAP* img_char;
ALLEGRO_BITMAP* img_char1;
ALLEGRO_BITMAP* img_char2;
ALLEGRO_BITMAP* img_char3;

ALLEGRO_SAMPLE* gun_bgm;
ALLEGRO_SAMPLE_ID gun_bgm_id;
ALLEGRO_SAMPLE* skill_bgm;
ALLEGRO_SAMPLE_ID skill_bgm_id;

/* Start Scene resources*/
ALLEGRO_BITMAP* start_img_background;
ALLEGRO_BITMAP* start_img_plane;
ALLEGRO_BITMAP* start_img_enemy;
ALLEGRO_BITMAP* img_boss;
ALLEGRO_SAMPLE* start_bgm;
ALLEGRO_SAMPLE_ID start_bgm_id;
//by me
/* GAMEOVER Scene resources*/
ALLEGRO_BITMAP* gameover_img_background;
ALLEGRO_SAMPLE* gameover_bgm;
ALLEGRO_SAMPLE_ID gameover_bgm_id;
ALLEGRO_SAMPLE* bossdie_bgm;
ALLEGRO_SAMPLE_ID bossdie_bgm_id;
//setting resources
ALLEGRO_SAMPLE* setting_bgm;
ALLEGRO_SAMPLE_ID setting_bgm_id;
//char resources
ALLEGRO_SAMPLE* char_bgm;
ALLEGRO_SAMPLE_ID char_bgm_id;
//by me
// [HACKATHON 2-1]
// TODO: Declare a variable to store your bullet's image.
// Uncomment and fill in the code below.
ALLEGRO_BITMAP* img_bullet;
ALLEGRO_BITMAP* img_skill;
ALLEGRO_BITMAP* img_sword;
//win scene
ALLEGRO_BITMAP* img_win;
ALLEGRO_SAMPLE* win_bgm;
ALLEGRO_SAMPLE_ID win_bgm_id;

ALLEGRO_BITMAP* img_bossbull;
ALLEGRO_BITMAP* img_bigapple;
typedef struct {
    // The center coordinate of the image.
    float x, y;
    // The width and height of the object.
    float w, h;
    // The velocity in x, y axes.
    float vx, vy;
    // Should we draw this object on the screen.
    bool hidden;
    float v;
    int hp;
    float angerpoint;
    // The pointer to the object’s image.
    ALLEGRO_BITMAP* img;
} MovableObject;
void draw_movable_object(MovableObject obj);
#define MAX_ENEMY 10
// [HACKATHON 2-2]
// TODO: Declare the max bullet count that will show on screen.
// You can try max 4 bullets here and see if you needed more.
// Uncomment and fill in the code below.
#define MAX_BULLET 5
#define MAX_APPLE 15
#define MAX_BIG 4
MovableObject plane;
MovableObject boss;
MovableObject enemies[MAX_ENEMY];
MovableObject apples[MAX_APPLE];
MovableObject bigapple[MAX_BIG];
// [HACKATHON 2-3]
// TODO: Declare an array to store bullets with size of max bullet count.
// Uncomment and fill in the code below.
MovableObject bullets[MAX_BULLET+100];
MovableObject skill;
MovableObject sword;
MovableObject sword2;

// [HACKATHON 2-4]
// TODO: Set up bullet shooting cool-down variables.
// 1) Declare your shooting cool-down time as constant. (0.2f will be nice)
// 2) Declare your last shoot timestamp.
// Uncomment and fill in the code below.
const float MAX_COOLDOWN = 0.2f;
const float BOSS_COOL_DOWN = 0.2f;
const float BIG_COOL_DOWN = 0.5f;

double last_shoot_timestamp;
double boss_last_shoot;
double big_last_shoot;


/* Declare function prototypes. */

// Initialize allegro5 library
void allegro5_init(void);
// Initialize variables and resources.
// Allows the game to perform any initialization it needs before
// starting to run.
void game_init(void);
// Process events inside the event queue using an infinity loop.
void game_start_event_loop(void);
// Run game logic such as updating the world, checking for collision,
// switching scenes and so on.
// This is called when the game should update its logic.
void game_update(void);
// Draw to display.
// This is called when the game should draw itself.
void game_draw(void);
// Release resources.
// Free the pointers we allocated.
void game_destroy(void);
// Function to change from one scene to another.
void game_change_scene(int next_scene);
// Load resized bitmap and check if failed.
ALLEGRO_BITMAP *load_bitmap_resized(const char *filename, int w, int h);
// [HACKATHON 3-2]
// TODO: Declare a function.
// Determines whether the point (px, py) is in rect (x, y, w, h).
// Uncomment the code below.
bool pnt_in_rect(int px, int py, int x, int y, int w, int h);

/* Event callbacks. */
void on_key_down(int keycode);
void on_mouse_down(int btn, int x, int y);

/* Declare function prototypes for debugging. */

// Display error message and exit the program, used like 'printf'.
// Write formatted output to stdout and file from the format string.
// If the program crashes unexpectedly, you can inspect "log.txt" for
// further information.
void game_abort(const char* format, ...);
// Log events for later debugging, used like 'printf'.
// Write formatted output to stdout and file from the format string.
// You can inspect "log.txt" for logs in the last run.
void game_log(const char* format, ...);
// Log using va_list.
void game_vlog(const char* format, va_list arg);

int main(int argc, char** argv) {
    // Set random seed for better random outcome.
    srand(time(NULL));
    allegro5_init();
    game_log("Allegro5 initialized");
    game_log("Game begin");
    // Initialize game variables.
    game_init();
    game_log("Game initialized");
    // Draw the first frame.
    game_draw();
    game_log("Game start event loop");
    // This call blocks until the game is finished.
    game_start_event_loop();
    game_log("Game end");
    game_destroy();
    return 0;
}

void allegro5_init(void) {
    if (!al_init())
        game_abort("failed to initialize allegro");

    // Initialize add-ons.
    if (!al_init_primitives_addon())
        game_abort("failed to initialize primitives add-on");
    if (!al_init_font_addon())
        game_abort("failed to initialize font add-on");
    if (!al_init_ttf_addon())
        game_abort("failed to initialize ttf add-on");
    if (!al_init_image_addon())
        game_abort("failed to initialize image add-on");
    if (!al_install_audio())
        game_abort("failed to initialize audio add-on");
    if (!al_init_acodec_addon())
        game_abort("failed to initialize audio codec add-on");
    if (!al_reserve_samples(RESERVE_SAMPLES))
        game_abort("failed to reserve samples");
    if (!al_install_keyboard())
        game_abort("failed to install keyboard");
    if (!al_install_mouse())
        game_abort("failed to install mouse");
    // TODO: Initialize other addons such as video, ...

    // Setup game display.
    game_display = al_create_display(SCREEN_W, SCREEN_H);
    if (!game_display)
        game_abort("failed to create display");
    al_set_window_title(game_display, "I2P(I)_2020 Final Project <109062306>");

    // Setup update timer.
    game_update_timer = al_create_timer(1.0f / FPS);
    if (!game_update_timer)
        game_abort("failed to create timer");

    // Setup event queue.
    game_event_queue = al_create_event_queue();
    if (!game_event_queue)
        game_abort("failed to create event queue");

    // Malloc mouse buttons state according to button counts.
    const unsigned m_buttons = al_get_mouse_num_buttons();
    game_log("There are total %u supported mouse buttons", m_buttons);
    // mouse_state[0] will not be used.
    mouse_state = malloc((m_buttons + 1) * sizeof(bool));
    memset(mouse_state, false, (m_buttons + 1) * sizeof(bool));

    // Register display, timer, keyboard, mouse events to the event queue.
    al_register_event_source(game_event_queue, al_get_display_event_source(game_display));
    al_register_event_source(game_event_queue, al_get_timer_event_source(game_update_timer));
    al_register_event_source(game_event_queue, al_get_keyboard_event_source());
    al_register_event_source(game_event_queue, al_get_mouse_event_source());
    // TODO: Register other event sources such as timer, video, ...

    // Start the timer to update and draw the game.
    al_start_timer(game_update_timer);
}

void game_init(void) {
    /* Shared resources*/
    font_pirulen_32 = al_load_font("pirulen.ttf", 32, 0);
    if (!font_pirulen_32)
        game_abort("failed to load font: pirulen.ttf with size 32");

    font_pirulen_24 = al_load_font("pirulen.ttf", 24, 0);
    if (!font_pirulen_24)
        game_abort("failed to load font: pirulen.ttf with size 24");

    /* Menu Scene resources*/
    main_img_background = load_bitmap_resized("angryplane.jpg", SCREEN_W, SCREEN_H);

    main_bgm = al_load_sample("menusong.ogg");
    if (!main_bgm)
        game_abort("failed to load audio: menusong.ogg");

    gun_bgm = al_load_sample("gun.ogg");
    if (!gun_bgm)
        game_abort("failed to load audio: gun.ogg");
    skill_bgm = al_load_sample("skill.ogg");
    if (!skill_bgm)
        game_abort("failed to load audio: skill.ogg");

    // [HACKATHON 3-4]
    // TODO: Load settings images.
    // Don't forget to check their return values.
    // Uncomment and fill in the code below.
    //img_settings = al_load_bitmap("settings.png");
    img_settings= load_bitmap_resized("settings.png", 38, 38);
    if (!img_settings)
        game_abort("failed to load image: settings.png");
    img_settings2 = al_load_bitmap("settings2.png");
    if (!img_settings2)
        game_abort("failed to load image: settings2.png");
    //set vol
    img_vol =load_bitmap_resized("setting_vol.png", 100, 100);
    if (!img_vol)
        game_abort("failed to load image: settings_vol.png");
    img_vol2= load_bitmap_resized("setting_vol2.png", 100, 100);
    if (!img_vol2)
        game_abort("failed to load image: setting_vol2.jpg");

    img_howtoplay= load_bitmap_resized("howtoplay.png", 200, 200);
    if (!img_howtoplay)
        game_abort("failed to load image: howtoplay.png");
    img_howtoplay_bg= load_bitmap_resized("howto.png", SCREEN_W, SCREEN_H);
    if (!img_howtoplay_bg)
        game_abort("failed to load image: howto.png");

    img_char= load_bitmap_resized("char.png", 38, 38);
    if (!img_char)
        game_abort("failed to load image: char.jpg");

    /* Start Scene resources*/
    start_img_background = load_bitmap_resized("classroom.jpg", SCREEN_W, SCREEN_H);

    start_img_plane = al_load_bitmap("plane.png");
    if (!start_img_plane)
        game_abort("failed to load image: plane.png");
    //boss
    img_boss = load_bitmap_resized("boss.png", 150, 150);
    if (!img_boss)
      game_abort("failed to load image: boss.png");
    //
    start_img_enemy = al_load_bitmap("ha.png");
    if (!start_img_enemy)
        game_abort("failed to load image: ha.png");

    start_bgm = al_load_sample("heartache.ogg");
    if (!start_bgm)
        game_abort("failed to load audio: heartache.ogg");

    //by me
    /* GAMEOVER Scene resources*/
    gameover_img_background = load_bitmap_resized("game_over.png", SCREEN_W, SCREEN_H);
    gameover_bgm = al_load_sample("laugh.ogg");
    if (!gameover_bgm)
        game_abort("failed to load audio: laugh.ogg");
    bossdie_bgm = al_load_sample("bossdie.ogg");
    if (!bossdie_bgm)
        game_abort("failed to load audio: bossdie.ogg");

    setting_bgm = al_load_sample("kirby.ogg");
    if (!setting_bgm)
        game_abort("failed to load audio: laugh.ogg");

    char_bgm = al_load_sample("char.ogg");
    if (!char_bgm)
        game_abort("failed to load audio: laugh.ogg");
    win_bgm = al_load_sample("win.ogg");
    if (!win_bgm)
        game_abort("failed to load audio: win.ogg");
    //by me
    // [HACKATHON 2-5]
    // TODO: Initialize bullets.
    // 1) Search for a bullet image online and put it in your project.
    //    You can use the image we provided.
    // 2) Load it in by 'al_load_bitmap' or 'load_bitmap_resized'.
    // 3) If you use 'al_load_bitmap', don't forget to check its return value.
    // Uncomment and fill in the code below.
    img_bullet = al_load_bitmap("angry.png");
    if (!img_bullet)
        game_abort("failed to load image: angry.png");
    img_skill = load_bitmap_resized("skill.jpg", SCREEN_W/2, SCREEN_H/2);
    if (!img_skill)
        game_abort("failed to load image: skill.jpg");
    img_sword = load_bitmap_resized("sword.png", SCREEN_W/2, SCREEN_H/2);
    if (!img_sword)
        game_abort("failed to load image: sword.jpg");
    //char
    img_char1 = load_bitmap_resized("plane.png",200,200);
    if (!img_char1)
        game_abort("failed to load image: char1.jpg");
    img_char2 = load_bitmap_resized("char2.png",200,200);
    if (!img_char2)
        game_abort("failed to load image: char2.jpg");
    img_char3 = load_bitmap_resized("char3.png",200,200);
    if (!img_char3)
        game_abort("failed to load image: char3.jpg");

    img_win = load_bitmap_resized("win.jpg",SCREEN_W,SCREEN_H);
    if (!img_win)
        game_abort("failed to load image: win.jpg");

    img_bossbull = load_bitmap_resized("apple.png",42,42);
    if (!img_bossbull)
        game_abort("failed to load image: apple.jpg");
    img_bigapple = load_bitmap_resized("apple.png",100,100);
    if (!img_bigapple)
        game_abort("failed to load image: apple.jpg");
    // Change to first scene.
    game_change_scene(SCENE_MENU);
}

void game_start_event_loop(void) {
    bool done = false;
    ALLEGRO_EVENT event;
    int redraws = 0;
    while (!done) {
        al_wait_for_event(game_event_queue, &event);
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            // Event for clicking the window close button.
            game_log("Window close button clicked");
            done = true;
        } else if (event.type == ALLEGRO_EVENT_TIMER) {
            // Event for redrawing the display.
            if (event.timer.source == game_update_timer)
                // The redraw timer has ticked.
                redraws++;
        } else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            // Event for keyboard key down.
            game_log("Key with keycode %d down", event.keyboard.keycode);
            key_state[event.keyboard.keycode] = true;
            on_key_down(event.keyboard.keycode);
        } else if (event.type == ALLEGRO_EVENT_KEY_UP) {
            // Event for keyboard key up.
            game_log("Key with keycode %d up", event.keyboard.keycode);
            key_state[event.keyboard.keycode] = false;
        } else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            // Event for mouse key down.
            game_log("Mouse button %d down at (%d, %d)", event.mouse.button, event.mouse.x, event.mouse.y);
            mouse_state[event.mouse.button] = true;
            on_mouse_down(event.mouse.button, event.mouse.x, event.mouse.y);
        } else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
            // Event for mouse key up.
            game_log("Mouse button %d up at (%d, %d)", event.mouse.button, event.mouse.x, event.mouse.y);
            mouse_state[event.mouse.button] = false;
        } else if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
            if (event.mouse.dx != 0 || event.mouse.dy != 0) {
                // Event for mouse move.
                // game_log("Mouse move to (%d, %d)", event.mouse.x, event.mouse.y);
                mouse_x = event.mouse.x;
                mouse_y = event.mouse.y;
            } else if (event.mouse.dz != 0) {
                // Event for mouse scroll.
                game_log("Mouse scroll at (%d, %d) with delta %d", event.mouse.x, event.mouse.y, event.mouse.dz);
            }
        }
        // TODO: Process more events and call callbacks by adding more
        // entries inside Scene.

        // Redraw
        if (redraws > 0 && al_is_event_queue_empty(game_event_queue)) {
            // if (redraws > 1)
            //     game_log("%d frame(s) dropped", redraws - 1);
            // Update and draw the next frame.
            game_update();
            game_draw();
            redraws = 0;
        }
    }
}
void game_update(void) {
    if (active_scene == SCENE_START) {

        //by me
        //disappear if shot
        for (int i = 0; i<MAX_ENEMY;i++)
        {
            for(int ii = 0; ii<MAX_BULLET;ii++)
            {
                if (bullets[ii].x+bullets[ii].w/2>=enemies[i].x-enemies[i].w/2&&bullets[ii].x-bullets[ii].w/2<=enemies[i].x+enemies[i].w/2&&bullets[ii].y-bullets[ii].h/2<=enemies[i].y+enemies[i].h/2)
                {
                    enemies[i].hidden = true, bullets[ii].hidden = true;
                    enemies[i].hidden = true, bullets[ii].hidden = true;
                    enemies[i].x = 2*SCREEN_W, bullets[ii].x = -2*SCREEN_W;
                    enemies[i].y = 2*SCREEN_H, bullets[ii].y = -2*SCREEN_H;
                    point++;
                    plane.angerpoint++;
                }
            }
        }
         //hit by enemy
        int dirx;
        for (int i = 0; i<MAX_ENEMY; i++)
        {
            if (enemies[i].x +enemies[i].w/2>= plane.x - plane.w/2 && enemies[i].x -enemies[i].w/2<= plane.x + plane.w/2 && enemies[i].y<=plane.y+plane.h/2 &&enemies[i].y >plane.y-plane.h/2)
                {
                    enemies[i].hidden = true;
                    enemies[i].x = 2*SCREEN_W;
                    enemies[i].y = 2*SCREEN_H;
                    plane.hp = plane.hp - 1;
                    if(!plane.hp) game_change_scene(4);
                }
        }
        //enemies move and hide
        for (int i = 0; i<MAX_ENEMY; i++)
        {
            dirx = rand()%2+1;
            if (enemies[i].hidden == true)
                continue;
            if(dirx == 1 && enemies[i].x <= SCREEN_W- enemies[i].w/2) enemies[i].x += enemies[i].vx;
            else if(dirx ==2 && enemies[i].x >= enemies[i].w/2) enemies[i].x -= enemies[i].vx;
            enemies[i].y += sqrt((enemies[i].v*enemies[i].v) - (enemies[i].vx*enemies[i].vx));
            if(point>=10)
            {
                    enemies[i].y  += 2*point/10;
            }
            if (enemies[i].y >= SCREEN_H-(enemies[i].w/2))
                enemies[i].hidden = true;
        }
        //hide enemies remake
        for (int i = 0; i<MAX_ENEMY;i++)
        {
            if (enemies[i].hidden == true&&point<BOSSPOINT)
            {
                enemies[i].hidden = false;
                enemies[i].x = enemies[i].w / 2 + (float)rand() / RAND_MAX * (SCREEN_W - enemies[i].w);
                enemies[i].y = 0;
                enemies[i].v = (rand() % 5) + 1 + 8; //至少多快
                enemies[i].vx = rand() % (int)(enemies[i].v-8) +1 + 7;
            }
        }
        //BOSS
        if(point>=BOSSPOINT&&boss.hp>=0)
        {
            bossappeared = 1;
            for(int i = 0;i<MAX_ENEMY;i++)
            {
                enemies[i].hidden = true;
                enemies[i].x = 2*SCREEN_W;
                enemies[i].y = 2*SCREEN_H;
            }
            boss.x = SCREEN_W/2;
            boss.y = SCREEN_H/2 - 50;
        }

        //boss shoot bullet
        if(bossappeared)
        {
            //boss died
            if(boss.hp<=0)
            {
                boss.x += 3;
                boss.y -= 3;
                if(boss.x>=SCREEN_W||boss.y<=0)
                {
                    game_change_scene(7);
                }
            }
            //boss hit plane
            if(plane.x+plane.w/2>=boss.x-boss.w/2&&plane.x-plane.w/2<=boss.x+boss.w/2&&plane.y-plane.h/2<=boss.y+boss.h/2)
            {
                plane.hp = 0;
                game_change_scene(SCENE_GAMEOVER);
            }
            //boss shoot bullet
            double nowb = al_get_time();
            for(int i = 0;i<MAX_APPLE;i++)
            {
                if (apples[i].hidden == true && nowb - boss_last_shoot>= BOSS_COOL_DOWN)
                {
                    boss_last_shoot = nowb;
                    apples[i].x = boss.x-boss.w/2+(rand()%(int)boss.w+1);
                    apples[i].y = boss.y + boss.h/2;
                    apples[i].hidden = false;
                    break;
                }
            }
            for(int i = 0; i<MAX_APPLE;i++)
            {
                if(apples[i].hidden == true) continue;
                apples[i].x += 0;
                apples[i].y += apples[i].vy;
                if (apples[i].y >= SCREEN_H-(apples[i].w/2))
                apples[i].hidden = true;
            }
            //hit by apple
            for (int i = 0; i<MAX_ENEMY; i++)
            {
                if (apples[i].x < plane.x + plane.w/2 && apples[i].x > plane.x - plane.w/2 && apples[i].y >plane.y-plane.h/2&& apples[i].y<= plane.y)
                    {
                        apples[i].hidden = true;
                        apples[i].x = 2*SCREEN_W;
                        apples[i].y = 2*SCREEN_H;
                        plane.hp = plane.hp - 2;
                    }
            }
            //BIG APPLE
            dirapple = rand()%2;
            double nowbig = al_get_time();
            for(int i = 0;i<MAX_BIG;i++)
            {
                if (bigapple[i].hidden == true && nowbig - big_last_shoot>= BIG_COOL_DOWN)
                {
                    boss_last_shoot = nowbig;
                    bigapple[i].x = boss.x+(dirapple? -150:150);
                    bigapple[i].y = 0-bigapple[i].h/2;
                    bigapple[i].hidden = false;
                    break;
                }
            }
            for(int i = 0; i<MAX_BIG;i++)
            {
                if(bigapple[i].hidden == true) continue;
                bigapple[i].x += 0;
                bigapple[i].y += bigapple[i].vy;
                if (bigapple[i].y >= SCREEN_H-(bigapple[i].w/2))
                bigapple[i].hidden = true;
            }
            //hit by big
            for (int i = 0; i<MAX_ENEMY; i++)
            {
                if (bigapple[i].x < plane.x + plane.w/2 && bigapple[i].x > plane.x - plane.w/2 && bigapple[i].y >plane.y-plane.h/2&& bigapple[i].y<= plane.y)
                    {
                        bigapple[i].hidden = true;
                        bigapple[i].x = 2*SCREEN_W;
                        bigapple[i].y = 2*SCREEN_H;
                        plane.hp = plane.hp - 2;
                    }
            }
            //bullet hit boss
            for(int i = 0; i<MAX_BULLET;i++)
            {
                if(bullets[i].x+bullets[i].w/2>=boss.x-boss.w/2&&bullets[i].x-bullets[i].w/2<=boss.x+boss.w/2&&bullets[i].y-bullets[i].h/2<=boss.y+boss.h/2)
                {
                    boss.hp--;
                    plane.angerpoint++;
                    point++;
                    bullets[i].hidden = true;
                    bullets[i].x = -2*SCREEN_W; bullets[i].y = -2*SCREEN_H;
                }
            }

        }
        if(plane.hp<=0) game_change_scene(4);

        //by me
        plane.vx = plane.vy = 0;
        if (key_state[ALLEGRO_KEY_UP] || key_state[ALLEGRO_KEY_W]) //health point rec
            plane.vy -= 1;
        if (key_state[ALLEGRO_KEY_DOWN] || key_state[ALLEGRO_KEY_S])
            plane.vy += 1;
        if (key_state[ALLEGRO_KEY_LEFT] || key_state[ALLEGRO_KEY_A])
            plane.vx -= 1;
        if (key_state[ALLEGRO_KEY_RIGHT] || key_state[ALLEGRO_KEY_D])
            plane.vx += 1;
        // 0.71 is (1/sqrt(2)).
        plane.y += plane.vy * 4 * (plane.vx ? 0.71f : 1);
        plane.x += plane.vx * 4 * (plane.vy ? 0.71f : 1);
        // [HACKATHON 1-1]
        // TODO: Limit the plane's collision box inside the frame.
        //       (x, y axes can be separated.)
        // Uncomment and fill in the code below.
        if (plane.x - plane.w/2 < 0)
            plane.x = plane.w/2;
        else if (plane.x +plane.w/2> SCREEN_W)
            plane.x = SCREEN_W - plane.w/2;
        if (plane.y - plane.h/2 < 0)
            plane.y = plane.h/2;
        else if (plane.y +plane.h/2 > SCREEN_H)
            plane.y = SCREEN_H - plane.h/2;
        // [HACKATHON 2-7]
        // TODO: Update bullet coordinates.
        // 1) For each bullets, if it's not hidden, update x, y
        // according to vx, vy.
        // 2) If the bullet is out of the screen, hide it.
        // Uncomment and fill in the code below.
        //int i;
        for (int i = 0; i<MAX_BULLET; i++) {
            if (bullets[i].hidden == true)
                continue;
            bullets[i].x += bullets[i].vx;
            bullets[i].y += bullets[i].vy;
            if (bullets[i].y < 0)
                bullets[i].hidden = true;
        }

        // [HACKATHON 2-8]
        // TODO: Shoot if key is down and cool-down is over.
        // 1) Get the time now using 'al_get_time'.
        // 2) If Space key is down in 'key_state' and the time
        //    between now and last shoot is not less that cool
        //    down time.
        // 3) Loop through the bullet array and find one that is hidden.
        //    (This part can be optimized.)
        // 4) The bullet will be found if your array is large enough.
        // 5) Set the last shoot time to now.
        // 6) Set hidden to false (recycle the bullet) and set its x, y to the
        //    front part of your plane.
        // Uncomment and fill in the code below.
        double now = al_get_time();
        if (key_state[ALLEGRO_KEY_SPACE] && now - last_shoot_timestamp >= MAX_COOLDOWN)
        {
            for (int i = 0; i<MAX_BULLET;i++) {
                if (bullets[i].hidden == true)
                {
                    last_shoot_timestamp = now;
                    bullets[i].hidden = false;
                    bullets[i].x = plane.x;
                    bullets[i].y = plane.y-plane.h/2;
                    if(volok)
                    {
                        if (!al_play_sample(gun_bgm, 0.2, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &gun_bgm_id))
                        game_abort("failed to play audio (bgm)");
                    }
                    break;
                }
            }
        }
        if(skill.x>-skill.w/2 && usedskill ==1)
        {
            skill.hidden = false;
            skill.x -= skill.vx;
            sword.x-= skill.vx;
            sword2.x -=skill.vx;

        }
        if(skill.x<=-skill.w/2&&usedskill == 1)
        {
            usedskill = 0;
            skill.x = SCREEN_W+skill.w/2;
            sword.x = SCREEN_W+sword.w/2;
            sword2.x = SCREEN_W+sword2.w/2;
        }


    }

}

void game_draw(void) {
    if (active_scene == SCENE_MENU) {
        al_draw_bitmap(main_img_background, 0, 0, 0);
        al_draw_text(font_pirulen_32, al_map_rgb(255, 255, 255), SCREEN_W / 2, 30, ALLEGRO_ALIGN_CENTER, "Space Shooter");
        al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), 20, SCREEN_H - 50, 0, "Press enter key to start");
        // [HACKATHON 3-5]
        // TODO: Draw settings images.
        // The settings icon should be located at (x, y, w, h) =
        // (SCREEN_W - 48, 10, 38, 38).
        // Change its image according to your mouse position.
        // Uncomment and fill in the code below.
        if (pnt_in_rect(mouse_x, mouse_y, SCREEN_W - 48, 10, 38,38))
            al_draw_bitmap(img_settings2, SCREEN_W - 48, 10, 0);
        else
            al_draw_bitmap(img_settings, SCREEN_W - 48, 10, 0);
        //char
        if (pnt_in_rect(mouse_x, mouse_y, SCREEN_W - 48, 65, 38,38))
        {
            al_draw_filled_rectangle(SCREEN_W - 48, 65, SCREEN_W-10,103, al_map_rgb(25, 255, 255));
            al_draw_bitmap(img_char, SCREEN_W - 48, 65, 0);
        }
        else
        {
            al_draw_filled_rectangle(SCREEN_W - 48, 65, SCREEN_W-10,103, al_map_rgb(255, 255, 255));
            al_draw_bitmap(img_char, SCREEN_W - 48, 65, 0);
        }

    } else if (active_scene == SCENE_START) {
        int i;
        al_draw_bitmap(start_img_background, 0, 0, 0);
        //by me point
        sprintf (pointstr, "%d", point);
        al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), SCREEN_W -125, 25, ALLEGRO_ALIGN_CENTRE, "score:");
        al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), SCREEN_W -40, 25, ALLEGRO_ALIGN_CENTRE, pointstr);
        //hp
        sprintf (hpstr, "%d", plane.hp);
        al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), SCREEN_W -125, 50, ALLEGRO_ALIGN_CENTRE, "HP:");
        al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), SCREEN_W -40, 50, ALLEGRO_ALIGN_CENTRE, hpstr);

        //hp update by me
        al_draw_filled_rectangle(plane.x-plane.w/2,plane.y+plane.h/2+2,plane.x+plane.w/2-(plane.w*(10-plane.hp)/10),plane.y+plane.h/2+6,al_map_rgb(255,255,0));
        // plane hp

        //plane angerpoint
        sprintf (angerstr, "%d", (int)plane.angerpoint);
        al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), SCREEN_W -125, 75, ALLEGRO_ALIGN_CENTRE, "RAGE:");
        al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), SCREEN_W -40, 75, ALLEGRO_ALIGN_CENTRE, angerstr);
        al_draw_filled_rectangle(0,SCREEN_H-5,SCREEN_W*((float)plane.angerpoint/15),SCREEN_H,al_map_rgb(255,0,0));
        al_draw_text(font_pirulen_24, al_map_rgb(255, 0, 0), 0, SCREEN_H-40, ALLEGRO_ALIGN_LEFT, "RAGE:");

        // [HACKATHON 2-9]
        // TODO: Draw all bullets in your bullet array.
        // Uncomment and fill in the code below.
        for (int i =0 ; i<MAX_APPLE;i++)
            draw_movable_object(apples[i]);
        for (int i =0 ; i<MAX_BULLET;i++)
            draw_movable_object(bullets[i]);
        for (int i =0 ; i<MAX_BIG;i++)
            draw_movable_object(bigapple[i]);
        draw_movable_object(plane);
        for (i = 0; i < MAX_ENEMY; i++)
            draw_movable_object(enemies[i]);
        //skill
        draw_movable_object(skill);
        draw_movable_object(sword);
        draw_movable_object(sword2);

        //BOSS
        draw_movable_object(boss);
        //boss hp
        if(bossappeared)
        {
            sprintf (bosshpstr, "%d", boss.hp);
            al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), 5, 50, ALLEGRO_ALIGN_LEFT, "BOSS HP:");
            al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), 175, 50, ALLEGRO_ALIGN_LEFT, bosshpstr);
        }

        //hp update by me
        al_draw_filled_rectangle(boss.x-boss.w/2,boss.y+boss.h/2+2,boss.x+boss.w/2-(boss.w*(100-boss.hp)/100),boss.y+boss.h/2+6,al_map_rgb(255,0,0));
    }
    // [HACKATHON 3-9]
    // TODO: If active_scene is SCENE_SETTINGS.
    // Draw anything you want, or simply clear the display.
    else if (active_scene == SCENE_SETTINGS) {
        al_clear_to_color(al_map_rgb(0, 0, 0));
        if(volok == 1) al_draw_bitmap(img_vol, SCREEN_W/2, SCREEN_H/2,0);
        if (pnt_in_rect(mouse_x, mouse_y, SCREEN_W/2, SCREEN_H/2, 100,100) || volok ==0)
        {
            al_draw_bitmap(img_vol2,  SCREEN_W/2, SCREEN_H/2,0);
        }
        if(volok == 0 &&pnt_in_rect(mouse_x, mouse_y, SCREEN_W/2, SCREEN_H/2, 100,100)) al_draw_bitmap(img_vol, SCREEN_W/2, SCREEN_H/2,0);
        al_draw_bitmap(img_howtoplay, 100, SCREEN_H-350,0);
        if (pnt_in_rect(mouse_x, mouse_y, 100, SCREEN_H-350, 200,200))
        {
            al_draw_text(font_pirulen_32, al_map_rgb(255, 255, 255), 90, SCREEN_H-250, ALLEGRO_ALIGN_LEFT, "*!");
        }

    }
     else if (active_scene == SCENE_GAMEOVER)
     {
         al_clear_to_color(al_map_rgb(0, 0, 0));
         al_draw_bitmap(gameover_img_background, 0, 0, 0);
         al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H/2, ALLEGRO_ALIGN_CENTRE, "GAMEOVER!XP");
         al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), SCREEN_W/2+70, SCREEN_H/2+50, ALLEGRO_ALIGN_LEFT, pointstr);
         al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H/2+50, ALLEGRO_ALIGN_CENTRE, "score:");
         al_draw_text(font_pirulen_24, al_map_rgb(255, 160, 0), SCREEN_W/2, SCREEN_H/2+230, ALLEGRO_ALIGN_CENTRE, "GET KILLED BY LAUGHING EMOJI");
         al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H/2+160, ALLEGRO_ALIGN_CENTRE, "RESTART!!");
         al_draw_rectangle(300, SCREEN_H/2+160+34, 500, SCREEN_H/2+160-6, al_map_rgb(255, 255, 255), 4);
         if (pnt_in_rect(mouse_x, mouse_y, 300,  SCREEN_H/2+154, 200, 40))
           al_draw_rectangle(300, SCREEN_H/2+160+34, 500, SCREEN_H/2+160-6, al_map_rgb(25, 255, 255), 4);
        int a[3];
        if(!rank_stored)
        {
            rank_stored = true;
            FILE* fp = fopen("rank.txt", "r");
            fscanf(fp,"%d %d %d", &a[0], &a[1], &a[2]);
            fclose(fp);

            for(int  i =0;i<3;i++)
            {
                if(point>a[i])
                {
                    for(int j = 2;j>i;j--) a[j] = a[j-1];
                    a[i] = point;
                    break;
                }
            }
            fp = fopen("rank.txt", "w");
            fprintf(fp,"%d %d %d", a[0], a[1], a[2]);
            fclose(fp);
        }
        FILE* fp = fopen("rank.txt", "r");
        fscanf(fp, "%d %d %d", &a[0], &a[1], &a[2]);
        fclose(fp);
        al_draw_text(font_pirulen_24, al_map_rgb(255,255,255),5, 20, ALLEGRO_ALIGN_LEFT, "ranking list");
         al_draw_textf(font_pirulen_24, al_map_rgb(255,255,255), 5,50 , ALLEGRO_ALIGN_LEFT, "1. %d", a[0]);
         al_draw_textf(font_pirulen_24, al_map_rgb(255,255,255), 5, 80, ALLEGRO_ALIGN_LEFT, "2. %d", a[1]);
         al_draw_textf(font_pirulen_24, al_map_rgb(255,255,255),5, 110, ALLEGRO_ALIGN_LEFT, "3. %d", a[2]);

     }

     else if (active_scene == SCENE_CHAR)
     {
         al_clear_to_color(al_map_rgb(0, 0, 0));
         al_draw_rectangle(50, SCREEN_H-100, SCREEN_W-50,SCREEN_H-10, al_map_rgb(255, 255, 255), 4);
         al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), 60, SCREEN_H-75, ALLEGRO_ALIGN_LEFT, "*Choose a character.");

         al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), SCREEN_W/2-80, SCREEN_H/2-100, ALLEGRO_ALIGN_LEFT, "yakumo");
         al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), SCREEN_W/2-80, SCREEN_H/2-70, ALLEGRO_ALIGN_LEFT,  "  ran");
         al_draw_bitmap(img_char2, SCREEN_W/2-100, SCREEN_H/2, 0);
         al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), SCREEN_W/2+120, SCREEN_H/2-100, ALLEGRO_ALIGN_LEFT, "Hakurei");
         al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), SCREEN_W/2+120, SCREEN_H/2-70, ALLEGRO_ALIGN_LEFT,  " Reimu");
         al_draw_bitmap(img_char3, SCREEN_W/2-100+200, SCREEN_H/2, 0);
         al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), SCREEN_W/2-100-200, SCREEN_H/2-100, ALLEGRO_ALIGN_LEFT, "happiness");
         al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), SCREEN_W/2-100-200, SCREEN_H/2-70, ALLEGRO_ALIGN_LEFT, " crasher");
         al_draw_bitmap(img_char1, SCREEN_W/2-100-200, SCREEN_H/2, 0);
        //draw_movable_object(cloud);
        if (pnt_in_rect(mouse_x, mouse_y, SCREEN_W/2-100, SCREEN_H/2, 200,200))
        {
            al_draw_rectangle(SCREEN_W/2-100, SCREEN_H/2, SCREEN_W/2+100, SCREEN_H/2+200, al_map_rgb(25, 255, 255), 4);
        }
        if (pnt_in_rect(mouse_x, mouse_y, SCREEN_W/2-300, SCREEN_H/2, 200,200))
        {
            al_draw_rectangle(SCREEN_W/2-300, SCREEN_H/2, SCREEN_W/2-100, SCREEN_H/2+200, al_map_rgb(25, 255, 255), 4);
        }
        if (pnt_in_rect(mouse_x, mouse_y, SCREEN_W/2+100, SCREEN_H/2, 200,200))
        {
            al_draw_rectangle(SCREEN_W/2+100, SCREEN_H/2, SCREEN_W/2+300, SCREEN_H/2+200, al_map_rgb(25, 255, 255), 4);
        }


     }
     else if(active_scene == SCENE_WIN)
     {
         al_draw_bitmap(img_win, 0, 0, 0);
         al_draw_text(font_pirulen_32, al_map_rgb(255,215,0), 399, 85, ALLEGRO_ALIGN_CENTRE, "WIN!");
         al_draw_text(font_pirulen_24, al_map_rgb(255,105,180), 360, 559, ALLEGRO_ALIGN_LEFT, pointstr);
         al_draw_text(font_pirulen_24, al_map_rgb(255,105,180), 297, 559, ALLEGRO_ALIGN_CENTRE, "score:");
         al_draw_text(font_pirulen_24, al_map_rgb(0,0,0), 538,559, ALLEGRO_ALIGN_CENTRE, "RESTART!!");
         al_draw_rectangle(452,558, 621, 588, al_map_rgb(255, 255, 255), 4);
         if (pnt_in_rect(mouse_x, mouse_y,452,558,621-452, 30))
           al_draw_rectangle(452,558, 621, 588, al_map_rgb(25, 255, 255), 4);
        int a[3];
        if(!rank_stored)
        {
            rank_stored = true;
            FILE* fp = fopen("rank.txt", "r");
            fscanf(fp,"%d %d %d", &a[0], &a[1], &a[2]);
            fclose(fp);

            for(int  i =0;i<3;i++)
            {
                if(point>a[i])
                {
                    for(int j = 2;j>i;j--) a[j] = a[j-1];
                    a[i] = point;
                    break;
                }
            }
            fp = fopen("rank.txt", "w");
            fprintf(fp,"%d %d %d", a[0], a[1], a[2]);
            fclose(fp);
        }
        FILE* fp = fopen("rank.txt", "r");
        fscanf(fp, "%d %d %d", &a[0], &a[1], &a[2]);
        fclose(fp);
        al_draw_text(font_pirulen_24, al_map_rgb(0,0,0),5, 20, ALLEGRO_ALIGN_LEFT, "ranking list");
         al_draw_textf(font_pirulen_24, al_map_rgb(0,0,0), 5,50 , ALLEGRO_ALIGN_LEFT, "1. %d", a[0]);
         al_draw_textf(font_pirulen_24, al_map_rgb(0,0,0), 5, 80, ALLEGRO_ALIGN_LEFT, "2. %d", a[1]);
         al_draw_textf(font_pirulen_24, al_map_rgb(0,0,0),5, 110, ALLEGRO_ALIGN_LEFT, "3. %d", a[2]);
         if(plane.hp == 10)
         {
             al_draw_text(font_pirulen_24, al_map_rgb(255,0,0),SCREEN_W/2, 40, ALLEGRO_ALIGN_CENTRE, "PERFECT!NO ENEMY HURT YOU");
         }

     }
     else if(active_scene == SCENE_HOWTO)
     {
        al_draw_bitmap(img_howtoplay_bg, 0, 0, 0);
     }
    al_flip_display();
}

void game_destroy(void) {
    // Destroy everything you have created.
    // Free the memories allocated by malloc or allegro functions.
    // Destroy shared resources.
    al_destroy_font(font_pirulen_32);
    al_destroy_font(font_pirulen_24);

    /* Menu Scene resources*/
    al_destroy_bitmap(main_img_background);
    al_destroy_sample(main_bgm);
    al_destroy_sample(gun_bgm);
    al_destroy_sample(skill_bgm);
    // [HACKATHON 3-6]
    // TODO: Destroy the 2 settings images.
    // Uncomment and fill in the code below.
    al_destroy_bitmap(img_settings);
    al_destroy_bitmap(img_settings2);
    al_destroy_bitmap(img_char);
    al_destroy_bitmap(img_char1);
    al_destroy_bitmap(img_char2);
    al_destroy_bitmap(img_char3);
    al_destroy_bitmap(img_vol);
    al_destroy_bitmap(img_vol2);
    al_destroy_bitmap(img_howtoplay);
    al_destroy_bitmap(img_howtoplay_bg);

    /* Start Scene resources*/
    al_destroy_bitmap(start_img_background);
    al_destroy_bitmap(start_img_plane);
    al_destroy_bitmap(start_img_enemy);
    al_destroy_sample(start_bgm);
    /* gameover Scene resources*/
    al_destroy_sample(gameover_bgm);
    al_destroy_sample(bossdie_bgm);
    al_destroy_bitmap(gameover_img_background);

    al_destroy_sample(setting_bgm);
    al_destroy_sample(char_bgm);
    al_destroy_sample(win_bgm);

    // [HACKATHON 2-10]
    // TODO: Destroy your bullet image.
    // Uncomment and fill in the code below.
    al_destroy_bitmap(img_bullet);
    al_destroy_bitmap(img_boss);
    al_destroy_bitmap(img_sword);
    al_destroy_bitmap(img_skill);
    al_destroy_bitmap(img_win);
    al_destroy_bitmap(img_bossbull);
    al_destroy_bitmap(img_bigapple);

    al_destroy_timer(game_update_timer);
    al_destroy_event_queue(game_event_queue);
    al_destroy_display(game_display);
    free(mouse_state);
}

void game_change_scene(int next_scene) {
    game_log("Change scene from %d to %d", active_scene, next_scene);
    // TODO: Destroy resources initialized when creating scene.
    if (active_scene == SCENE_MENU ) {
        al_stop_sample(&main_bgm_id);
        game_log("stop audio (bgm)");
    } else if (active_scene == SCENE_START ) {
        al_stop_sample(&start_bgm_id);
        game_log("stop audio (bgm)");
    }
    else if (active_scene == SCENE_GAMEOVER )
    {
        al_stop_sample(&gameover_bgm_id);
        game_log("stop audio (bgm)");
    }
    else if (active_scene == SCENE_SETTINGS )
    {
        al_stop_sample(&setting_bgm_id);
        game_log("stop audio (bgm)");
    }
    else if (active_scene == SCENE_CHAR )
    {
        al_stop_sample(&char_bgm_id);
        game_log("stop audio (bgm)");
    }
    else if (active_scene == SCENE_WIN )
    {
        al_stop_sample(&win_bgm_id);
        game_log("stop audio (bgm)");
    }
    else if (active_scene == SCENE_HOWTO )
    {
        al_stop_sample(&setting_bgm_id);
        game_log("stop audio (bgm)");
    }
    active_scene = next_scene;
    // TODO: Allocate resources before entering scene.
    if (active_scene == SCENE_MENU)
    {
        if(volok)
        {
            if (!al_play_sample(main_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &main_bgm_id))
            game_abort("failed to play audio (bgm)");
        }
    }
    else if (active_scene == SCENE_START)
    {
        rank_stored = false;
        bossappeared = 0;
        usedskill = 0;
        BOSSPOINT = 50; //boss會出來的分數
        point=0;
        int i;
        plane.img = start_img_plane;
        plane.x = 400;
        plane.y = 500;
        plane.w = al_get_bitmap_width(plane.img);
        plane.h = al_get_bitmap_height(plane.img);
        plane.hp = 10;
        plane.angerpoint = 0;
        for (i = 0; i < MAX_ENEMY; i++) {
            enemies[i].img = start_img_enemy;
            enemies[i].w = al_get_bitmap_width(start_img_enemy);
            enemies[i].h = al_get_bitmap_height(start_img_enemy);
            enemies[i].x = enemies[i].w / 2 + (float)rand() / RAND_MAX * (SCREEN_W - enemies[i].w);
            enemies[i].y = 0;
            //crazy
            enemies[i].v = (rand() % 5) + 1 + 8; //至少多快
            enemies[i].vx = rand() % (int)(enemies[i].v-8) +1 + 7;
            //enemies[i].vy = 3;
            enemies[i].hidden = false;
        }
        for (i = 0; i < MAX_APPLE; i++) {
            apples[i].img = img_bossbull;
            apples[i].w = al_get_bitmap_width(img_bossbull);
            apples[i].h = al_get_bitmap_height(img_bossbull);
            apples[i].x = 3*SCREEN_W;
            apples[i].y = 3*SCREEN_H/2;
            //crazy
            apples[i].vx = 5;
            apples[i].vy = 3;
            apples[i].hidden = true;
        }
        for (i = 0; i < MAX_BIG; i++)
        {
            bigapple[i].img = img_bigapple;
            bigapple[i].w = al_get_bitmap_width(img_bigapple);
            bigapple[i].h = al_get_bitmap_height(img_bigapple);
            bigapple[i].x = 3*SCREEN_W;
            bigapple[i].y = 3*SCREEN_H/2;
            bigapple[i].vy = 3;
            bigapple[i].hidden = true;
        }
        // [HACKATHON 2-6]
        // TODO: Initialize bullets.
        // For each bullets in array, set their w and h to the size of
        // the image, and set their img to bullet image, hidden to true,
        // (vx, vy) to (0, -3).
        // Uncomment and fill in the code below.
        for (int i = 0; i<MAX_BULLET;i++) {
            bullets[i].w = al_get_bitmap_width(img_bullet);
            bullets[i].h = al_get_bitmap_height(img_bullet);
            bullets[i].img = img_bullet;
            bullets[i].vx = 0;
            bullets[i].vy = -3;
            bullets[i].hidden = true;
        }
        if(volok ==1)
        {
             if (!al_play_sample(start_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &start_bgm_id))
            game_abort("failed to play audio (bgm)");
        }
        //skill
        skill.w = al_get_bitmap_width(img_skill);
        skill.h = al_get_bitmap_height(img_skill);
        skill.x = SCREEN_W+skill.w/2;
        skill.y = SCREEN_H/2;
        skill.vx = 20;
        skill.img = img_skill;
        skill.hidden = true;

        //boss
        boss.img = img_boss;
        boss.hp = 100;
        boss.w = al_get_bitmap_width(img_boss);
        boss.h = al_get_bitmap_height(img_boss);
        boss.x = 3*SCREEN_W;
        boss.y = 3*SCREEN_H/2;


        sword.w = al_get_bitmap_width(img_sword);
        sword.h = al_get_bitmap_height(img_sword);
        sword.x = SCREEN_W+sword.w/2;
        sword.y = SCREEN_H/4-50;
        sword.vx = 20;
        sword.img = img_sword;

        sword2.w = al_get_bitmap_width(img_sword);
        sword2.h = al_get_bitmap_height(img_sword);
        sword2.x = SCREEN_W+sword2.w/2;
        sword2.y = 3*SCREEN_H/4+50;
        sword2.vx = 20;
        sword2.img = img_sword;
    }
    else if (active_scene == SCENE_GAMEOVER)
    {
        if(volok)
        {
            if (!al_play_sample(gameover_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &gameover_bgm_id))
            game_abort("failed to play audio (bgm)");
        }
    }
    else if (active_scene == SCENE_SETTINGS)
    {
        if(volok)
        {
            if (!al_play_sample(setting_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &setting_bgm_id))
            game_abort("failed to play audio (bgm)");
        }
    }
    else if (active_scene == SCENE_CHAR)
    {
        if(volok)
        {
            if (!al_play_sample(char_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &char_bgm_id))
            game_abort("failed to play audio (bgm)");
        }
    }
     else if (active_scene == SCENE_WIN)
    {
        if(volok)
        {
            if (!al_play_sample(win_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &win_bgm_id))
            game_abort("failed to play audio (bgm)");
        }
    }
     else if (active_scene == SCENE_HOWTO)
    {
        if(volok)
        {
            if (!al_play_sample(setting_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &setting_bgm_id))
            game_abort("failed to play audio (bgm)");
        }
    }
}

void on_key_down(int keycode) {
    if (active_scene == SCENE_MENU) {
        if (keycode == ALLEGRO_KEY_ENTER)
            game_change_scene(SCENE_START);
    }
    // [HACKATHON 3-10]
    // TODO: If active_scene is SCENE_SETTINGS and Backspace is pressed,
    // return to SCENE_MENU.
    else if (active_scene == SCENE_SETTINGS)
    {
        if (keycode == ALLEGRO_KEY_BACKSPACE )
            game_change_scene(SCENE_MENU);

    }
    else if (active_scene == SCENE_START)
    {
        if(boss.hp<=0)
        {
            if(volok)
                {
                    if (!al_play_sample(bossdie_bgm, 4, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &bossdie_bgm_id))
                    game_abort("failed to play audio (bgm)");
                }
        }

        if (keycode == ALLEGRO_KEY_Z && plane.angerpoint>=15)
        {
            if(volok)
            {
                if (!al_play_sample(skill_bgm, 5, 0.0,1.0, ALLEGRO_PLAYMODE_ONCE, &skill_bgm_id))
                game_abort("failed to play audio (bgm)");
            }
            usedskill = 1;
            plane.angerpoint -= 15;
            for(int i = 0;i<MAX_ENEMY;i++)
            {
                enemies[i].y = -2*SCREEN_H;
            }
            if(bossappeared)
            {
                boss.hp -= 20;
                point+=20;
            }
            else
            {
                point+=MAX_ENEMY;
            }
        }
        if (keycode == ALLEGRO_KEY_BACKSPACE )
            game_change_scene(SCENE_MENU);
    }
    else if(active_scene == SCENE_CHAR)
    {
        if (keycode == ALLEGRO_KEY_BACKSPACE )
            game_change_scene(SCENE_MENU);
    }
    else if(active_scene == SCENE_HOWTO)
    {
        if (keycode == ALLEGRO_KEY_BACKSPACE )
            game_change_scene(SCENE_SETTINGS);
    }


}

void on_mouse_down(int btn, int x, int y) {
    // [HACKATHON 3-8]
    // TODO: When settings clicked, switch to settings scene.
    // Uncomment and fill in the code below.
    if (active_scene == SCENE_MENU) {
        if (btn==true) {
            if (pnt_in_rect(x, y, SCREEN_W - 48, 10, 38, 38))
                game_change_scene(3);
            if(pnt_in_rect(mouse_x, mouse_y, SCREEN_W - 48, 65, 38,38))
                game_change_scene(5);
        }
    }
    //by me
    if (active_scene == SCENE_GAMEOVER)
    {
        if (btn==true)
        {
            if (pnt_in_rect(x, y, 300,  SCREEN_H/2+154, 200, 40)) game_change_scene(1);
        }
    }
    if (active_scene == SCENE_SETTINGS)
    {
        if (btn==true)
        {
            if (pnt_in_rect(mouse_x, mouse_y, SCREEN_W/2, SCREEN_H/2, 100,100))
            {
                if(volok == 1) volok =0;
                else volok = 1;
                game_change_scene(1);
            }
            if (pnt_in_rect(mouse_x, mouse_y, 100, SCREEN_H-350, 200,200))
            {
                game_change_scene(6);
            }
        }

    }
    if (active_scene == SCENE_CHAR)
    {
        if (btn==true)
        {
            if (pnt_in_rect(x, y,  SCREEN_W/2-300, SCREEN_H/2 ,200,200))
            {
                start_img_plane = load_bitmap_resized("plane.png",60,60);
                game_change_scene(1);
            }
            if (pnt_in_rect(x, y, SCREEN_W/2-100, SCREEN_H/2, 200, 200))
            {
                start_img_plane = load_bitmap_resized("char2.png",60,60);
                game_change_scene(1);
            }
            if (pnt_in_rect(x, y,  SCREEN_W/2+100, SCREEN_H/2, 200,200))
            {
                start_img_plane = load_bitmap_resized("char3.png",60,60);
                game_change_scene(1);
            }
        }
    }
    else if(active_scene== SCENE_WIN)
    {
        if(btn==true)
        {
            if(pnt_in_rect(mouse_x, mouse_y,452,558,621-452, 30))
            {
                game_change_scene(1);

            }
        }
    }
}

void draw_movable_object(MovableObject obj) {
    if (obj.hidden)
        return;
    al_draw_bitmap(obj.img, round(obj.x - obj.w / 2), round(obj.y - obj.h / 2), 0);
}

ALLEGRO_BITMAP *load_bitmap_resized(const char *filename, int w, int h) {
    ALLEGRO_BITMAP* loaded_bmp = al_load_bitmap(filename);
    if (!loaded_bmp)
        game_abort("failed to load image: %s", filename);
    ALLEGRO_BITMAP *resized_bmp = al_create_bitmap(w, h);
    ALLEGRO_BITMAP *prev_target = al_get_target_bitmap();

    if (!resized_bmp)
        game_abort("failed to create bitmap when creating resized image: %s", filename);
    al_set_target_bitmap(resized_bmp);
    al_draw_scaled_bitmap(loaded_bmp, 0, 0,
        al_get_bitmap_width(loaded_bmp),
        al_get_bitmap_height(loaded_bmp),
        0, 0, w, h, 0);
    al_set_target_bitmap(prev_target);
    al_destroy_bitmap(loaded_bmp);

    game_log("resized image: %s", filename);

    return resized_bmp;
}

// [HACKATHON ]
// TODO: Define bool pnt_in_rect(int px, int py, int x, int y, int w, int h)
// Uncomment and fill in the code below.
bool pnt_in_rect(int px, int py, int x, int y, int w, int h) {
    if(px<= x+w && px>=x && py<=y+h && py>=y)
            return true;
    else return false;
}


// +=================================================================+
// | Code below is for debugging purpose, it's fine to remove it.    |
// | Deleting the code below and removing all calls to the functions |
// | doesn't affect the game.                                        |
// +=================================================================+

void game_abort(const char* format, ...) {
    va_list arg;
    va_start(arg, format);
    game_vlog(format, arg);
    va_end(arg);
    fprintf(stderr, "error occured, exiting after 2 secs");
    // Wait 2 secs before exiting.
    al_rest(2);
    // Force exit program.
    exit(1);
}

void game_log(const char* format, ...) {
#ifdef LOG_ENABLED
    va_list arg;
    va_start(arg, format);
    game_vlog(format, arg);
    va_end(arg);
#endif
}

void game_vlog(const char* format, va_list arg) {
#ifdef LOG_ENABLED
    static bool clear_file = true;
    // Write log to file for later debugging.
    FILE* pFile = fopen("log.txt", clear_file ? "w" : "a");
    if (pFile) {
        va_list arg2;
        va_copy(arg2, arg);
        vfprintf(pFile, format, arg2);
        va_end(arg2);
        fprintf(pFile, "\n");
        fclose(pFile);
    }
    vprintf(format, arg);
    printf("\n");
    clear_file = false;
#endif
}
